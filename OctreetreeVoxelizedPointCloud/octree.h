#pragma once

#include "common.h"  

static omp_lock_t lock;//线程锁
//粗糙程度计算函数
double CalculateRoughness(double *xd, double *yd, double *zd, size_t length) {
	double err[1], p[3];
	if (length > 3) {
		mwArray Err(1, 1, mxDOUBLE_CLASS);
		mwArray N(3, 1, mxDOUBLE_CLASS);
		mwArray P(3, 1, mxDOUBLE_CLASS);
		mwArray XD(length, 1, mxSINGLE_CLASS);
		XD.SetData(xd, length);
		mwArray YD(length, 1, mxSINGLE_CLASS);
		YD.SetData(yd, length);
		mwArray ZD(length, 1, mxSINGLE_CLASS);
		ZD.SetData(zd, length);
		char geometry[] = "plane";
		mwArray Geometry(geometry);
		char visualization[] = "off";
		mwArray Visualization(visualization);
		char sod[] = "off";
		mwArray Sod(sod);
		fit_3D_data(1, Err, N, P, XD, YD, ZD, Geometry, Visualization, Sod);
		Err.GetData(err, 1);
	}
	else {
		err[0] = 0;
	}
	return err[0];
}




//八叉树
void CreateOctree(float resolution, std::string PathofPCD, std::string Attribute = "null") {
	clock_t a, b, c;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	//生成原始点云(lujian)存储简化后点云
	pcl::PointCloud<PointType>::Ptr rawCloud(new pcl::PointCloud<PointType>);
	//自定义点云模板
	pcl::PointCloud<MyPointType> ::Ptr cloud_mypoint(new pcl::PointCloud<MyPointType>);
	//自定义点云模板（lujian）存储单个体素点云
	pcl::PointCloud<PointType> ::Ptr SaveInVoxelPoint(new pcl::PointCloud<PointType>);
	STDCOUTYELLOW;
	cout << "开始读取文件" << PathofPCD << "数据..." << endl;
	STDCOUTWHITE;
	a = clock();
	pcl::io::loadPCDFile(PathofPCD, *cloud);
	b = clock();
	//Visualization<pcl::PointXYZRGBA>(cloud);
	STDCOUTGREEN;
	std::cout << "读取" << PathofPCD << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
	STDCOUTWHITE;
	cout << "开始构建八叉树..." << endl;
	pcl::octree::OctreePointCloudSearch <pcl::PointXYZRGBA>::Ptr octree(new pcl::octree::OctreePointCloudSearch <pcl::PointXYZRGBA>(resolution));
	octree->setInputCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr(cloud));
	octree->addPointsFromInputCloud();
	c = clock();
	STDCOUTGREEN;
	std::cout << "构建八叉树用时：" << (double)(c - b) / 1000 << "s" << std::endl;
	STDCOUTWHITE;

	// 查找方法
	pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGBA>);
	// 生成法向量
	cout << "估计表面法线..." << endl;
	pcl::NormalEstimationOMP<pcl::PointXYZRGBA, pcl::Normal> n(4);
	n.setInputCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr(cloud));
	n.setSearchMethod(tree);
	n.setKSearch(25); // 法向量生成半径3cm
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	n.compute(*normals);
	STDCOUTGREEN;
	cout << "估计表面法线完毕！" << std::endl;
	STDCOUTWHITE;
	//cout << "估计快速点特征直方图FPFH..." << endl;
	//// 计算FPFH
	////创建FPFH估计对象fpfh,并把输入数据集cloud和法线normals传递给它
	//pcl::FPFHEstimationOMP<pcl::PointXYZRGBA, pcl::Normal, pcl::FPFHSignature33> fpfh;
	//fpfh.setInputCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr(cloud));
	//fpfh.setInputNormals(normals);
	//fpfh.setSearchMethod(tree);
	//pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs(new pcl::PointCloud<pcl::FPFHSignature33>());

	//fpfh.setKSearch(25);
	//fpfh.setNumberOfThreads(4);
	//fpfh.compute(*fpfhs);
	//STDCOUTGREEN;
	//cout << "快速点特征直方图FPFH完毕！" << std::endl;
	//STDCOUTWHITE;

	cout << "开始遍历八叉树..." << endl;

	pcl::octree::OctreePointCloudSearch<pcl::PointXYZRGBA>::LeafNodeIterator it_Octree(octree.get());

	std::vector<std::vector<int>> indexVectorVector;  // 储存节点容器的容器

	// 八叉树遍历
	while (*++it_Octree) {
		std::vector<int> indexVector = it_Octree.getLeafContainer().getPointIndicesVector();
		if (indexVector.size() > 1) {
			indexVectorVector.push_back(indexVector);
		}
	}
	int Total = indexVectorVector.size();
	cloud_mypoint->points.resize(Total * 1);
	rawCloud->points.resize(Total * 1);
	int increment10 = 0.10 * Total;
	int countdown = 1;
	int percent10 = 0;
	if (Attribute == "CNN")
	{
//		omp_init_lock(&lock); //初始化互斥锁   
//		omp_set_num_threads(omp_get_num_procs());  // 四线程并行计算
//#pragma omp parallel for
		for (int i = 0; i < Total; i++)
		{
			//omp_set_lock(&lock);   //获得互斥器   
			std::vector<int> indexVector = indexVectorVector[i];
			std::vector< float > sqr_distances; //距离 没有用到
			size_t length = indexVector.size();  //体素内包含的点的数量
			int voxelCentroidPointIndex = indexVector[length / 2];  //中心点索引
			pcl::PointXYZRGBA VoxelCentroidPoint = cloud->points[voxelCentroidPointIndex];
			// 如果点数过少，则采用K邻近法扩大范围
			if (length < 51) {
				octree->nearestKSearch(VoxelCentroidPoint, 50, indexVector, sqr_distances);   // 查找K邻域点
				length = indexVector.size();
			}
			//粗糙度
			double *xd = new double[length];
			double *yd = new double[length];
			double *zd = new double[length];
			for (size_t i = 0; i < length; i++)
			{
				xd[i] = cloud->points[indexVector[i]].x;
				yd[i] = cloud->points[indexVector[i]].y;
				zd[i] = cloud->points[indexVector[i]].z;
			}
			double Roughness = CalculateRoughness(xd, yd, zd, length);
			delete[] xd;
			delete[] yd;
			delete[] zd;
			//设置容器大小
			SaveInVoxelPoint->points.resize(length * 1);
			//// 计算PFH
			Eigen::VectorXf pfh_histogram;
			computePointPFH(*cloud, *normals, indexVector, 3, pfh_histogram);
			
			// 计算体素的平均RGB
			int Sum_r = 0, Sum_b = 0, Sum_g = 0;
			for (size_t j = 0; j < length; j++)
			{
				Sum_r += cloud->points[indexVector[j]].r;
				Sum_g += cloud->points[indexVector[j]].g;
				Sum_b += cloud->points[indexVector[j]].b;
				//取出每个体素内所有点生成小文件点云
				PointType* voxelPoint(new PointType);
				voxelPoint->x = cloud->points[indexVector[j]].x;
				voxelPoint->y = cloud->points[indexVector[j]].y;
				voxelPoint->z = cloud->points[indexVector[j]].z;
				voxelPoint->r = cloud->points[indexVector[j]].r;
				voxelPoint->g = cloud->points[indexVector[j]].g;
				voxelPoint->b = cloud->points[indexVector[j]].b;
				voxelPoint->rgba = cloud->points[indexVector[j]].rgba;
				voxelPoint->normal_x = normals->points[indexVector[j]].normal_x;
				voxelPoint->normal_y = normals->points[indexVector[j]].normal_y;
				voxelPoint->normal_z = normals->points[indexVector[j]].normal_z;
				voxelPoint->Roughness = Roughness;
				////赋值fpfh
				//for (size_t k = 0; k < 33; k++)
				//{
				//	voxelPoint->histogram[k] = fpfhs->points[indexVector[j]].histogram[k];
				//}
				//赋值pfh
				for (size_t k = 0; k < pfh_histogram.rows(); k++)
				{
					voxelPoint->histogram[k] = pfh_histogram[k];
				}
				voxelPoint->fileIndex = 0;
				voxelPoint->Classification = -1;
				SaveInVoxelPoint->points[j] = *voxelPoint;
				delete voxelPoint;
			}
			//保存每个体素点云数据（小点云文件命名：体素索引+所属模型编号）
			//数字转字符串
			char fileindex[10];//体素索引
			sprintf(fileindex, "%d", i+1);
			int pos = PathofPCD.find_last_of('_');
			//所属模型编号
			std::string fileName(PathofPCD.substr(pos + 1));
			//文件名
			std::string voxelFileName;
			voxelFileName = fileindex + fileName;
			/*pcl::io::savePCDFileBinary(WorkPath + "\\VoxelPointData\\" + voxelFileName, *SaveInVoxelPoint);*/
			pcl::io::savePCDFileBinary("H:\\VoxelPointData\\" + voxelFileName, *SaveInVoxelPoint);
			//字符串转int
			int posIndex = voxelFileName.find_last_of('.');
			std::string intfileName = voxelFileName.substr(0,posIndex);
			std::stringstream iss(intfileName);
			int num;
			iss >> num;
			//简化后点云
			PointType* voxelToPoint(new PointType);
			voxelToPoint->x = VoxelCentroidPoint.x;
			voxelToPoint->y = VoxelCentroidPoint.y;
			voxelToPoint->z = VoxelCentroidPoint.z;
			voxelToPoint->r = Sum_r / length;
			voxelToPoint->g = Sum_g / length;
			voxelToPoint->b = Sum_b / length;
			voxelToPoint->rgba = VoxelCentroidPoint.rgba;
			voxelToPoint->normal_x = 0;
			voxelToPoint->normal_y = 0;
			voxelToPoint->normal_z = 0;
			voxelToPoint->Roughness = Roughness;
			for (size_t k = 0; k < 81; k++)
			{
				voxelToPoint->histogram[k] = 0;
			}
			voxelToPoint->fileIndex = num;
			voxelToPoint->Classification = -1;
			rawCloud->points[i] = *voxelToPoint;
			delete voxelToPoint;
			if (--countdown == 0)
				{
					std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
					percent10++;
					countdown = increment10;
					std::cout.flush();
				}
			//omp_unset_lock(&lock); //释放互斥器
			}
		//omp_destroy_lock(&lock);  //销毁互斥器   
		//写入文件
		int pos = PathofPCD.find_last_of('\\');
		std::string fileName(PathofPCD.substr(pos + 1));
		cout << "正在保存简化后的原始点云数据---" << PathofPCD << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\SimplifiedRawData\\simplifiedrawdata_" + fileName, *rawCloud);
	}
	else
	{
	omp_set_num_threads(omp_get_num_procs());  // 四线程并行计算
#pragma omp parallel for
		for (int i = 0; i < Total; i++)
		{
			std::vector<int> indexVector = indexVectorVector[i];
			std::vector< float > sqr_distances; //距离 没有用到
			size_t length = indexVector.size();  //体素内包含的点的数量
			int voxelCentroidPointIndex = indexVector[length / 2];  //中心点索引
			pcl::PointXYZRGBA VoxelCentroidPoint = cloud->points[voxelCentroidPointIndex];
			// 如果点数过少，则采用K邻近法扩大范围
			if (length < 51) {
				octree->nearestKSearch(VoxelCentroidPoint, 50, indexVector, sqr_distances);   // 查找K邻域点
				length = indexVector.size();
			}
			// 邻域半径内的所有点，依此计算粗糙程度
			//std::vector< int >   k_indices;
			//std::vector< float >   k_sqr_distances;
			//tree->nearestKSearch(VoxelCentroidPoint, 50, k_indices, k_sqr_distances);   // FPFH与粗糙程度的生成半径
			//for (size_t i = 0; i < k_sqr_distances.size(); i++)
			//{
			//	k_sqr_distances[i] = pow(k_sqr_distances[i], 0.5);
			//}
			//size_t k_length = k_indices.size();
			double *xd = new double[length];
			double *yd = new double[length];
			double *zd = new double[length];
			for (size_t i = 0; i < length; i++)
			{
				xd[i] = cloud->points[indexVector[i]].x;
				yd[i] = cloud->points[indexVector[i]].y;
				zd[i] = cloud->points[indexVector[i]].z;
			}
			double Roughness = CalculateRoughness(xd, yd, zd, length);
			delete[] xd;
			delete[] yd;
			delete[] zd;
			// 计算体素的平均RGB
			int Sum_r = 0, Sum_b = 0, Sum_g = 0;
			for (size_t j = 0; j < length; j++)
			{
				Sum_r += cloud->points[indexVector[j]].r;
				Sum_g += cloud->points[indexVector[j]].g;
				Sum_b += cloud->points[indexVector[j]].b;
			}
			
			// 计算PFH
			Eigen::VectorXf pfh_histogram;
			computePointPFH(*cloud, *normals, indexVector, 3, pfh_histogram);
			MyPointType * point(new MyPointType);
			point->x = VoxelCentroidPoint.x;
			point->y = VoxelCentroidPoint.y;
			point->z = VoxelCentroidPoint.z;
			point->r = Sum_r / length;
		    point->g = Sum_g / length;
			point->b = Sum_b / length;
			point->rgba = VoxelCentroidPoint.rgba;
			point->Roughness = Roughness;
			point->Classification = 0;
			for (size_t k = 0; k < pfh_histogram.rows(); k++)
			{
				point->histogram[k] = pfh_histogram[k];
			}
			point->inclination = -1;
			point->tendency = -1;
			cloud_mypoint->points[i] = *point;
			delete point;
			if (--countdown == 0)
			{
			    std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
			    percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		//写入文件
		int pos = PathofPCD.find_last_of('\\');
		std::string fileName(PathofPCD.substr(pos + 1));
		cout << "正在保存简化后的原始点云数据---" << PathofPCD << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\SimplifiedRawData\\simplifiedrawdata_" + fileName, *cloud_mypoint);
	}
}

//网格体素
void CreateVoxelGrid(std::string PathofPCD) {
	clock_t a, b, c;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);
	//生成原始点云
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud_filtered(new pcl::PointCloud<pcl::PointXYZRGBA>);
	STDCOUTYELLOW;
	cout << "开始读取文件" << PathofPCD << "数据..." << endl;
	STDCOUTWHITE;
	a = clock();
	pcl::io::loadPCDFile(PathofPCD, *cloud);
	b = clock();
	//Visualization<pcl::PointXYZRGBA>(cloud);
	STDCOUTGREEN;
	std::cout << "读取" << PathofPCD << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
	STDCOUTWHITE;
	cout << "开始压缩点云..." << endl;
	pcl::VoxelGrid<pcl::PointXYZRGBA> sor;//创建滤波对象
	sor.setInputCloud(cloud);//设置需要过滤的点云
	sor.setLeafSize(0.03f, 0.03f, 0.03f);//体素体积为3cm³的立方体。
	sor.filter(*cloud_filtered);
	c = clock();
	STDCOUTGREEN;
	std::cout << "压缩点云用时：" << (double)(c - b) / 1000 << "s" << std::endl;
	STDCOUTWHITE;
	int pos = PathofPCD.find_last_of('\\');
	std::string fileName(PathofPCD.substr(pos + 1));
	cout << "正在保存简化后的原始点云数据---" << PathofPCD << endl;
	pcl::io::savePCDFileBinary(WorkPath + "\\SimplifiedRawData\\compressrawdata_" + fileName, *cloud_filtered);

}