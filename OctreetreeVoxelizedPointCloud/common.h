#pragma once
#include <vtkAutoInit.h>
#include <iostream>
#include <pcl/io/io.h>
#include <pcl/io/pcd_io.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl/octree/octree_iterator.h>
#include <pcl/octree/octree_pointcloud.h>
#include <pcl/octree/octree_pointcloud_voxelcentroid.h>
#include <pcl/search/octree.h>
#include <fit_3d_plane.h>
#include <libpq-fe.h>
#include <pcl/visualization/cloud_viewer.h>
#include <pcl/visualization/pcl_plotter.h>
#include <pcl/filters/conditional_removal.h>
#include <pcl/filters/statistical_outlier_removal.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d_omp.h>
#include <pcl/surface/gp3.h>
#include <pcl/features/pfh.h>
#include <math.h>
#include <pcl/filters/voxel_grid.h>
#include <sstream>
#include <iomanip>
#include <pcl/features/fpfh_omp.h>

#define STDCOUTWHITE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE)//设置三原色相加
#define STDCOUTGREEN SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN)//设置绿色
#define STDCOUTRED SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED)//设置绿色
#define STDCOUTBLUE SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_BLUE | FOREGROUND_GREEN)//设置蓝色
#define STDCOUTYELLOW SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN | FOREGROUND_RED)//设置红绿相加
VTK_MODULE_INIT(vtkInteractionStyle);
VTK_MODULE_INIT(vtkRenderingFreeType);
VTK_MODULE_INIT(vtkRenderingOpenGL);

// 全局变量
std::string WorkPath;
//自定义点云结构
struct MyPointType
{
	PCL_ADD_POINT4D; // This adds the members x,y,z which can also be accessed using the point (which is float[4])
	PCL_ADD_RGB;           // preferred way of adding a XYZ+padding
	//float AverageReflectionIntensity;
	float Roughness;
	int Classification;
	float histogram[81];
	float tendency;
	float inclination;
	int ischeckdata;
	int boundary;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;   // make sure our new allocators are aligned
} EIGEN_ALIGN16;                    // enforce SSE padding for correct memory alignment

POINT_CLOUD_REGISTER_POINT_STRUCT(MyPointType,           // here we assume a XYZ + "intensity" (as fields)
(float, x, x)
(float, y, y)
(float, z, z)
(float, r, r)
(float, g, g)
(float, b, b)
(float, rgba, rgba)
//(float, AverageReflectionIntensity, AverageReflectionIntensity)
(float, Roughness, Roughness)
(int, Classification, Classification)
(float[81], histogram, pfh)
(float, tendency, tendency)
(float, inclination, inclination)
(int, ischeckdata, ischeckdata)
(int, boundary, boundary)
)
//自定义结构体（lujian）
struct PointType
{
	PCL_ADD_POINT4D; // This adds the members x,y,z which can also be accessed using the point (which is float[4])
	PCL_ADD_RGB;           // preferred way of adding a XYZ+padding
	PCL_ADD_UNION_NORMAL4D;
	float Roughness;
	float histogram[33];
	int fileIndex;
	int Classification;
	int boundary;
	EIGEN_MAKE_ALIGNED_OPERATOR_NEW;   // make sure our new allocators are aligned
} EIGEN_ALIGN16;                    // enforce SSE padding for correct memory alignment

POINT_CLOUD_REGISTER_POINT_STRUCT(PointType,           // here we assume a XYZ + "intensity" (as fields)
(float, x, x)
(float, y, y)
(float, z, z)
(float, r, r)
(float, g, g)
(float, b, b)
(float, rgba, rgba)
(float, normal_x, normal_x)
(float, normal_y, normal_y)
(float, normal_z, normal_z)
(float, Roughness, Roughness)
(float[33], histogram, pfh)
(int, fileIndex, fileIndex)
(int, Classification, Classification)
(int, boundary, boundary)
)

// 获取当前时间字符串
std::string GetNowTime() {
	time_t setTime;
	time(&setTime);
	tm* ptm = localtime(&setTime);
	std::string time = std::to_string(ptm->tm_year + 1900)
		+ ""
		+ std::to_string(ptm->tm_mon + 1)
		+ ""
		+ std::to_string(ptm->tm_mday)
		+ std::to_string(ptm->tm_hour) + ""
		+ std::to_string(ptm->tm_min) + ""
		+ std::to_string(ptm->tm_sec);
	return time;
}

// 验证输入文件的有效性
bool CheckFolderValidity(std::string path)
{
	intptr_t hFile = 0;               //文件句柄
	struct _finddata_t fileinfo;  //文件信息读取结构
	std::string p;
	int i = 0;
	bool result = false;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					if (strcmp(fileinfo.name, "RawData") == 0)
					{
						i++;
					}
					if (strcmp(fileinfo.name, "SimplifiedRawData") == 0)
					{
						i++;
					}
					if (strcmp(fileinfo.name, "SelectedData") == 0)
					{
						i++;
					}
					if (strcmp(fileinfo.name, "ClassifiedData") == 0)
					{
						i++;
					}
					if (strcmp(fileinfo.name, "Filtering") == 0) 
					{
						i++;
					}
					if (strcmp(fileinfo.name, "PickingResult") == 0)
					{
						i++;
					}
					if (strcmp(fileinfo.name, "NormalizedClassfication") == 0)
					{
						i++;
					}
				}
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
	if (i == 7) {
		result = true;
	}
	return result;
}
// 获取pcd文件容器
void getAllFiles(std::string path, std::vector<std::string>& files)
{
	intptr_t hFile = 0;               //文件句柄
	struct _finddata_t fileinfo;  //文件信息读取结构
	std::string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if ((fileinfo.attrib & _A_SUBDIR))    //比较文件类型是否是文件夹
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
				{
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);     //如果是文件夹，继续向下遍历
				}
			}
			else    //是文件
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));  //是文件，将其路径加入到files集合中
			}
		} while (_findnext(hFile, &fileinfo) == 0);  //寻找下一个，成功返回0，否则-1
		_findclose(hFile);
	}
}
// 链接数据库
bool ConnectToDB(PGconn *&conn, char *pghost, char *pgport, char *dbname, char *user, char *pwd)
{
	//pgoptions、pgtty参数默认为NULL
	char *pgoptions, *pgtty;
	pgoptions = NULL;
	pgtty = NULL;

	conn = PQsetdbLogin(pghost, pgport, pgoptions, pgtty, dbname, user, pwd);
	if (PQstatus(conn) == CONNECTION_BAD) // or conn==NULL 
	{
		cout << "Connection db " << dbname << " failed!" << endl;
		cout << PQerrorMessage(conn) << endl;
		return false;
	}
	else
	{
		STDCOUTGREEN;
		cout << "Connection db " << dbname << " success!" << endl;
		STDCOUTWHITE;
		return true;
	}
}
// 空间向量结构体
struct SpaceVector
{
	double x;
	double y;
	double z;
};

// 案例模型结构体
struct CEM
{
	int Id;
	double R;
	double G;
	double B;
	double Z;
	double X;
	double Y;
	double Roughness;
	double AverageReflectionIntensity;
	double tendency;
	double inclination;
	std::vector<float> histogram;
	int Classification;
	SpaceVector UnitNormal;
};


// 产状转单位法向量
SpaceVector Occurrence2UnitNormalVector(float tendency, float inclination) {
	double tendency_r = tendency * M_PI / 180;
	double inclination_r = inclination * M_PI / 180;
	SpaceVector res;
	res.x = sin(tendency_r)*sin(inclination_r);
	res.y = cos(tendency_r)*sin(inclination_r);
	res.z = cos(inclination_r);
	return res;
}

// 数组转字符串
template<typename Result, typename Para>
Result lexical_cast(Para para)
{
	std::stringstream ss;
	ss << para;
	Result result;
	ss >> result;
	return result;
}
std::string Array2string(float* a, int length) {
	std::vector<std::string> str_arr;
	for (size_t i = 0; i < length; ++i)
	{
		str_arr.push_back(lexical_cast<std::string>(a[i]));
	}

	std::string res;
	for (int i = 0; i < str_arr.size(); ++i) {
		res += str_arr[i] + ',';
	}
	res = res.substr(0, res.length() - 1);
	res += '\0';
	return res;
}

// split函数实现
void supersplit(const std::string& s, std::vector<float>& v, const std::string& c)
{
	std::string::size_type pos1, pos2;
	size_t len = s.length();
	pos2 = s.find(c);
	pos1 = 0;
	while (std::string::npos != pos2)
	{
		v.emplace_back(atof(s.substr(pos1, pos2 - pos1).c_str()));

		pos1 = pos2 + c.size();
		pos2 = s.find(c, pos1);
	}
	if (pos1 != len)
		v.emplace_back(atof(s.substr(pos1).c_str()));
}

// sqrt
float Q_rsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;   // evil floating point bit level hacking
	i = 0x5f3759df - (i >> 1); // 
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y)); // 1st iteration
	// y   = y * ( threehalfs - ( x2 * y * y ) ); // 2nd iteration, this can be removed

#ifndef Q3_VM
#ifdef __linux__
	assert(!isnan(y)); // bk010122 - FPE?
#endif
#endif
	return 1 / y;
}

// 归一化直方图
void Normal(std::vector<float>& v) {
	double Sum = 0;
	for (int i = 0; i < v.size(); i++) {
		Sum += v[i];
	}
	for (int i = 0; i < v.size(); i++) {
		v[i] = v[i] / Sum;
	}
}

// PFH
void computePointPFH(
	const pcl::PointCloud<pcl::PointXYZRGBA> &cloud, const pcl::PointCloud<pcl::Normal> &normals,
	const std::vector<int> &indices, int nr_split, Eigen::VectorXf &pfh_histogram)
{
	unsigned int max_cache_size_ = (1ul * 1024ul * 1024ul * 1024ul) / sizeof(std::pair<std::pair<int, int>, Eigen::Vector4f>);
	bool use_cache_ = true;
	float d_pi_ = 1.0f / (2.0f * static_cast<float> (M_PI));

	// Clear the resultant point histogram
	pfh_histogram.setZero(nr_split * nr_split * nr_split * nr_split);

	// Factorization constant
	float hist_incr = 100.0f / static_cast<float> (indices.size() * (indices.size() - 1) / 2);

	bool key_found = false;

	//	omp_set_num_threads(omp_get_num_procs());  // 四线程并行计算
	//#pragma omp parallel for
		// Iterate over all the points in the neighborhood
	for (int i_idx = 0; i_idx < indices.size(); ++i_idx)
	{
		int f_index_[4];
		Eigen::Vector4f pfh_tuple_;
		/** \brief Internal hashmap, used to optimize efficiency of redundant computations. */
		std::map<std::pair<int, int>, Eigen::Vector4f, std::less<std::pair<int, int> >, Eigen::aligned_allocator<std::pair<const std::pair<int, int>, Eigen::Vector4f> > > feature_map_;
		/** \brief Queue of pairs saved, used to constrain memory usage. */
		std::queue<std::pair<int, int> > key_list_;
		int h_index, h_p;
		std::pair<int, int> key;
		pcl::PFHEstimation <pcl::PointXYZRGBA, pcl::Normal, pcl::PFHSignature125> pfh;
		for (size_t j_idx = 0; j_idx < i_idx; ++j_idx)
		{
			// If the 3D points are invalid, don't bother estimating, just continue
			if (!isFinite(cloud.points[indices[i_idx]]) || !isFinite(cloud.points[indices[j_idx]]))
				continue;

			if (use_cache_)
			{
				// In order to create the key, always use the smaller index as the first key pair member
				int p1, p2;
				//      if (indices[i_idx] >= indices[j_idx])
				//      {
				p1 = indices[i_idx];
				p2 = indices[j_idx];
				//      }
				//      else
				//      {
				//        p1 = indices[j_idx];
				//        p2 = indices[i_idx];
				//      }
				key = std::pair<int, int>(p1, p2);

				// Check to see if we already estimated this pair in the global hashmap
				std::map<std::pair<int, int>, Eigen::Vector4f, std::less<std::pair<int, int> >, Eigen::aligned_allocator<std::pair<const std::pair<int, int>, Eigen::Vector4f> > >::iterator fm_it = feature_map_.find(key);
				if (fm_it != feature_map_.end())
				{
					pfh_tuple_ = fm_it->second;
					key_found = true;
				}
				else
				{
					// Compute the pair NNi to NNj
					if (!pfh.computePairFeatures(cloud, normals, indices[i_idx], indices[j_idx],
						pfh_tuple_[0], pfh_tuple_[1], pfh_tuple_[2], pfh_tuple_[3]))
						continue;

					key_found = false;
				}
			}
			else
				if (!pfh.computePairFeatures(cloud, normals, indices[i_idx], indices[j_idx],
					pfh_tuple_[0], pfh_tuple_[1], pfh_tuple_[2], pfh_tuple_[3]))
					continue;

			// Normalize the f1, f2, f3 features and push them in the histogram
			f_index_[0] = static_cast<int> (floor(nr_split * ((pfh_tuple_[0] + M_PI) * d_pi_)));
			if (f_index_[0] < 0)         f_index_[0] = 0;
			if (f_index_[0] >= nr_split) f_index_[0] = nr_split - 1;

			f_index_[1] = static_cast<int> (floor(nr_split * ((pfh_tuple_[1] + 1.0) * 0.5)));
			if (f_index_[1] < 0)         f_index_[1] = 0;
			if (f_index_[1] >= nr_split) f_index_[1] = nr_split - 1;

			f_index_[2] = static_cast<int> (floor(nr_split * ((pfh_tuple_[2] + 1.0) * 0.5)));
			if (f_index_[2] < 0)         f_index_[2] = 0;
			if (f_index_[2] >= nr_split) f_index_[2] = nr_split - 1;

			f_index_[3] = static_cast<int> (floor(nr_split * ((pfh_tuple_[3]) * 10)));
			if (f_index_[3] < 0)         f_index_[3] = 0;
			if (f_index_[3] >= nr_split) f_index_[3] = nr_split - 1;

			// Copy into the histogram
			h_index = 0;
			h_p = 1;
			for (int d = 0; d < 4; ++d)
			{
				h_index += h_p * f_index_[d];
				h_p *= nr_split;
			}
			pfh_histogram[h_index] += hist_incr;

			if (use_cache_ && !key_found)
			{
				// Save the value in the hashmap
				feature_map_[key] = pfh_tuple_;

				// Use a maximum cache so that we don't go overboard on RAM usage
				key_list_.push(key);
				// Check to see if we need to remove an element due to exceeding max_size
				if (key_list_.size() > max_cache_size_)
				{
					// Remove the oldest element.
					feature_map_.erase(key_list_.front());
					key_list_.pop();
				}
			}
		}
	}
}

// 求众数
int findMode(std::vector<int> data) {
	int biggestMode = 1;
	std::vector<int> mode, numbers;
	numbers.push_back(data.at(0));
	mode.push_back(1);
	int count = 0;
	for (int i = 1; i < data.size(); i++) {
		if (data.at(i) == numbers.at(count)) {
			mode.at(count)++;
		}
		else {
			if (biggestMode < mode.at(count)) {
				biggestMode = mode.at(count);
			}
			count++;
			mode.push_back(1);
			numbers.push_back(data.at(i));
		}
	}
	if (biggestMode < mode.at(count)) {
		biggestMode = mode.at(count);
	}
	int res;
	for (int i = 0; i < mode.size(); i++) {
		if (mode.at(i) == biggestMode)
			res =  numbers.at(i);
	}
	return res;
}