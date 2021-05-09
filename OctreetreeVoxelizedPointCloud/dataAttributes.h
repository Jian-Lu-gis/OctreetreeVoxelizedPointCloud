#pragma once
//#include "points.h"
#include "newpoints.h"
#include "common.h"
#include "direct.h"

//floatתstring
string Convert(float Num)
{
	std::stringstream oss;
	oss << Num;
	string str(oss.str());
	return str;
}

////RGB����
//void RgbAttributes()                         
//{
//	clock_t a, b, c, d, e, f, g, h;
//	cout << "������ת������ļ��и�����" << endl;
//	STDCOUTWHITE;
//	int classindex;
//	cin >> classindex;
//	//train
//	char trainSource[100];
//	char trainTransform[100];
//	char trainPoints[80];
//	//test
//	char testSource[100];
//	char testTransform[100];
//	char testPoints[80];
//	//validation
//	char valSource[100];
//	char valTransform[100];
//	char valPoints[80];
//	//pcd���������ļ���
//	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
//	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
//	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
//	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
//	e = clock();
//	for (size_t i = 0; i < classindex; i++)
//	{
//		while (!trainfiles.empty())//���vector
//		{
//			trainfiles.pop_back();
//		}
//		while (!testfiles.empty())//���vector
//		{
//			testfiles.pop_back();
//		}
//		while (!valfiles.empty())//���vector
//		{
//			valfiles.pop_back();
//		}
//		//train�ļ���
//		sprintf(trainSource, trainDatafile, i);
//		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment10 = 0.10 * trainfiles.size();
//		int countdown = 1;
//		int percent10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
//		a = clock();
//		for (int j = 0; j < trainfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = trainfiles[j].find_last_of("\\");
//			int pos1 = trainfiles[j].find(".");
//			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_train;
//			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
//			for (size_t p = 0; p < Cloud->size(); p++)
//			{
//				points.push_back(Cloud->points[p].x);
//				points.push_back(Cloud->points[p].y);
//				points.push_back(Cloud->points[p].z);
//				//todo ����6λ�ַ���
//				normals.push_back(0);
//				normals.push_back(0);
//				normals.push_back(1);
//				features.push_back(Cloud->points[p].r);
//				features.push_back(Cloud->points[p].g);
//				features.push_back(Cloud->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
//			//����ת���ļ���
//			_mkdir(trainTransform);
//
//			//����.points�ļ�
//			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
//			point_cloud_train.set_points(points, normals, features, labels);
//			point_cloud_train.write_points(trainPoints);
//			if (--countdown == 0)
//			{
//				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
//				percent10++;
//				countdown = increment10;
//				std::cout.flush();
//			}
//		}
//		b = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//test�ļ���
//		sprintf(testSource, testDatafile, i);
//		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment = 0.10 * testfiles.size();
//		int countdown10 = 1;
//		int percent = 0;
//		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
//		c = clock();
//		for (int k = 0; k < testfiles.size(); k++)
//		{
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_test;
//			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
//			for (size_t p = 0; p < cloud->size(); p++)
//			{
//				points.push_back(cloud->points[p].x);
//				points.push_back(cloud->points[p].y);
//				points.push_back(cloud->points[p].z);
//				normals.push_back(0);
//				normals.push_back(0);
//				normals.push_back(1);
//				features.push_back(cloud->points[p].r);
//				features.push_back(cloud->points[p].g);
//				features.push_back(cloud->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
//			//����ת���ļ���
//			_mkdir(testTransform);
//			//��ȡpcd�ļ���
//			int pos = testfiles[k].find_last_of("\\");
//			int pos1 = testfiles[k].find(".");
//			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
//			//����.points�ļ�
//			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
//			point_cloud_test.set_points(points, normals, features, labels);
//			point_cloud_test.write_points(testPoints);
//			if (--countdown10 == 0)
//			{
//				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
//				percent++;
//				countdown10 = increment;
//				std::cout.flush();
//			}
//		}
//		d = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//validation�ļ���
//		sprintf(valSource, valDatafile, i);
//		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int incrementval10 = 0.10 * valfiles.size();
//		int countdownval = 1;
//		int percentval10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
//		g = clock();
//		for (int j = 0; j < valfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = valfiles[j].find_last_of("\\");
//			int pos1 = valfiles[j].find(".");
//			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_val;
//			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
//			for (size_t p = 0; p < Cloud_val->size(); p++)
//			{
//				points.push_back(Cloud_val->points[p].x);
//				points.push_back(Cloud_val->points[p].y);
//				points.push_back(Cloud_val->points[p].z);
//				normals.push_back(0);
//				normals.push_back(0);
//				normals.push_back(1);
//				features.push_back(Cloud_val->points[p].r);
//				features.push_back(Cloud_val->points[p].g);
//				features.push_back(Cloud_val->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
//			//����ת���ļ���
//			_mkdir(valTransform);
//
//			//����.points�ļ�
//			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
//			point_cloud_val.set_points(points, normals, features, labels);
//			point_cloud_val.write_points(valPoints);
//			if (--countdownval == 0)
//			{
//				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
//				percentval10++;
//				countdownval = incrementval10;
//				std::cout.flush();
//			}
//		}
//		h = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//	}
//	f = clock();
//	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
//}
////RGB+Normals
//void RgbAndNormalsAttributes()
//{
//	clock_t a, b, c, d, e, f, g, h;
//	cout << "������ת������ļ��и�����" << endl;
//	STDCOUTWHITE;
//	int classindex;
//	cin >> classindex;
//	//train
//	char trainSource[100];
//	char trainTransform[100];
//	char trainPoints[80];
//	//test
//	char testSource[100];
//	char testTransform[100];
//	char testPoints[80];
//	//validation
//	char valSource[100];
//	char valTransform[100];
//	char valPoints[80];
//	//pcd���������ļ���
//	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
//	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
//	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
//	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
//	e = clock();
//	for (size_t i = 0; i < classindex; i++)
//	{
//		while (!trainfiles.empty())//���vector
//		{
//			trainfiles.pop_back();
//		}
//		while (!testfiles.empty())//���vector
//		{
//			testfiles.pop_back();
//		}
//		while (!valfiles.empty())//���vector
//		{
//			valfiles.pop_back();
//		}
//		//train�ļ���
//		sprintf(trainSource, trainDatafile, i);
//		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment10 = 0.10 * trainfiles.size();
//		int countdown = 1;
//		int percent10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
//		a = clock();
//		for (int j = 0; j < trainfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = trainfiles[j].find_last_of("\\");
//			int pos1 = trainfiles[j].find(".");
//			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_train;
//			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
//			for (size_t p = 0; p < Cloud->size(); p++)
//			{
//				float normal_x = Cloud->points[p].normal_x;
//				float normal_y = Cloud->points[p].normal_y;
//				float normal_z = Cloud->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(Cloud->points[p].x);
//				points.push_back(Cloud->points[p].y);
//				points.push_back(Cloud->points[p].z);
//				//todo ����6λ�ַ���
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				features.push_back(Cloud->points[p].r);
//				features.push_back(Cloud->points[p].g);
//				features.push_back(Cloud->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
//			//����ת���ļ���
//			_mkdir(trainTransform);
//
//			//����.points�ļ�
//			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
//			point_cloud_train.set_points(points, normals, features, labels);
//			point_cloud_train.write_points(trainPoints);
//			if (--countdown == 0)
//			{
//				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
//				percent10++;
//				countdown = increment10;
//				std::cout.flush();
//			}
//		}
//		b = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//test�ļ���
//		sprintf(testSource, testDatafile, i);
//		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment = 0.10 * testfiles.size();
//		int countdown10 = 1;
//		int percent = 0;
//		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
//		c = clock();
//		for (int k = 0; k < testfiles.size(); k++)
//		{
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_test;
//			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
//			for (size_t p = 0; p < cloud->size(); p++)
//			{
//				float normal_x = cloud->points[p].normal_x;
//				float normal_y = cloud->points[p].normal_y;
//				float normal_z = cloud->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(cloud->points[p].x);
//				points.push_back(cloud->points[p].y);
//				points.push_back(cloud->points[p].z);
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				features.push_back(cloud->points[p].r);
//				features.push_back(cloud->points[p].g);
//				features.push_back(cloud->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
//			//����ת���ļ���
//			_mkdir(testTransform);
//			//��ȡpcd�ļ���
//			int pos = testfiles[k].find_last_of("\\");
//			int pos1 = testfiles[k].find(".");
//			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
//			//����.points�ļ�
//			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
//			point_cloud_test.set_points(points, normals, features, labels);
//			point_cloud_test.write_points(testPoints);
//			if (--countdown10 == 0)
//			{
//				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
//				percent++;
//				countdown10 = increment;
//				std::cout.flush();
//			}
//		}
//		d = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//validation�ļ���
//		sprintf(valSource, valDatafile, i);
//		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int incrementval10 = 0.10 * valfiles.size();
//		int countdownval = 1;
//		int percentval10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
//		g = clock();
//		for (int j = 0; j < valfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = valfiles[j].find_last_of("\\");
//			int pos1 = valfiles[j].find(".");
//			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_val;
//			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
//			for (size_t p = 0; p < Cloud_val->size(); p++)
//			{
//				float normal_x = Cloud_val->points[p].normal_x;
//				float normal_y = Cloud_val->points[p].normal_y;
//				float normal_z = Cloud_val->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(Cloud_val->points[p].x);
//				points.push_back(Cloud_val->points[p].y);
//				points.push_back(Cloud_val->points[p].z);
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				features.push_back(Cloud_val->points[p].r);
//				features.push_back(Cloud_val->points[p].g);
//				features.push_back(Cloud_val->points[p].b);
//				labels.push_back(i);
//			}
//			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
//			//����ת���ļ���
//			_mkdir(valTransform);
//
//			//����.points�ļ�
//			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
//			point_cloud_val.set_points(points, normals, features, labels);
//			point_cloud_val.write_points(valPoints);
//			if (--countdownval == 0)
//			{
//				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
//				percentval10++;
//				countdownval = incrementval10;
//				std::cout.flush();
//			}
//		}
//		h = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//	}
//	f = clock();
//	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
//}
////Normals+FPFH
//void NormalsAndFPFHAttributes()
//{
//	clock_t a, b, c, d, e, f, g, h;
//	cout << "������ת������ļ��и�����" << endl;
//	STDCOUTWHITE;
//	int classindex;
//	cin >> classindex;
//	//train
//	char trainSource[100];
//	char trainTransform[100];
//	char trainPoints[80];
//	//test
//	char testSource[100];
//	char testTransform[100];
//	char testPoints[80];
//	//validation
//	char valSource[100];
//	char valTransform[100];
//	char valPoints[80];
//	//pcd���������ļ���
//	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
//	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
//	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
//	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
//	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
//	e = clock();
//	for (size_t i = 0; i < classindex; i++)
//	{
//		while (!trainfiles.empty())//���vector
//		{
//			trainfiles.pop_back();
//		}
//		while (!testfiles.empty())//���vector
//		{
//			testfiles.pop_back();
//		}
//		while (!valfiles.empty())//���vector
//		{
//			valfiles.pop_back();
//		}
//		//train�ļ���
//		sprintf(trainSource, trainDatafile, i);
//		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment10 = 0.10 * trainfiles.size();
//		int countdown = 1;
//		int percent10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
//		a = clock();
//		for (int j = 0; j < trainfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = trainfiles[j].find_last_of("\\");
//			int pos1 = trainfiles[j].find(".");
//			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_train;
//			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
//			for (size_t p = 0; p < Cloud->size(); p++)
//			{
//				float normal_x = Cloud->points[p].normal_x;
//				float normal_y = Cloud->points[p].normal_y;
//				float normal_z = Cloud->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(Cloud->points[p].x);
//				points.push_back(Cloud->points[p].y);
//				points.push_back(Cloud->points[p].z);
//				//todo ����6λ�ַ���
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				for (size_t k = 0; k < 33; k++)
//				{
//					features.push_back(Cloud->points[p].histogram[k]);
//				}
//				labels.push_back(i);
//			}
//			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
//			//����ת���ļ���
//			_mkdir(trainTransform);
//
//			//����.points�ļ�
//			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
//			point_cloud_train.set_points(points, normals, features, labels);
//			point_cloud_train.write_points(trainPoints);
//			if (--countdown == 0)
//			{
//				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
//				percent10++;
//				countdown = increment10;
//				std::cout.flush();
//			}
//		}
//		b = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//test�ļ���
//		sprintf(testSource, testDatafile, i);
//		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int increment = 0.10 * testfiles.size();
//		int countdown10 = 1;
//		int percent = 0;
//		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
//		c = clock();
//		for (int k = 0; k < testfiles.size(); k++)
//		{
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_test;
//			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
//			for (size_t p = 0; p < cloud->size(); p++)
//			{
//				float normal_x = cloud->points[p].normal_x;
//				float normal_y = cloud->points[p].normal_y;
//				float normal_z = cloud->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(cloud->points[p].x);
//				points.push_back(cloud->points[p].y);
//				points.push_back(cloud->points[p].z);
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				for (size_t k = 0; k < 33; k++)
//				{
//					features.push_back(cloud->points[p].histogram[k]);
//				}
//				labels.push_back(i);
//			}
//			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
//			//����ת���ļ���
//			_mkdir(testTransform);
//			//��ȡpcd�ļ���
//			int pos = testfiles[k].find_last_of("\\");
//			int pos1 = testfiles[k].find(".");
//			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
//			//����.points�ļ�
//			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
//			point_cloud_test.set_points(points, normals, features, labels);
//			point_cloud_test.write_points(testPoints);
//			if (--countdown10 == 0)
//			{
//				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
//				percent++;
//				countdown10 = increment;
//				std::cout.flush();
//			}
//		}
//		d = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//		//validation�ļ���
//		sprintf(valSource, valDatafile, i);
//		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
//		//������
//		int incrementval10 = 0.10 * valfiles.size();
//		int countdownval = 1;
//		int percentval10 = 0;
//		STDCOUTWHITE;
//		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
//		g = clock();
//		for (int j = 0; j < valfiles.size(); j++)
//		{
//			//��ȡpcd�ļ���
//			int pos = valfiles[j].find_last_of("\\");
//			int pos1 = valfiles[j].find(".");
//			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
//			vector<float> points, normals, features, labels;//.points�ṹ
//			Points point_cloud_val;
//			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
//			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
//			for (size_t p = 0; p < Cloud_val->size(); p++)
//			{
//				float normal_x = Cloud_val->points[p].normal_x;
//				float normal_y = Cloud_val->points[p].normal_y;
//				float normal_z = Cloud_val->points[p].normal_z;
//				if (Convert(normal_x) == "-nan(ind)") {
//					normal_x = 0;
//				}
//				if (Convert(normal_y) == "-nan(ind)")
//				{
//					normal_y = 0;
//				}
//				if (Convert(normal_z) == "-nan(ind)")
//				{
//					normal_z = 1;
//				}
//				points.push_back(Cloud_val->points[p].x);
//				points.push_back(Cloud_val->points[p].y);
//				points.push_back(Cloud_val->points[p].z);
//				normals.push_back(normal_x);
//				normals.push_back(normal_y);
//				normals.push_back(normal_z);
//				for (size_t k = 0; k < 33; k++)
//				{
//					features.push_back(Cloud_val->points[p].histogram[k]);
//				}
//				labels.push_back(i);
//			}
//			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
//			//����ת���ļ���
//			_mkdir(valTransform);
//
//			//����.points�ļ�
//			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
//			point_cloud_val.set_points(points, normals, features, labels);
//			point_cloud_val.write_points(valPoints);
//			if (--countdownval == 0)
//			{
//				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
//				percentval10++;
//				countdownval = incrementval10;
//				std::cout.flush();
//			}
//		}
//		h = clock();
//		STDCOUTGREEN;
//		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
//		STDCOUTWHITE;
//	}
//	f = clock();
//	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
//}
/*
ת��������������
*/
//normal+FPFH+RGB+Roughness
void AllAttributes()
{
	clock_t a, b, c, d, e, f, g, h;
	cout << "������ת������ļ��и�����" << endl;
	STDCOUTWHITE;
	int classindex;
	cin >> classindex;
	//train
	char trainSource[100];
	char trainTransform[100];
	char trainPoints[80];
	//test
	char testSource[100];
	char testTransform[100];
	char testPoints[80];
	//validation
	char valSource[100];
	char valTransform[100];
	char valPoints[80];
	//pcd���������ļ���
	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
	e = clock();
	for (size_t i = 0; i < classindex; i++)
	{
		while (!trainfiles.empty())//���vector
		{
			trainfiles.pop_back();
		}
		while (!testfiles.empty())//���vector
		{
			testfiles.pop_back();
		}
		while (!valfiles.empty())//���vector
		{
			valfiles.pop_back();
		}
		//train�ļ���
		sprintf(trainSource, trainDatafile, i);
		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment10 = 0.10 * trainfiles.size();
		int countdown = 1;
		int percent10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
		a = clock();
		for (int j = 0; j < trainfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = trainfiles[j].find_last_of("\\");
			int pos1 = trainfiles[j].find(".");
			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features,fpfh,roughness,labels;//.points�ṹ
			Points point_cloud_train;
			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
			for (size_t p = 0; p < Cloud->size(); p++)
			{
				float normal_x = Cloud->points[p].normal_x;
				float normal_y = Cloud->points[p].normal_y;
				float normal_z = Cloud->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(Cloud->points[p].x);
				points.push_back(Cloud->points[p].y);
				points.push_back(Cloud->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(Cloud->points[p].r);
				features.push_back(Cloud->points[p].g);
				features.push_back(Cloud->points[p].b);
				//�ֲڶ�
				roughness.push_back(Cloud->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k <33; k++)
				{
					fpfh.push_back(Cloud->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
			//����ת���ļ���
			_mkdir(trainTransform);

			//����.points�ļ�
			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
			point_cloud_train.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_train.write_points(trainPoints);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		b = clock();
		STDCOUTGREEN;
		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//test�ļ���
		sprintf(testSource, testDatafile, i);
		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment = 0.10 * testfiles.size();
		int countdown10 = 1;
		int percent = 0;
		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
		c = clock();
		for (int k = 0; k < testfiles.size(); k++)
		{
			vector<float> points, normals, features,fpfh,roughness,labels;//.points�ṹ
			Points point_cloud_test;
			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
			for (size_t p = 0; p < cloud->size(); p++)
			{
				float normal_x = cloud->points[p].normal_x;
				float normal_y = cloud->points[p].normal_y;
				float normal_z = cloud->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(cloud->points[p].x);
				points.push_back(cloud->points[p].y);
				points.push_back(cloud->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(cloud->points[p].r);
				features.push_back(cloud->points[p].g);
				features.push_back(cloud->points[p].b);
				//�ֲڶ�
				roughness.push_back(cloud->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(cloud->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
			//����ת���ļ���
			_mkdir(testTransform);
			//��ȡpcd�ļ���
			int pos = testfiles[k].find_last_of("\\");
			int pos1 = testfiles[k].find(".");
			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
			//����.points�ļ�
			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
			point_cloud_test.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_test.write_points(testPoints);
			if (--countdown10 == 0)
			{
				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
				percent++;
				countdown10 = increment;
				std::cout.flush();
			}
		}
		d = clock();
		STDCOUTGREEN;
		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//validation�ļ���
		sprintf(valSource, valDatafile, i);
		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int incrementval10 = 0.10 * valfiles.size();
		int countdownval = 1;
		int percentval10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
		g = clock();
		for (int j = 0; j < valfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = valfiles[j].find_last_of("\\");
			int pos1 = valfiles[j].find(".");
			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features, fpfh,roughness,labels;//.points�ṹ
			Points point_cloud_val;
			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
			for (size_t p = 0; p < Cloud_val->size(); p++)
			{
				float normal_x = Cloud_val->points[p].normal_x;
				float normal_y = Cloud_val->points[p].normal_y;
				float normal_z = Cloud_val->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(Cloud_val->points[p].x);
				points.push_back(Cloud_val->points[p].y);
				points.push_back(Cloud_val->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(Cloud_val->points[p].r);
				features.push_back(Cloud_val->points[p].g);
				features.push_back(Cloud_val->points[p].b);
				//�ֲڶ�
				roughness.push_back(Cloud_val->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(Cloud_val->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
			//����ת���ļ���
			_mkdir(valTransform);

			//����.points�ļ�
			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
			point_cloud_val.set_points(points, normals, features,fpfh,roughness,labels);
			point_cloud_val.write_points(valPoints);
			if (--countdownval == 0)
			{
				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
				percentval10++;
				countdownval = incrementval10;
				std::cout.flush();
			}
		}
		h = clock();
		STDCOUTGREEN;
		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
		STDCOUTWHITE;
	}
	f = clock();
	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
}
//RGB+roughness
void RgbAndRoughness()
{
	clock_t a, b, c, d, e, f, g, h;
	cout << "������ת������ļ��и�����" << endl;
	STDCOUTWHITE;
	int classindex;
	cin >> classindex;
	//train
	char trainSource[100];
	char trainTransform[100];
	char trainPoints[80];
	//test
	char testSource[100];
	char testTransform[100];
	char testPoints[80];
	//validation
	char valSource[100];
	char valTransform[100];
	char valPoints[80];
	//pcd���������ļ���
	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
	e = clock();
	for (size_t i = 0; i < classindex; i++)
	{
		while (!trainfiles.empty())//���vector
		{
			trainfiles.pop_back();
		}
		while (!testfiles.empty())//���vector
		{
			testfiles.pop_back();
		}
		while (!valfiles.empty())//���vector
		{
			valfiles.pop_back();
		}
		//train�ļ���
		sprintf(trainSource, trainDatafile, i);
		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment10 = 0.10 * trainfiles.size();
		int countdown = 1;
		int percent10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
		a = clock();
		for (int j = 0; j < trainfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = trainfiles[j].find_last_of("\\");
			int pos1 = trainfiles[j].find(".");
			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_train;
			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
			for (size_t p = 0; p < Cloud->size(); p++)
			{
				points.push_back(Cloud->points[p].x);
				points.push_back(Cloud->points[p].y);
				points.push_back(Cloud->points[p].z);
				//������
				normals.push_back(0);
				normals.push_back(0);
				normals.push_back(1);
				//rgb
				features.push_back(Cloud->points[p].r);
				features.push_back(Cloud->points[p].g);
				features.push_back(Cloud->points[p].b);
				//�ֲڶ�
				roughness.push_back(Cloud->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(0);
				}
				labels.push_back(i);
			}
			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
			//����ת���ļ���
			_mkdir(trainTransform);

			//����.points�ļ�
			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
			point_cloud_train.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_train.write_points(trainPoints);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		b = clock();
		STDCOUTGREEN;
		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//test�ļ���
		sprintf(testSource, testDatafile, i);
		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment = 0.10 * testfiles.size();
		int countdown10 = 1;
		int percent = 0;
		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
		c = clock();
		for (int k = 0; k < testfiles.size(); k++)
		{
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_test;
			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
			for (size_t p = 0; p < cloud->size(); p++)
			{
				points.push_back(cloud->points[p].x);
				points.push_back(cloud->points[p].y);
				points.push_back(cloud->points[p].z);
				//������
				normals.push_back(0);
				normals.push_back(0);
				normals.push_back(1);
				//rgb
				features.push_back(cloud->points[p].r);
				features.push_back(cloud->points[p].g);
				features.push_back(cloud->points[p].b);
				//�ֲڶ�
				roughness.push_back(cloud->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(0);
				}
				labels.push_back(i);
			}
			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
			//����ת���ļ���
			_mkdir(testTransform);
			//��ȡpcd�ļ���
			int pos = testfiles[k].find_last_of("\\");
			int pos1 = testfiles[k].find(".");
			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
			//����.points�ļ�
			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
			point_cloud_test.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_test.write_points(testPoints);
			if (--countdown10 == 0)
			{
				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
				percent++;
				countdown10 = increment;
				std::cout.flush();
			}
		}
		d = clock();
		STDCOUTGREEN;
		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//validation�ļ���
		sprintf(valSource, valDatafile, i);
		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int incrementval10 = 0.10 * valfiles.size();
		int countdownval = 1;
		int percentval10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
		g = clock();
		for (int j = 0; j < valfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = valfiles[j].find_last_of("\\");
			int pos1 = valfiles[j].find(".");
			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_val;
			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
			for (size_t p = 0; p < Cloud_val->size(); p++)
			{
				points.push_back(Cloud_val->points[p].x);
				points.push_back(Cloud_val->points[p].y);
				points.push_back(Cloud_val->points[p].z);
				//������
				normals.push_back(0);
				normals.push_back(0);
				normals.push_back(1);
				//rgb
				features.push_back(Cloud_val->points[p].r);
				features.push_back(Cloud_val->points[p].g);
				features.push_back(Cloud_val->points[p].b);
				//�ֲڶ�
				roughness.push_back(Cloud_val->points[p].Roughness);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(0);
				}
				labels.push_back(i);
			}
			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
			//����ת���ļ���
			_mkdir(valTransform);

			//����.points�ļ�
			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
			point_cloud_val.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_val.write_points(valPoints);
			if (--countdownval == 0)
			{
				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
				percentval10++;
				countdownval = incrementval10;
				std::cout.flush();
			}
		}
		h = clock();
		STDCOUTGREEN;
		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
		STDCOUTWHITE;
	}
	f = clock();
	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
}
//Normal+FPFH
void NormalAndFPFH()
{
	clock_t a, b, c, d, e, f, g, h;
	cout << "������ת������ļ��и�����" << endl;
	STDCOUTWHITE;
	int classindex;
	cin >> classindex;
	//train
	char trainSource[100];
	char trainTransform[100];
	char trainPoints[80];
	//test
	char testSource[100];
	char testTransform[100];
	char testPoints[80];
	//validation
	char valSource[100];
	char valTransform[100];
	char valPoints[80];
	//pcd���������ļ���
	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
	char valDatafile[] = "C:\\CBR\\CnnData\\%d\\validation";
	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
	e = clock();
	for (size_t i = 0; i < classindex; i++)
	{
		while (!trainfiles.empty())//���vector
		{
			trainfiles.pop_back();
		}
		while (!testfiles.empty())//���vector
		{
			testfiles.pop_back();
		}
		while (!valfiles.empty())//���vector
		{
			valfiles.pop_back();
		}
		//train�ļ���
		sprintf(trainSource, trainDatafile, i);
		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment10 = 0.10 * trainfiles.size();
		int countdown = 1;
		int percent10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "ѵ�����ݼ���ʼת��.." << endl;
		a = clock();
		for (int j = 0; j < trainfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = trainfiles[j].find_last_of("\\");
			int pos1 = trainfiles[j].find(".");
			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_train;
			pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(trainfiles[j].c_str(), *Cloud);
			for (size_t p = 0; p < Cloud->size(); p++)
			{
				float normal_x = Cloud->points[p].normal_x;
				float normal_y = Cloud->points[p].normal_y;
				float normal_z = Cloud->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(Cloud->points[p].x);
				points.push_back(Cloud->points[p].y);
				points.push_back(Cloud->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(0);
				features.push_back(0);
				features.push_back(0);
				//�ֲڶ�
				roughness.push_back(0);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(Cloud->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(trainTransform, "C:\\CBR\\CnnData\\%d\\train_points", i);
			//����ת���ļ���
			_mkdir(trainTransform);

			//����.points�ļ�
			sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\%s.points", i, clipName.c_str());
			point_cloud_train.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_train.write_points(trainPoints);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		b = clock();
		STDCOUTGREEN;
		cout << "���" << i << "ѵ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//test�ļ���
		sprintf(testSource, testDatafile, i);
		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment = 0.10 * testfiles.size();
		int countdown10 = 1;
		int percent = 0;
		cout << "���" << i << "�������ݼ���ʼת��.." << endl;
		c = clock();
		for (int k = 0; k < testfiles.size(); k++)
		{
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_test;
			pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(testfiles[k].c_str(), *cloud);
			for (size_t p = 0; p < cloud->size(); p++)
			{
				float normal_x = cloud->points[p].normal_x;
				float normal_y = cloud->points[p].normal_y;
				float normal_z = cloud->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(cloud->points[p].x);
				points.push_back(cloud->points[p].y);
				points.push_back(cloud->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(0);
				features.push_back(0);
				features.push_back(0);
				//�ֲڶ�
				roughness.push_back(0);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(cloud->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(testTransform, "C:\\CBR\\CnnData\\%d\\test_points", i);
			//����ת���ļ���
			_mkdir(testTransform);
			//��ȡpcd�ļ���
			int pos = testfiles[k].find_last_of("\\");
			int pos1 = testfiles[k].find(".");
			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
			//����.points�ļ�
			sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\%s.points", i, clipName.c_str());
			point_cloud_test.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_test.write_points(testPoints);
			if (--countdown10 == 0)
			{
				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
				percent++;
				countdown10 = increment;
				std::cout.flush();
			}
		}
		d = clock();
		STDCOUTGREEN;
		cout << "���" << i << "�������ݼ�ת����ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//validation�ļ���
		sprintf(valSource, valDatafile, i);
		getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int incrementval10 = 0.10 * valfiles.size();
		int countdownval = 1;
		int percentval10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "��֤���ݼ���ʼת��.." << endl;
		g = clock();
		for (int j = 0; j < valfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = valfiles[j].find_last_of("\\");
			int pos1 = valfiles[j].find(".");
			std::string clipName = valfiles[j].substr(pos + 1, pos1 - pos - 1);
			vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
			Points point_cloud_val;
			pcl::PointCloud<PointType>::Ptr Cloud_val(new pcl::PointCloud<PointType>);
			pcl::io::loadPCDFile(valfiles[j].c_str(), *Cloud_val);
			for (size_t p = 0; p < Cloud_val->size(); p++)
			{
				float normal_x = Cloud_val->points[p].normal_x;
				float normal_y = Cloud_val->points[p].normal_y;
				float normal_z = Cloud_val->points[p].normal_z;
				if (Convert(normal_x) == "-nan(ind)") {
					normal_x = 0;
				}
				if (Convert(normal_y) == "-nan(ind)")
				{
					normal_y = 0;
				}
				if (Convert(normal_z) == "-nan(ind)")
				{
					normal_z = 1;
				}
				points.push_back(Cloud_val->points[p].x);
				points.push_back(Cloud_val->points[p].y);
				points.push_back(Cloud_val->points[p].z);
				//������
				normals.push_back(normal_x);
				normals.push_back(normal_y);
				normals.push_back(normal_z);
				//rgb
				features.push_back(0);
				features.push_back(0);
				features.push_back(0);
				//�ֲڶ�
				roughness.push_back(0);
				//fpfh
				for (size_t k = 0; k < 33; k++)
				{
					fpfh.push_back(Cloud_val->points[p].histogram[k]);
				}
				labels.push_back(i);
			}
			sprintf(valTransform, "C:\\CBR\\CnnData\\%d\\val_points", i);
			//����ת���ļ���
			_mkdir(valTransform);

			//����.points�ļ�
			sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\%s.points", i, clipName.c_str());
			point_cloud_val.set_points(points, normals, features, fpfh, roughness, labels);
			point_cloud_val.write_points(valPoints);
			if (--countdownval == 0)
			{
				std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
				percentval10++;
				countdownval = incrementval10;
				std::cout.flush();
			}
		}
		h = clock();
		STDCOUTGREEN;
		cout << "���" << i << "��֤���ݼ�ת����ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
		STDCOUTWHITE;
	}
	f = clock();
	cout << "�ܹ�" << classindex << "������" << "ȫ��ת����ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
}
//readPoints
void readPoints()
{
	Points pointCloud;
	string filestr = "C:\\CBR\\CnnData\\0\\train_points\\231.points";
	pointCloud.read_points(filestr);
}
/*
ת��ȫ����������
*/
//normal+FPFH+RGB+Roughness
void AllDataAttributes()
{
	clock_t a, b;
	char voxelDatafile[] = "C:\\CBR\\VoxelPointData";
	std::vector<std::string>files;
	getAllFiles(voxelDatafile, files);
	char dataTransform[] = "C:\\CBR\\VoxelPointData_points";
	char dataPoint[50];
	//������
	int increment10 = 0.10 * files.size();
	int countdown = 1;
	int percent10 = 0;
	STDCOUTWHITE;
	cout << "ȫ�����ݼ���ʼת��.." << endl;
	a = clock();
	for (int j = 0; j < files.size(); j++)
	{
		//��ȡpcd�ļ���
		int pos = files[j].find_last_of("\\");
		int pos1 = files[j].find(".");
		std::string clipName = files[j].substr(pos + 1, pos1 - pos - 1);
		vector<float> points, normals, features, fpfh, roughness,labels;//.points�ṹ
		Points point_cloud;
		pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
		pcl::io::loadPCDFile(files[j].c_str(), *Cloud);
		for (size_t p = 0; p < Cloud->size(); p++)
		{
			float normal_x = Cloud->points[p].normal_x;
			float normal_y = Cloud->points[p].normal_y;
			float normal_z = Cloud->points[p].normal_z;
			if (Convert(normal_x) == "-nan(ind)") {
				normal_x = 0;
			}
			if (Convert(normal_y) == "-nan(ind)")
			{
				normal_y = 0;
			}
			if (Convert(normal_z) == "-nan(ind)")
			{
				normal_z = 1;
			}
			points.push_back(Cloud->points[p].x);
			points.push_back(Cloud->points[p].y);
			points.push_back(Cloud->points[p].z);
			//������
			normals.push_back(normal_x);
			normals.push_back(normal_y);
			normals.push_back(normal_z);
			//rgb
			features.push_back(Cloud->points[p].r);
			features.push_back(Cloud->points[p].g);
			features.push_back(Cloud->points[p].b);
			//�ֲڶ�
			roughness.push_back(Cloud->points[p].Roughness);
			//fpfh
			for (size_t k = 0; k < 33; k++)
			{
				fpfh.push_back(Cloud->points[p].histogram[k]);
			}
			labels.push_back(0);
		}
		//����ת���ļ���
		_mkdir(dataTransform);
		//����.points�ļ�
		sprintf(dataPoint, "C:\\CBR\\VoxelPointData_points\\%s.points", clipName.c_str());
		point_cloud.set_points(points, normals, features, fpfh, roughness,labels);
		point_cloud.write_points(dataPoint);
		if (--countdown == 0)
		{
			std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
			percent10++;
			countdown = increment10;
			std::cout.flush();
		}
	}
	b = clock();
	STDCOUTGREEN;
	cout << "ȫ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
	STDCOUTWHITE;
}
//RGB+Roughness
void AllDataRGBAndRoughnessAttributes()
{
	clock_t a, b;
	char voxelDatafile[] = "C:\\CBR\\VoxelPointData";
	std::vector<std::string>files;
	getAllFiles(voxelDatafile, files);
	char dataTransform[] = "C:\\CBR\\VoxelPointData_RR_points";
	char dataPoint[50];
	//������
	int increment10 = 0.10 * files.size();
	int countdown = 1;
	int percent10 = 0;
	STDCOUTWHITE;
	cout << "ȫ�����ݼ���ʼת��.." << endl;
	a = clock();
	for (int j = 0; j < files.size(); j++)
	{
		//��ȡpcd�ļ���
		int pos = files[j].find_last_of("\\");
		int pos1 = files[j].find(".");
		std::string clipName = files[j].substr(pos + 1, pos1 - pos - 1);
		vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
		Points point_cloud;
		pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
		pcl::io::loadPCDFile(files[j].c_str(), *Cloud);
		for (size_t p = 0; p < Cloud->size(); p++)
		{
			points.push_back(Cloud->points[p].x);
			points.push_back(Cloud->points[p].y);
			points.push_back(Cloud->points[p].z);
			//������
			normals.push_back(0);
			normals.push_back(0);
			normals.push_back(1);
			//rgb
			features.push_back(Cloud->points[p].r);
			features.push_back(Cloud->points[p].g);
			features.push_back(Cloud->points[p].b);
			//�ֲڶ�
			roughness.push_back(Cloud->points[p].Roughness);
			//fpfh
			for (size_t k = 0; k < 33; k++)
			{
				fpfh.push_back(0);
			}
			labels.push_back(0);
		}
		//����ת���ļ���
		_mkdir(dataTransform);
		//����.points�ļ�
		sprintf(dataPoint, "C:\\CBR\\VoxelPointData_RR_points\\%s.points", clipName.c_str());
		point_cloud.set_points(points, normals, features, fpfh, roughness, labels);
		point_cloud.write_points(dataPoint);
		if (--countdown == 0)
		{
			std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
			percent10++;
			countdown = increment10;
			std::cout.flush();
		}
	}
	b = clock();
	STDCOUTGREEN;
	cout << "ȫ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
	STDCOUTWHITE;
}
//Normal+FPFH
void AllDataNormalAndFPFHAttributes()
{
	clock_t a, b;
	char voxelDatafile[] = "C:\\CBR\\VoxelPointData";
	std::vector<std::string>files;
	getAllFiles(voxelDatafile, files);
	char dataTransform[] = "C:\\CBR\\VoxelPointData_points";
	char dataPoint[50];
	//������
	int increment10 = 0.10 * files.size();
	int countdown = 1;
	int percent10 = 0;
	STDCOUTWHITE;
	cout << "ȫ�����ݼ���ʼת��.." << endl;
	a = clock();
	for (int j = 0; j < files.size(); j++)
	{
		//��ȡpcd�ļ���
		int pos = files[j].find_last_of("\\");
		int pos1 = files[j].find(".");
		std::string clipName = files[j].substr(pos + 1, pos1 - pos - 1);
		vector<float> points, normals, features, fpfh, roughness, labels;//.points�ṹ
		Points point_cloud;
		pcl::PointCloud<PointType>::Ptr Cloud(new pcl::PointCloud<PointType>);
		pcl::io::loadPCDFile(files[j].c_str(), *Cloud);
		for (size_t p = 0; p < Cloud->size(); p++)
		{
			float normal_x = Cloud->points[p].normal_x;
			float normal_y = Cloud->points[p].normal_y;
			float normal_z = Cloud->points[p].normal_z;
			if (Convert(normal_x) == "-nan(ind)") {
				normal_x = 0;
			}
			if (Convert(normal_y) == "-nan(ind)")
			{
				normal_y = 0;
			}
			if (Convert(normal_z) == "-nan(ind)")
			{
				normal_z = 1;
			}
			points.push_back(Cloud->points[p].x);
			points.push_back(Cloud->points[p].y);
			points.push_back(Cloud->points[p].z);
			//������
			normals.push_back(normal_x);
			normals.push_back(normal_y);
			normals.push_back(normal_z);
			//rgb
			features.push_back(0);
			features.push_back(0);
			features.push_back(0);
			//�ֲڶ�
			roughness.push_back(0);
			//fpfh
			for (size_t k = 0; k < 33; k++)
			{
				fpfh.push_back(Cloud->points[p].histogram[k]);
			}
			labels.push_back(0);
		}
		//����ת���ļ���
		_mkdir(dataTransform);
		//����.points�ļ�
		sprintf(dataPoint, "C:\\CBR\\VoxelPointData_points\\%s.points", clipName.c_str());
		point_cloud.set_points(points, normals, features, fpfh, roughness, labels);
		point_cloud.write_points(dataPoint);
		if (--countdown == 0)
		{
			std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
			percent10++;
			countdown = increment10;
			std::cout.flush();
		}
	}
	b = clock();
	STDCOUTGREEN;
	cout << "ȫ�����ݼ�ת����ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
	STDCOUTWHITE;
}
