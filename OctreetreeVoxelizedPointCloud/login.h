#pragma once


#include "visualization.h"
#include "octree.h"
#include "similarity.h"
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>
#include "direct.h"

void VisualizationClassifiedPointCloud() {
	pcl::PointCloud<PointType>::Ptr mycloud(new pcl::PointCloud<PointType>);
	pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);
	pcl::PointCloud<MyPointType>::Ptr compercloud(new pcl::PointCloud<MyPointType>);
	cout << "请选择：" << endl;
	STDCOUTBLUE;
	cout << "【1】  " << "从文件读取" << endl;
	cout << "【2】  " << "从数据库读取" << endl;
	STDCOUTWHITE;
	int WayNumber;
	cin >> WayNumber;
	if (WayNumber == 1) {
		std::vector<std::string> files;           //保存文件路径信息
		while (!files.empty())           //清空vector
		{
			files.pop_back();
		}
		getAllFiles(WorkPath + "\\ClassifiedData", files);  //获取ClassifiedData文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, d;
		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *mycloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		//pcl::io::savePCDFileBinary(files[FileNumber - 1].c_str(), *cloud);
		// 读取选择后的点云数据
		while (!files.empty())           //清空vector
		{
			files.pop_back();
		}
	}
	else if (WayNumber == 2) {
		cout << "请选择：" << endl;
		STDCOUTBLUE;
		cout << "【1】  " << "案例推理" << endl;
		cout << "【2】  " << "随机森林" << endl;
		STDCOUTWHITE;
		int algorithm;
		cin >> algorithm;
		if (algorithm == 1)
		{
			PGconn *conn = NULL;
			char gphost[] = "localhost";
			char pgport[] = "5432";
			char dbname[] = "CBA";
			char user[] = "admin";
			char pwd[] = "123456";
			if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
				char sql[200];
				// 获取数据表名
				PGresult *Res_tablename = NULL;
				char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
				Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
				int tuple_num_tablename = PQntuples(Res_tablename);
				cout << "请选择表：" << endl;
				STDCOUTBLUE;
				for (size_t i = 0; i < tuple_num_tablename; i++)
				{
					cout << "【" << i + 1 << "】  " << PQgetvalue(Res_tablename, i, 0) << endl;
				}
				STDCOUTWHITE;
				int tableindex;
				cin >> tableindex;
				std::string tablename = PQgetvalue(Res_tablename, tableindex - 1, 0);
				PQclear(Res_tablename);
				PGresult *Res_All = NULL;
				char sql_all[] = "SELECT * FROM %s ORDER BY id";
				sprintf(sql, sql_all, tablename.c_str());
				Res_All = PQexec(const_cast<PGconn *>(conn), sql);
				int tuple_num_all = PQntuples(Res_All);
				int field_num_all = PQnfields(Res_All);
				int classification_filed_num;
				do {
					cout << "请输入类别字段名称：" << endl;
					std::string field_name;
					cin >> field_name;
					classification_filed_num = PQfnumber(Res_All, field_name.c_str());
				} while (classification_filed_num == -1);

				cloud->resize(tuple_num_all);
				int increment10 = 0.10 * tuple_num_all;
				int countdown = 1;
				int percent10 = 0;
				omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for
				for (int i = 0; i < tuple_num_all; ++i)
				{
					int id = atof(PQgetvalue(Res_All, i, 0));
					cloud->points[id - 1].x = atof(PQgetvalue(Res_All, i, 1));
					cloud->points[id - 1].y = atof(PQgetvalue(Res_All, i, 2));
					cloud->points[id - 1].z = atof(PQgetvalue(Res_All, i, 3));
					cloud->points[id - 1].Roughness = atof(PQgetvalue(Res_All, i, 4));
					cloud->points[id - 1].r = atoi(PQgetvalue(Res_All, i, 5));
					cloud->points[id - 1].g = atoi(PQgetvalue(Res_All, i, 6));
					cloud->points[id - 1].b = atoi(PQgetvalue(Res_All, i, 7));
					cloud->points[id - 1].rgba = atoi(PQgetvalue(Res_All, i, 8));
					cloud->points[id - 1].tendency = atof(PQgetvalue(Res_All, i, 10));
					cloud->points[id - 1].inclination = atof(PQgetvalue(Res_All, i, 11));
					// 直方图属性整理
					std::vector<float> histogramV;
					supersplit(PQgetvalue(Res_All, i, 9), histogramV, ",");
					for (size_t j = 0; j < histogramV.size(); j++)
					{
						cloud->points[id - 1].histogram[j] = histogramV[j];
					}
					cloud->points[id - 1].Classification = atof(PQgetvalue(Res_All, i, classification_filed_num));
					if (--countdown == 0)
					{
						std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
						percent10++;
						countdown = increment10;
						std::cout.flush();
					}
				}
				PQclear(Res_All); // 清理数据库查询结果数据集
				cout << "是否想要保存文件？Y/N" << endl;
				std::string ifsave;
				cin >> ifsave;
				if (ifsave == "Y") {
					cout << "请输入文件名：" << endl;
					std::string filename;
					cin >> filename;
					cout << "正在保存点云数据：" << filename << ".pcd" << endl;
					pcl::io::savePCDFileBinary(WorkPath + "\\ClassifiedData\\" + filename + ".pcd", *cloud);
					STDCOUTGREEN;
					std::cout << "保存点云数据成功！" << std::endl;
					STDCOUTWHITE;
				}
			}
		}
		else if (algorithm == 2) {
			PGconn *conn = NULL;
			char gphost[] = "localhost";
			char pgport[] = "5432";
			char dbname[] = "RF";
			char user[] = "admin";
			char pwd[] = "123456";
			if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
				char sql[200];

				// 获取数据表名
				PGresult *Res_tablename = NULL;
				char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
				Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
				int tuple_num_tablename = PQntuples(Res_tablename);
				cout << "请选择表：" << endl;
				STDCOUTBLUE;
				for (size_t i = 0; i < tuple_num_tablename; i++)
				{
					cout << "【" << i + 1 << "】  " << PQgetvalue(Res_tablename, i, 0) << endl;
				}
				STDCOUTWHITE;
				int tableindex;
				cin >> tableindex;
				std::string tablename = PQgetvalue(Res_tablename, tableindex - 1, 0);
				PQclear(Res_tablename);
				PGresult *Res_All = NULL;
				char sql_all[] = "SELECT * FROM %s ORDER BY id";
				sprintf(sql, sql_all, tablename.c_str());
				Res_All = PQexec(const_cast<PGconn *>(conn), sql);
				int tuple_num_all = PQntuples(Res_All);
				int field_num_all = PQnfields(Res_All);
				int classification_filed_num;
				do {
					cout << "请输入类别字段名称：" << endl;
					std::string field_name;
					cin >> field_name;
					classification_filed_num = PQfnumber(Res_All, field_name.c_str());
				} while (classification_filed_num == -1);
				cloud->resize(tuple_num_all);
				int increment10 = 0.10 * tuple_num_all;
				int countdown = 1;
				int percent10 = 0;
				for (int i = 0; i < tuple_num_all; ++i)
				{
					int id = atof(PQgetvalue(Res_All, i, 0));
					cloud->points[id - 1].x = atof(PQgetvalue(Res_All, i, 1));
					cloud->points[id - 1].y = atof(PQgetvalue(Res_All, i, 2));
					cloud->points[id - 1].z = atof(PQgetvalue(Res_All, i, 3));
					cloud->points[id - 1].Classification = atof(PQgetvalue(Res_All, i, classification_filed_num));
					if (--countdown == 0)
					{
						std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
						percent10++;
						countdown = increment10;
						std::cout.flush();
					}
				}
				PQclear(Res_All); // 清理数据库查询结果数据集
				cout << "是否想要保存文件？Y/N" << endl;
				std::string ifsave;
				cin >> ifsave;
				if (ifsave == "Y") {
					cout << "请输入文件名：" << endl;
					std::string filename;
					cin >> filename;
					cout << "正在保存点云数据：" << filename << ".pcd" << endl;
					pcl::io::savePCDFileBinary(WorkPath + "\\ClassifiedData\\" + filename + ".pcd", *cloud);
					STDCOUTGREEN;
					std::cout << "保存点云数据成功！" << std::endl;
					STDCOUTWHITE;
				}
			}
		}
	}
	else {
		return;
	}
	pcl::RGB RGBArray[10];

	RGBArray[0].r = 227;
	RGBArray[0].g = 23;
	RGBArray[0].b = 13;

	RGBArray[1].r = 34;
	RGBArray[1].g = 139;
	RGBArray[1].b = 34;

	RGBArray[2].r = 30;
	RGBArray[2].g = 144;
	RGBArray[2].b = 255;

	RGBArray[3].r = 160;
	RGBArray[3].g = 32;
	RGBArray[3].b = 240;

	RGBArray[4].r = 255;
	RGBArray[4].g = 255;
	RGBArray[4].b = 0;

	RGBArray[5].r = 128;
	RGBArray[5].g = 42;
	RGBArray[5].b = 42;

	RGBArray[6].r = 255;
	RGBArray[6].g = 97;
	RGBArray[6].b = 0;

	RGBArray[7].r = 127;
	RGBArray[7].g = 255;
	RGBArray[7].b = 0;

	RGBArray[8].r = 0;
	RGBArray[8].g = 255;
	RGBArray[8].b = 255;

	RGBArray[9].r = 112;
	RGBArray[9].g = 128;
	RGBArray[9].b = 105;
	for (int i = 0; i < mycloud->points.size(); ++i)
	{
		mycloud->points[i].r = RGBArray[mycloud->points[i].Classification].r;
		mycloud->points[i].g = RGBArray[mycloud->points[i].Classification].g;
		mycloud->points[i].b = RGBArray[mycloud->points[i].Classification].b;
	}
	// 读取对比文件
	/*STDCOUTYELLOW;
	cout << "开始读取对比文件" << endl;
	STDCOUTWHITE;
	pcl::io::loadPCDFile(WorkPath + "\\SelectedData\\selected.pcd", *compercloud);
	STDCOUTWHITE;*/
	Visualization<PointType>(mycloud);

}
// case 1
void VisualizationData() {
	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	cout << "请输入文件夹名称：" << endl;
	std::string floderName;
	cin >> floderName;
	// 原始数据
	if (floderName == "RawData") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;

		Visualization<pcl::PointXYZRGBA>(cloud, NULL, false, "no",true);
	}
	// 选择后数据
	if (floderName == "SelectedData" || floderName == "NormalizedClassfication") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;
		pcl::RGB RGBArray[10];

		RGBArray[0].r = 227;
		RGBArray[0].g = 23;
		RGBArray[0].b = 13;

		RGBArray[1].r = 34;
		RGBArray[1].g = 139;
		RGBArray[1].b = 34;

		RGBArray[2].r = 30;
		RGBArray[2].g = 144;
		RGBArray[2].b = 255;

		RGBArray[3].r = 160;
		RGBArray[3].g = 32;
		RGBArray[3].b = 240;

		RGBArray[4].r = 255;
		RGBArray[4].g = 255;
		RGBArray[4].b = 0;

		RGBArray[5].r = 128;
		RGBArray[5].g = 42;
		RGBArray[5].b = 42;

		RGBArray[6].r = 255;
		RGBArray[6].g = 97;
		RGBArray[6].b = 0;

		RGBArray[7].r = 127;
		RGBArray[7].g = 255;
		RGBArray[7].b = 0;

		RGBArray[8].r = 0;
		RGBArray[8].g = 255;
		RGBArray[8].b = 255;

		RGBArray[9].r = 112;
		RGBArray[9].g = 128;
		RGBArray[9].b = 105;
		// 验证直方图属性 和是否为1
		/*double sum = 0;
		for (int j = 0; j < 81; ++j) {
			sum += cloud->points[1].histogram[j];
		}
		if (sum == 1) {
			cout << "直方图属性正确！" << endl;
		}*/
		cout << "是否渲染训练样本的颜色 Y/N" << endl;
		std::string isrendercolor;
		cin >> isrendercolor;
		if (isrendercolor == "Y")
		{
			for (int i = 0; i < cloud->points.size(); ++i)
			{
				if (cloud->points[i].Classification != -1) {
					cloud->points[i].r = RGBArray[cloud->points[i].Classification].r;
					cloud->points[i].g = RGBArray[cloud->points[i].Classification].g;
					cloud->points[i].b = RGBArray[cloud->points[i].Classification].b;
				}
			}
		}
		//pcl::io::savePCDFileBinary(files[FileNumber - 1].c_str(), *cloud);
		Visualization<PointType>(cloud, NULL);
	}
	// 体素化后的数据
	if (floderName == "SimplifiedRawData") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;

		Visualization<PointType>(cloud);
	}
	// 滤波后的数据
	if (floderName == "Filtering") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;
		/*for (size_t i = 0; i < cloud->size(); i++)
		{
			cloud->points[i].ischeckdata = 0;
		}
		pcl::io::savePCDFileBinary(files[FileNumber - 1].c_str(), *cloud);*/
		Visualization<MyPointType>(cloud);
	}
	// 分类后的数据
	if (floderName == "ClassifiedData") {
		VisualizationClassifiedPointCloud();
	}
	// 拣选结果
	if (floderName == "PickingResult") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;
		for (int i = 0; i < cloud->points.size(); ++i)
		{
			cloud->points[i].Classification = 0;
			cloud->points[i].inclination = -1;
			cloud->points[i].tendency = -1;
		}
		Visualization<MyPointType>(cloud);
	}
	if (floderName == "Boundary") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);;

		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;

		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "完成！" << std::endl;
		STDCOUTWHITE;
		// 验证直方图属性 和是否为1
		double sum = 0;
		for (int j = 0; j < 81; ++j) {
			sum += cloud->points[1].histogram[j];
		}
		if (sum == 1) {
			cout << "直方图属性正确！" << endl;
		}
		pcl::RGB RGBArray[10];
		RGBArray[0].r = 215;
		RGBArray[0].g = 31;
		RGBArray[0].b = 49;

		RGBArray[1].r = 33;
		RGBArray[1].g = 150;
		RGBArray[1].b = 243;

		RGBArray[2].r = 245;
		RGBArray[2].g = 251;
		RGBArray[2].b = 0;

		RGBArray[3].r = 0;
		RGBArray[3].g = 150;
		RGBArray[3].b = 136;

		RGBArray[4].r = 156;
		RGBArray[4].g = 39;
		RGBArray[4].b = 176;

		RGBArray[5].r = 251;
		RGBArray[5].g = 142;
		RGBArray[5].b = 0;

		RGBArray[6].r = 54;
		RGBArray[6].g = 200;
		RGBArray[6].b = 83;

		RGBArray[7].r = 0;
		RGBArray[7].g = 98;
		RGBArray[7].b = 255;

		RGBArray[8].r = 255;
		RGBArray[8].g = 217;
		RGBArray[8].b = 106;

		RGBArray[9].r = 130;
		RGBArray[9].g = 155;
		RGBArray[9].b = 158;
		pcl::PointCloud<MyPointType>::Ptr boundary_points_3d(new pcl::PointCloud<MyPointType>);
		for (int i = 0; i < cloud->points.size(); ++i)
		{
			if (cloud->points[i].boundary != 0) {
				boundary_points_3d->push_back(cloud->points[i]);
				/*cloud->points[i].r = 233;
				cloud->points[i].g = 30;
				cloud->points[i].b = 99;*/
			}
		}
		for (size_t i = 0; i < boundary_points_3d->points.size(); i++)
		{
			/*	boundary_points_3d->points[i].r = RGBArray[boundary_points_3d->points[i].boundary - 1].r;
				boundary_points_3d->points[i].g = RGBArray[boundary_points_3d->points[i].boundary - 1].g;
				boundary_points_3d->points[i].b = RGBArray[boundary_points_3d->points[i].boundary - 1].b;*/
			boundary_points_3d->points[i].r = 233;
			boundary_points_3d->points[i].g = 30;
			boundary_points_3d->points[i].b = 99;
		}
		//pcl::io::savePCDFileBinary(files[FileNumber - 1].c_str(), *cloud);
		//Visualization<MyPointType>(cloud, NULL);
		boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("PointCloudShow"));
		viewer->initCameraParameters();   //初始化相机参数
		pcl::visualization::PointCloudColorHandlerRGBField <MyPointType> intensity_distribution(cloud);
		viewer->addPointCloud<MyPointType>(cloud, intensity_distribution, "cloud");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "cloud");
		viewer->setBackgroundColor(0.0, 0.0, 0.0); //设置背景色为黑色
		pcl::visualization::PointCloudColorHandlerRGBField<MyPointType> red(boundary_points_3d);
		viewer->addPointCloud(boundary_points_3d, red, "boundary");
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 3, "boundary");
		while (!viewer->wasStopped())
		{
			//you can also do cool processing here
			//FIXME: Note that this is running in a separate thread from viewerPsycho
			//and you should guard against race conditions yourself...
			viewer->spinOnce(100);
			boost::this_thread::sleep(boost::posix_time::microseconds(100000));
		}
		viewer->close();
	}
}

// case 2
void Caculate() {
	clock_t a, b, c, d, e, f;
	STDCOUTWHITE;
	cout << "初始化Matlab库..." << endl;
	a = clock(); // 程序计时
				 // 初始化程序
	if (!mclInitializeApplication(NULL, 0))
	{
		fprintf(stderr, "Could not initialize the application.n");
		exit(1);
	}
	// 初始化库
	/*Had the same issue with MATLAB 2015b, 2016a, 2017b, Visual Studio 2015 and 2017.
	The program either crashed on "mylibInitialize()" or returned with an empty error message.
	Turned out that the program actually worked outside of Visual Studio, i.e. starting from command line.
	Solved it by setting Debugger Type to Managed Only in Visual Studio (Properties of Project -> Configuration Properties ->
	Debugging -> Debugger Type). 来自：https://ww2.mathworks.cn/matlabcentral/answers/286624-matlab-c-shared-dll-library-initialization-problem*/
	if (!fit_3d_planeInitialize())
	{
		fprintf(stderr, "Could not initialize the library.n");
		exit(1);
	}

	b = clock(); // 计时
	std::cout << "初始化Matlab库用时：" << (double)(b - a) / 1000 << "s" << std::endl;

	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	getAllFiles(WorkPath + "\\RawData", files);  //获取文件夹中的点云文件
	cout << "将读取以下文件：" << endl;
	for (int i = 0; i < files.size(); i++)
	{
		STDCOUTGREEN;
		cout << files[i].c_str() << endl;
	}
	float resolution;
	size_t maxnum;
	STDCOUTWHITE;
	cout << "Please input a resolution of Octree:" << endl;
	std::cin >> resolution;     // 输入八叉树节点半径
	STDCOUTWHITE;
mark:
	cout << "请选择想要生成点云附带属性：" << endl;
	STDCOUTBLUE;
	cout << "【1】.CNN属性（XYZRGB+FileIndex+Classification）" << std::endl;
	cout << "【2】.CBR属性(XYZRGB+PFH..)" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		e = clock();
		for (size_t i = 0; i < files.size(); i++)
		{
			// 计算八叉树及其简化点云
			c = clock();
			CreateOctree(resolution, files[i].c_str(),"CNN");
			d = clock();
			STDCOUTGREEN;
			std::cout << "构建、遍历八叉树用时：" << (double)(d - c) / 1000 << "s" << std::endl;
			STDCOUTWHITE;
		}
		f = clock();
		std::cout << "构建、遍历" << files.size() << "个文件的八叉树总用时：" << (double)(f - e) / 1000 << "s" << std::endl;
		fit_3d_planeTerminate();
		/*cout << "正在保存简化后的原始点云数据..." << endl;
		pcl::io::savePCDFile(WorkPath + "\\SimplifiedRawData\\simplifiedrawdata.pcd", *cloud_xyzi);*/
		STDCOUTGREEN;
		std::cout << "体素化原始点云数据成功！" << std::endl;
		STDCOUTWHITE;
	}; break;
	case 2: {
		e = clock();
		for (size_t i = 0; i < files.size(); i++)
		{
			// 计算八叉树及其简化点云
			c = clock();
			CreateOctree(resolution, files[i].c_str(),"CBR");
			d = clock();
			STDCOUTGREEN;
			std::cout << "构建、遍历八叉树用时：" << (double)(d - c) / 1000 << "s" << std::endl;
			STDCOUTWHITE;
		}
		f = clock();
		std::cout << "构建、遍历" << files.size() << "个文件的八叉树总用时：" << (double)(f - e) / 1000 << "s" << std::endl;
		fit_3d_planeTerminate();
		/*cout << "正在保存简化后的原始点云数据..." << endl;
		pcl::io::savePCDFile(WorkPath + "\\SimplifiedRawData\\simplifiedrawdata.pcd", *cloud_xyzi);*/
		STDCOUTGREEN;
		std::cout << "体素化原始点云数据成功！" << std::endl;
		STDCOUTWHITE;
	}; break;
	default:
		break;
	}
	// 可视化点云数据
	//Visualization<MyPointType>(cloud_xyzi);
}



// case 3
void saveInfoToDB() {
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		std::vector<std::string> files;           //保存文件路径信息
		while (!files.empty())           //清空vector
		{
			files.pop_back();
		}
		getAllFiles(WorkPath + "\\SelectedData", files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成选择后的点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "将尝试读取以下文件：" << endl;
		STDCOUTGREEN;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, f;
		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始写入数据库..." << endl;
		STDCOUTWHITE;
		c = clock();
		size_t length = cloud->size();
		std::string strsql;
		char sql[1000];
		char sqltext[] = "CREATE TABLE public.%s ( \
id integer NOT NULL,\
xvalue double precision,\
yvalue double precision,\
zvalue double precision,\
rvalue integer,\
gvalue integer,\
bvalue integer,\
rgbavalue integer,\
normalx double precision,\
normaly double precision,\
normalz double precision,\
roughness double precision,\
histogram text,\
fileindex integer,\
classification integer,\
ischeckdata integer,\
CONSTRAINT %s_pkey PRIMARY KEY(id));";
		std::string strTime = "table" + GetNowTime();
		sprintf(sql, sqltext, strTime.c_str(), strTime.c_str());
		PQexec(const_cast<PGconn *>(conn), sql); // 创建新的数据表
		STDCOUTGREEN;
		cout << "已创建数据表：" << strTime.c_str() << endl;
		STDCOUTWHITE;
		cout << "请输入验证与测试样本抽取比例(%)：" << endl;
		int selectCheckdataScale;
		cin >> selectCheckdataScale;
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		omp_set_num_threads(4);  // 四线程并行计算
		PGconn *connArray[4];//对应处理器个数，先生成一个储存链接指针的数组
		ConnectToDB(connArray[0], gphost, pgport, dbname, user, pwd);
		ConnectToDB(connArray[1], gphost, pgport, dbname, user, pwd);
		ConnectToDB(connArray[2], gphost, pgport, dbname, user, pwd);
		ConnectToDB(connArray[3], gphost, pgport, dbname, user, pwd);
#pragma omp parallel for
		for (int i = 0; i < length; i++)
		{
			PGresult *Res_insert = NULL;
			char sql_insert[2000];
			int fileindex = cloud->points[i].fileIndex;
			int classification = cloud->points[i].Classification;
			std::string histogram = Array2string(cloud->points[i].histogram,33);

			//产生随机数 按selectCheckdataScale选择验证样本
			int ischeckdata = 0;
			//抽取10%作为验证样本，10%作为测试样本
			if (classification != -1)
			{
				int randvalue = rand() % 100 + 1;
				if (randvalue <= selectCheckdataScale) {
					ischeckdata = 1;
				}
				else if(randvalue>100-selectCheckdataScale) {
					ischeckdata = 2;
				}
			}
			sprintf(sql_insert, "INSERT INTO %s VALUES ('%d','%f','%f','%f','%d','%d','%d','%d','%f','%f','%f','%f','%s','%d','%d','%d');", strTime.c_str(), i + 1, cloud->points[i].x,
				cloud->points[i].y, cloud->points[i].z, cloud->points[i].r, cloud->points[i].g,cloud->points[i].b, cloud->points[i].rgba,cloud->points[i].normal_x, 
				cloud->points[i].normal_y, cloud->points[i].normal_z, cloud->points[i].Roughness, histogram.c_str(),fileindex,classification,ischeckdata);
			int k = omp_get_thread_num();//获得每个线程的ID
			Res_insert = PQexec(const_cast<PGconn *>(connArray[k]), sql_insert);
			int a = PQresultStatus(Res_insert);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_insert) << endl;
			}
			PQclear(Res_insert);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		f = clock();
		STDCOUTGREEN;
		std::cout << "写入数据完毕，用时：" << (double)(f - c) / 1000 << "s" << std::endl;
		STDCOUTWHITE;

	}
	PQfinish(conn);
}


// case 4
void CBA() {
	PGconn *conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		char sql[500];

		// 获取数据表名
		PGresult *Res_tablename = NULL;
		char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
		Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
		int tuple_num_tablename = PQntuples(Res_tablename);
		cout << "请选择表：" << endl;
		STDCOUTBLUE;
		for (size_t i = 0; i < tuple_num_tablename; i++)
		{
			cout << "【" << i + 1 << "】  " << PQgetvalue(Res_tablename, i, 0) << endl;
		}
		STDCOUTWHITE;
		int tableindex;
		cin >> tableindex;
		std::string tablename = PQgetvalue(Res_tablename, tableindex - 1, 0);
		PQclear(Res_tablename);

		// 读取已分类好的案例，写入内存
		PGresult *Res_classification = NULL;
		char sql_c[] = "SELECT roughness,rvalue,gvalue,bvalue,classification,xvalue,yvalue,zvalue,histogram,tendency,inclination FROM %s WHERE classification != 0 AND ischeckdata = 0";
		sprintf(sql, sql_c, tablename.c_str());
		Res_classification = PQexec(const_cast<PGconn *>(conn), sql);
		int tuple_num = PQntuples(Res_classification);
		int field_num = PQnfields(Res_classification);
		std::vector<CEM> ClassSet;
		for (int i = 0; i < tuple_num; ++i)
		{
			CEM item;
			item.Roughness = atof(PQgetvalue(Res_classification, i, 0));
			item.R = atof(PQgetvalue(Res_classification, i, 1));
			item.G = atof(PQgetvalue(Res_classification, i, 2));
			item.B = atof(PQgetvalue(Res_classification, i, 3));
			item.Classification = atof(PQgetvalue(Res_classification, i, 4));
			item.X = atof(PQgetvalue(Res_classification, i, 5));
			item.Y = atof(PQgetvalue(Res_classification, i, 6));
			item.Z = atof(PQgetvalue(Res_classification, i, 7));
			item.tendency = atof(PQgetvalue(Res_classification, i, 9));
			item.inclination = atof(PQgetvalue(Res_classification, i, 10));
			supersplit(PQgetvalue(Res_classification, i, 8), item.histogram, ",");
			Normal(item.histogram);
			// 求取法向量
			item.UnitNormal = Occurrence2UnitNormalVector(atof(PQgetvalue(Res_classification, i, 9)), atof(PQgetvalue(Res_classification, i, 10)));
			ClassSet.push_back(item);
		}
		PQclear(Res_classification); // 清理数据库查询结果数据集
									 // 读取未分类的案例，写入内存
		PGresult *Res_All = NULL;
		char sql_all[] = "SELECT id,roughness,rvalue,gvalue,bvalue,xvalue,yvalue,zvalue,histogram FROM %s";
		sprintf(sql, sql_all, tablename.c_str());
		Res_All = PQexec(const_cast<PGconn *>(conn), sql);
		int tuple_num_all = PQntuples(Res_All);
		int field_num_all = PQnfields(Res_All);
		std::vector<CEM> AllDataSet;
		int increment10_tuple_num_all = 0.10 * tuple_num_all;
		int countdown_tuple_num_all = 1;
		int percent10_tuple_num_all = 0;
		for (int i = 0; i < tuple_num_all; ++i)
		{
			CEM item;
			item.Id = atof(PQgetvalue(Res_All, i, 0));
			item.Roughness = atof(PQgetvalue(Res_All, i, 1));
			item.R = atof(PQgetvalue(Res_All, i, 2));
			item.G = atof(PQgetvalue(Res_All, i, 3));
			item.B = atof(PQgetvalue(Res_All, i, 4));
			item.X = atof(PQgetvalue(Res_All, i, 5));
			item.Y = atof(PQgetvalue(Res_All, i, 6));
			item.Z = atof(PQgetvalue(Res_All, i, 7));
			supersplit(PQgetvalue(Res_All, i, 8), item.histogram, ",");
			Normal(item.histogram);
			AllDataSet.push_back(item);
			if (--countdown_tuple_num_all == 0)
			{
				std::cout << "\r" << std::string(percent10_tuple_num_all, '|') << percent10_tuple_num_all * 10 << "%";
				percent10_tuple_num_all++;
				countdown_tuple_num_all = increment10_tuple_num_all;
				std::cout.flush();
			}
		}
		PQclear(Res_All); // 清理数据库查询结果数据集
		STDCOUTYELLOW;
		cout << "准备案例检索、更新数据库、点云添加类别！" << endl;
		STDCOUTWHITE;

		clock_t a, b, c, d;
		//推理方式
		cout << "请选择推理方式" << endl;
		cout << "【1】集成推理1" << endl;
		cout << "【2】集成推理1" << endl;
		int CBR_Way;
		cin >> CBR_Way;
		//  输入权重
		double WCommonFactor, WDistanceFactor, WHistogramFactor, WSpatial;
		double Wroughness, Wr, Wg, Wb, Wdis;
		if (CBR_Way == 1) {
			cout << "请输入普通属性权重" << endl;
			cin >> WCommonFactor;
			cout << "请输入距离属性权重" << endl;
			cin >> WDistanceFactor;
			cout << "请输入直方图属性权重" << endl;
			cin >> WHistogramFactor;
			cout << "请输入普通属性各子因子权重" << endl;
			cout << "Roughness:" << endl;
			cin >> Wroughness;
			cout << "R:" << endl;
			cin >> Wr;
			cout << "G:" << endl;
			cin >> Wg;
			cout << "B:" << endl;
			cin >> Wb;
		}
		if (CBR_Way == 2) {
			cout << "请输入属性特征权重" << endl;
			cin >> WCommonFactor;
			cout << "请输入空间特征权重" << endl;
			cin >> WSpatial;
			//
			cout << "请输入普通属性各子因子权重" << endl;
			cout << "Roughness:" << endl;
			cin >> Wroughness;
			cout << "R:" << endl;
			cin >> Wr;
			cout << "G:" << endl;
			cin >> Wg;
			cout << "B:" << endl;
			cin >> Wb;
			//
			cout << "请输入空间特征子因子权重" << endl;
			cout << "请输入距离属性权重" << endl;
			cin >> WDistanceFactor;
			cout << "请输入直方图属性权重" << endl;
			cin >> WHistogramFactor;
		}

		// 为数据库添加列：cba_classification、cba_maximumsimilarity
		std::string NowTime = GetNowTime();
		std::string conlumn_classification = "cba_classification_" + NowTime;
		std::string conlumn_maximumsimilarity = "cba_maximumsimilarity_" + NowTime;
		char sql_addcon[] = "ALTER TABLE public.%s ADD COLUMN %s integer;ALTER TABLE public.%s ADD COLUMN %s double precision;";
		sprintf(sql, sql_addcon, tablename.c_str(), conlumn_classification.c_str(), tablename.c_str(), conlumn_maximumsimilarity.c_str());
		PQexec(const_cast<PGconn *>(conn), sql);


		size_t length = ClassSet.size();

		int increment10 = 0.10 * tuple_num_all;
		int countdown = 1;
		int percent10 = 0;

		for (int i = 0; i < tuple_num_all; i++)
		{
			double *simarr = new double[length];
			omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for  
			for (int j = 0; j < length; j++)
			{
				// 普通因子
				double CommonFactor = 0.0;
				if (WCommonFactor > 0.000000000000001) {
					CommonFactor = Similarity(AllDataSet[i].Roughness, ClassSet[j].Roughness,
						AllDataSet[i].R, ClassSet[j].R,
						AllDataSet[i].G, ClassSet[j].G,
						AllDataSet[i].B, ClassSet[j].B,
						Wroughness, Wr, Wg, Wb);
				}
				// 距离因子
				double DistanceFactor = 0.0;
				if (WDistanceFactor > 0.000000000000001)
				{
					SpaceVector AP;
					AP.x = AllDataSet[i].X - ClassSet[j].X;
					AP.y = AllDataSet[i].Y - ClassSet[j].Y;
					AP.z = AllDataSet[i].Z - ClassSet[j].Z;
					double dish, disv;
					SpatialFeatureMetric(AP, ClassSet[j].UnitNormal, dish, disv);
					DistanceFactor = Dis2Occurrence(dish, disv);
				}
				// 直方图因子（场景属性）
				double HistogramFactor = 0.0;
				if (WHistogramFactor > 0.000000000000001) {
					HistogramFactor = modelMatch(ClassSet[j].histogram, AllDataSet[i].histogram);
				}
				// 总相似度
				if (CBR_Way == 1) {
					simarr[j] = (WCommonFactor* CommonFactor + WDistanceFactor * DistanceFactor + WHistogramFactor * HistogramFactor) /
						(WCommonFactor + WDistanceFactor + WHistogramFactor);
				}
				else if (CBR_Way == 2) {
					double SpatialFactor = (WDistanceFactor * DistanceFactor + WHistogramFactor * HistogramFactor) / (WDistanceFactor + WHistogramFactor);
					simarr[j] = (WCommonFactor* CommonFactor + WSpatial * SpatialFactor) /
						(WCommonFactor + WSpatial);
				}
				/*if (simarr[j] > 1) {
					cout << CommonFactor << "," << DistanceFactor << "," << HistogramFactor << endl;
				}*/
			}

			double *MaximumSimilarity = std::max_element(simarr, simarr + length); // 获得最相似的条目的指针
			int MaximumSimilarityIndex = std::distance(simarr, MaximumSimilarity); // 获得最相似的条目的索引
			PGresult *Res_insert = NULL;
			sprintf(sql, "UPDATE %s SET %s = %d,%s=%f,tendency=%f,inclination=%f WHERE id = %d",
				tablename.c_str(), conlumn_classification.c_str(), ClassSet[MaximumSimilarityIndex].Classification,
				conlumn_maximumsimilarity.c_str(), *MaximumSimilarity, ClassSet[MaximumSimilarityIndex].tendency,
				ClassSet[MaximumSimilarityIndex].inclination, AllDataSet[i].Id);
			Res_insert = PQexec(const_cast<PGconn *>(conn), sql);
			PQclear(Res_insert);
			//delete MaximumSimilarity;
			delete[] simarr;
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		STDCOUTGREEN;
		cout << "案例检索完毕！" << endl;
		STDCOUTWHITE;
		// 精度计算
		cout << "预测精度：" << endl;
		PGresult *Res_correct = NULL;
		sprintf(sql, "SELECT COUNT(*) FROM %s WHERE ischeckdata = 1 AND classification = %s",
			tablename.c_str(), conlumn_classification.c_str());
		Res_correct = PQexec(const_cast<PGconn *>(conn), sql);
		float count_correct = atof(PQgetvalue(Res_correct, 0, 0));
		PQclear(Res_correct);
		PGresult *Res_total = NULL;
		sprintf(sql, "SELECT COUNT(*) FROM %s WHERE ischeckdata = 1 ",
			tablename.c_str());
		Res_total = PQexec(const_cast<PGconn *>(conn), sql);
		float count_total = atof(PQgetvalue(Res_total, 0, 0));
		PQclear(Res_total);
		float Precision = count_correct / count_total;
		std::cout << Precision << std::endl;
	}

	PQfinish(conn);
}

//case 5
void CombinePointCloud() {
	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	cout << "请输入文件夹名称：" << endl;
	std::string floderName;
	cin >> floderName;
	STDCOUTWHITE;
mark:
	cout << "请选择想要生成点云附带属性：" << endl;
	STDCOUTBLUE;
	cout << "【1】.CBR属性（XYZRGB+PFH..）" << std::endl;
	cout << "【2】.CNN属性(XYZRGB+FileIndex+Classification)" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		if (floderName == "SimplifiedRawData") {
			getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
			if (files.size() == 0) {
				STDCOUTRED;
				cout << "还没有生成点云文件！" << endl;
				STDCOUTWHITE;
				return;
			}
			cout << "包含以下文件：" << endl;
			STDCOUTBLUE;
			for (int i = 0; i < files.size(); i++)
			{
				cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
			}
			STDCOUTWHITE;
			pcl::PointCloud<MyPointType>::Ptr cloud_c(new pcl::PointCloud<MyPointType>);;
			for (size_t i = 0; i < files.size(); i++)
			{
				pcl::PointCloud<MyPointType> cloud;
				STDCOUTYELLOW;
				cout << "开始读取文件" << files[i].c_str() << "数据..." << endl;
				STDCOUTWHITE;

				pcl::io::loadPCDFile(files[i].c_str(), cloud);

				STDCOUTGREEN;
				std::cout << "读取" << files[i].c_str() << "完成！" << std::endl;
				STDCOUTWHITE;
				*cloud_c += cloud;
				//delete cloud;
			}
			//Visualization<MyPointType>(cloud_c);
			cout << "正在保存合并的点云数据" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\SimplifiedRawData\\combine.pcd", *cloud_c);
			STDCOUTGREEN;
			std::cout << "合并数据成功！" << std::endl;
			STDCOUTWHITE;
	}break;
	case 2: {
		if (floderName == "SimplifiedRawData") {
			getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
			if (files.size() == 0) {
				STDCOUTRED;
				cout << "还没有生成点云文件！" << endl;
				STDCOUTWHITE;
				return;
			}
			cout << "包含以下文件：" << endl;
			STDCOUTBLUE;
			for (int i = 0; i < files.size(); i++)
			{
				cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
			}
			STDCOUTWHITE;
			pcl::PointCloud<PointType>::Ptr cloud_c(new pcl::PointCloud<PointType>);;
			for (size_t i = 0; i < files.size(); i++)
			{
				pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
				STDCOUTYELLOW;
				cout << "开始读取文件" << files[i].c_str() << "数据..." << endl;
				STDCOUTWHITE;

				pcl::io::loadPCDFile(files[i].c_str(), *cloud);

				STDCOUTGREEN;
				std::cout << "读取" << files[i].c_str() << "完成！" << std::endl;
				STDCOUTWHITE;
				*cloud_c += *cloud;
				/*delete cloud;*/
			}
			//Visualization<MyPointType>(cloud_c);
			cout << "正在保存合并的点云数据" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\SimplifiedRawData\\combine.pcd", *cloud_c);
			STDCOUTGREEN;
			std::cout << "合并数据成功！" << std::endl;
			STDCOUTWHITE;
		}break;
	}break;
	default:
		break;
	}
	
	}

}

//case 6
void NormalizedClassification() {
	pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	cout << "请输入文件夹名称：" << endl;
	std::string floderName;
	cin >> floderName;
	if (floderName != "") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, d;
		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;

		cout << "请输入检索球半径：" << endl;
		double radius;
		cin >> radius;
		cout << "请输入半层岩层厚度：" << endl;
		double thickness;
		cin >> thickness;
		int length = cloud->size();
		pcl::search::KdTree<PointType>::Ptr tree(new pcl::search::KdTree<PointType>);
		tree->setInputCloud(cloud);
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for
		for (int i = 0; i < length; i++)
		{
			std::vector<int> indexVector;
			std::vector< float > sqr_distances;
			tree->radiusSearch(cloud->points[i], radius, indexVector, sqr_distances);   // 查找K邻域点
			int indexVectorLength = indexVector.size();
			std::vector<int> ClassificationVector;
			/*SpaceVector normal = Occurrence2UnitNormalVector(cloud->points[i].tendency, cloud->points[i].inclination);*/
			//SpaceVector normal = Occurrence2UnitNormalVector(220.03,3.618);
			SpaceVector res;
			res.x = cloud->points[i].normal_x;
			res.y = cloud->points[i].normal_y;
			res.z = cloud->points[i].normal_z;
			for (size_t j = 0; j < indexVectorLength; j++)
			{
				// 计算线面夹角
				SpaceVector AP;
				AP.x = cloud->points[indexVector[j]].x - cloud->points[i].x;
				AP.y = cloud->points[indexVector[j]].y - cloud->points[i].y;
				AP.z = cloud->points[indexVector[j]].z - cloud->points[i].z;
				//double SinAngle = (abs(normal.x*AP.x + normal.y*AP.y + normal.z*AP.z)) / (Q_rsqrt(AP.x*AP.x + AP.y*AP.y + AP.z*AP.z));
				/*if (SinAngle <= 0.1) {
					ClassificationVector.push_back(cloud->points[indexVector[j]].Classification);
				}*/
				double dish = abs(res.x*AP.x + res.y*AP.y + res.z*AP.z);
				if (dish <= thickness) {
					ClassificationVector.push_back(cloud->points[indexVector[j]].Classification);
				}
			}
			cloud->points[i].Classification = findMode(ClassificationVector);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}

		cout << "是否想要保存文件？Y/N" << endl;
		std::string ifsave;
		cin >> ifsave;
		if (ifsave == "Y") {
			cout << "请输入文件名：" << endl;
			std::string filename;
			cin >> filename;
			cout << "正在保存点云数据：" << filename << ".pcd" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\NormalizedClassfication\\" + filename + ".pcd", *cloud);
			STDCOUTGREEN;
			std::cout << "保存点云数据成功！" << std::endl;
			STDCOUTWHITE;
		}
	}
}

// case 7
void Triangulation() {
	pcl::PointCloud<MyPointType> *mypointtype = new pcl::PointCloud<MyPointType>;
	pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGB>);
	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	getAllFiles(WorkPath + "\\ClassifiedData", files);  //获取ClassifiedData文件夹中的点云文件
	if (files.size() == 0) {
		STDCOUTRED;
		cout << "还没有生成点云文件！" << endl;
		STDCOUTWHITE;
		return;
	}
	cout << "包含以下文件：" << endl;
	STDCOUTBLUE;
	for (int i = 0; i < files.size(); i++)
	{
		cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
	}
	STDCOUTWHITE;
	cout << "请输入想要读取文件的序号：" << endl;
	int FileNumber;
	cin >> FileNumber;
	clock_t a, b, c, d;
	STDCOUTYELLOW;
	cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
	STDCOUTWHITE;
	a = clock();
	pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *mypointtype);
	b = clock();
	STDCOUTGREEN;
	std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
	STDCOUTWHITE;
	// 读取选择后的点云数据
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}

	int filelength = mypointtype->points.size();
	cloud->points.resize(filelength * 1);
	for (size_t i = 0; i < filelength; i++)
	{
		cloud->points[i].x = mypointtype->points[i].x;
		cloud->points[i].y = mypointtype->points[i].y;
		cloud->points[i].z = mypointtype->points[i].z;
		cloud->points[i].r = mypointtype->points[i].r;
		cloud->points[i].g = mypointtype->points[i].g;
		cloud->points[i].b = mypointtype->points[i].b;
		//cloud->points[i].rgba = mypointtype->points[i].rgba;
	}

	delete mypointtype;
	//boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("PointCloudShow"));
	//viewer->initCameraParameters();   //初始化相机参数
	//pcl::visualization::PointCloudColorHandlerRGBField <pcl::PointXYZRGB> intensity_distribution(cloud);
	//viewer->addPointCloud<pcl::PointXYZRGB>(cloud, intensity_distribution, "intensity");
	//viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
	//viewer->setBackgroundColor(0.0, 0.0, 0.0); //设置背景色为黑色


	// Normal estimation（法向量估计）
	pcl::NormalEstimationOMP<pcl::PointXYZRGB, pcl::Normal> n(4);
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	pcl::search::KdTree<pcl::PointXYZRGB>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGB>);
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	n.setKSearch(50);
	n.compute(*normals);  //法线
	//* normals should not contain the point normals + surface curvatures（不能同时包含点的法向量和表面的曲率）


	// Concatenate the XYZ and normal fields （将点云和法线放在一起）
	pcl::PointCloud<pcl::PointXYZRGBNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointXYZRGBNormal>);
	pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);


	// Create search tree*
	//pcl::search::KdTree<pcl::PointNormal>::Ptr tree2(new pcl::search::KdTree<pcl::PointNormal>);
	//tree2->setInputCloud(cloud_with_normals);

	// Initialize objects （初始化对象）
	pcl::GreedyProjectionTriangulation<pcl::PointXYZRGBNormal> gp3;
	pcl::PolygonMesh triangles;

	//设置参数
	gp3.setSearchRadius(10);
	gp3.setMu(2.5);
	gp3.setMaximumNearestNeighbors(200);
	gp3.setMaximumSurfaceAngle(M_PI / 4); // 45 degrees 最大平面角
	gp3.setMinimumAngle(M_PI / 18); // 10 degrees 每个三角的最大角度
	gp3.setMaximumAngle(2 * M_PI / 3); // 120 degrees
	gp3.setNormalConsistency(false);

	// 设置搜索方法和输入点云
	gp3.setInputCloud(cloud_with_normals);
	//gp3.setSearchMethod(tree2);

	//执行重构，结果保存在triangles中
	gp3.reconstruct(triangles);

	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D Viewer"));
	viewer->setBackgroundColor(0, 0, 0);
	viewer->addPolygonMesh(triangles, "my");
	//viewer->addCoordinateSystem(1.0);
	viewer->initCameraParameters();
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}

// case 8
//void VisualizationNormals() {
//	pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);
//	std::vector<std::string> files;           //保存文件路径信息
//	while (!files.empty())           //清空vector
//	{
//		files.pop_back();
//	}
//	getAllFiles(WorkPath + "\\SimplifiedRawData", files);  //获取ClassifiedData文件夹中的点云文件
//	if (files.size() == 0) {
//		STDCOUTRED;
//		cout << "还没有生成点云文件！" << endl;
//		STDCOUTWHITE;
//		return;
//	}
//	cout << "包含以下文件：" << endl;
//	STDCOUTBLUE;
//	for (int i = 0; i < files.size(); i++)
//	{
//		cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
//	}
//	STDCOUTWHITE;
//	cout << "请输入想要读取文件的序号：" << endl;
//	int FileNumber;
//	cin >> FileNumber;
//	clock_t a, b, c, d;
//	STDCOUTYELLOW;
//	cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
//	STDCOUTWHITE;
//	a = clock();
//	pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);
//	b = clock();
//	STDCOUTGREEN;
//	std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
//	STDCOUTWHITE;
//	// 读取选择后的点云数据
//	while (!files.empty())           //清空vector
//	{
//		files.pop_back();
//	}
//
//	//normal visualization
//	pcl::visualization::PCLVisualizer viewer("PCL viewer");
//	viewer.setBackgroundColor(0.0, 0.0, 0.0);
//	pcl::visualization::PointCloudColorHandlerCustom<MyPointType> single_color(cloud, 0, 255, 0);
//
//	//添加需要显示的点云数据
//	viewer.addPointCloud<MyPointType>(cloud, single_color, "sample cloud");
//
//	//设置点显示大小
//	viewer.setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "sample cloud");
//
//	//添加需要显示的点云法向。cloud为原始点云模型，normal为法向信息，10表示需要显示法向的点云间隔，即每10个点显示一次法向，５表示法向长度。
//	viewer.addPointCloudNormals<MyPointType>(cloud, 1, 5, "normals");
//
//	while (!viewer.wasStopped())
//	{
//		viewer.spinOnce();
//	}
//
//	viewer.close();
//}

// case 9
void DistinguishingBoundary() {
	pcl::PointCloud<MyPointType>::Ptr cloud(new pcl::PointCloud<MyPointType>);

	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	cout << "请输入文件夹名称：" << endl;
	std::string floderName;
	cin >> floderName;
	if (floderName != "") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入想要读取文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, d;
		STDCOUTYELLOW;
		cout << "开始读取文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;

		int length = cloud->size();
		pcl::search::KdTree<MyPointType>::Ptr tree(new pcl::search::KdTree<MyPointType>);
		tree->setInputCloud(cloud);
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for
		for (int i = 0; i < length; i++)
		{
			std::vector<int> indexVector;
			std::vector< float > sqr_distances;
			tree->nearestKSearch(cloud->points[i], 5, indexVector, sqr_distances);   // 查找K邻域点
			int indexVectorLength = indexVector.size();
			//for (size_t j = 0; j < indexVectorLength; j++)
			//{
				// 判断类别是否不同
			if (cloud->points[indexVector[0]].Classification != cloud->points[indexVector[4]].Classification) {
				if (cloud->points[indexVector[0]].Classification < cloud->points[indexVector[4]].Classification) {
					cloud->points[i].boundary = cloud->points[indexVector[0]].Classification;
				}
				else {
					cloud->points[i].boundary = cloud->points[indexVector[4]].Classification;
				}
			}
			//}
			//cloud->points[i].Classification = findMode(ClassificationVector);
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}

		cout << "是否想要保存文件？Y/N" << endl;
		std::string ifsave;
		cin >> ifsave;
		if (ifsave == "Y") {
			cout << "请输入文件名：" << endl;
			std::string filename;
			cin >> filename;
			cout << "正在保存点云数据：" << filename << ".pcd" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\Boundary\\" + filename + ".pcd", *cloud);
			STDCOUTGREEN;
			std::cout << "保存点云数据成功！" << std::endl;
			STDCOUTWHITE;
		}
	}
}

// case 10
void GenerateData() {
	clock_t a, b, c, d, e, f,g,h;
	STDCOUTWHITE;
	cout << "正在生成文件夹目录..." << endl;
	_mkdir("C:\\CBR\\CnnData");
	for (int i = 0; i < 9; i++)
	{
		char name[20];
		sprintf(name, "C:\\CBR\\CnnData\\%d", i);
		_mkdir(name);
		char testName[26];
		sprintf(testName, "%s\\test", name);
		_mkdir(testName);
		char trainName[27];
		sprintf(trainName, "%s\\train", name);
		_mkdir(trainName);
		char valName[35];
		sprintf(valName, "%s\\validation", name);
		_mkdir(valName);

	}
	STDCOUTGREEN;
	std::cout << "目录生成完毕！" << std::endl;
	STDCOUTWHITE;
	cout << "开始读取数据库..." << endl;

	PGconn *conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		char sql[200];

		// 获取数据表名
		PGresult *Res_tablename = NULL;
		char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
		Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
		int tuple_num_tablename = PQntuples(Res_tablename);
		cout << "请选择表：" << endl;
		STDCOUTBLUE;
		for (size_t i = 0; i < tuple_num_tablename; i++)
		{
			cout << "【" << i + 1 << "】  " << PQgetvalue(Res_tablename, i, 0) << endl;
		}
		STDCOUTWHITE;
		int tableindex;
		cin >> tableindex;
		std::string tablename = PQgetvalue(Res_tablename, tableindex - 1, 0);
		PQclear(Res_tablename);
		cout << "请输入总类别数：" << endl;
		STDCOUTWHITE;
		int classindex;
		cin >> classindex;
		//体素点云所在文件夹
		char voxelDatafile[] = "C:\\CBR\\VoxelPointData\\%s.pcd";
		/*char voxelDatafile[] = "H:\\VoxelPointData\\%s.pcd";*/
		PGresult *Res_TrainData = NULL;
		PGresult *Res_testData = NULL;
		PGresult *Res_valData = NULL;
		e = clock();
		for (size_t j = 0; j < classindex; j++)
		{
			//train
			char trainSource[50];
			char trainDestination[50];
			//test
			char testSource[50];
			char testDestination[50];
			//val
			char valSource[50];
			char valDestination[50];
			//train,查询数据库中类别为j且不是验证集的数据索引
			char sql_train[] = "SELECT fileindex FROM %s WHERE classification=%d AND ischeckdata=0";
			sprintf(sql, sql_train, tablename.c_str(), j);
			Res_TrainData = PQexec(const_cast<PGconn *>(conn), sql);
			int tuple_num_all = PQntuples(Res_TrainData);
			//进度条
			int increment10 = 0.10 * tuple_num_all;
			int countdown = 1;
			int percent10 = 0;
			STDCOUTWHITE;
			cout << "类别" << j << "训练数据集开始构建.." << endl;
			a = clock();
			for (size_t k = 0; k < tuple_num_all; k++)
			{
				std::string fileIndex =PQgetvalue(Res_TrainData, k, 0);
				//复制前先创建空文件容器
				FILE *fp;
				sprintf(trainDestination,"C:\\CBR\\CnnData\\%d\\train\\%s.pcd",j,fileIndex.c_str());
				fopen_s(&fp, trainDestination, "w+");//w+可读可写
				//复制源文件至数据集文件
				sprintf(trainSource,voxelDatafile,fileIndex.c_str());
				CopyFile(trainSource,trainDestination,false);//false代表覆盖，true不覆盖
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
			cout << "类别"<<j<<"训练数据集构建完毕！"<<"用时"<<(double)(b - a) / 1000 << "s"<<endl;
			STDCOUTWHITE;

			//test查询数据库中类别为j且是测试集的数据索引
			char sql_test[] = "SELECT fileindex FROM %s WHERE classification=%d AND ischeckdata=1";
			sprintf(sql, sql_test,tablename.c_str(), j);
			Res_testData = PQexec(const_cast<PGconn *>(conn), sql);
			int tuple_num = PQntuples(Res_testData);
			//进度条
			int increment = 0.10 * tuple_num;
			int countdown10 = 1;
			int percent = 0;
			cout << "类别" << j << "测试数据集开始构建.." << endl;
			c = clock();
			for (size_t p = 0; p < tuple_num; p++)
			{
				std::string Index =PQgetvalue(Res_testData, p, 0);
				//复制前先创建空文件容器
				FILE *fp;
				sprintf(testDestination, "C:\\CBR\\CnnData\\%d\\test\\%s.pcd",j, Index.c_str());
				fopen_s(&fp, testDestination, "w+");//w+可读可写
				//复制源文件至数据集文件
				sprintf(testSource, voxelDatafile, Index.c_str());
				CopyFile(testSource, testDestination, FALSE);//false代表覆盖，true不覆盖
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
			cout << "类别" << j << "测试数据集构建完毕！"<<"用时" << (double)(d - c) / 1000 << "s" << endl;
			STDCOUTWHITE;

			//val查询数据库中类别为j且是验证集的数据索引
			char sql_val[] = "SELECT fileindex FROM %s WHERE classification=%d AND ischeckdata=2";
			sprintf(sql, sql_val, tablename.c_str(), j);
			Res_valData = PQexec(const_cast<PGconn *>(conn), sql);
			int tuple_num_val = PQntuples(Res_valData);
			//进度条
			int incrementval = 0.10 * tuple_num_val;
			int countdownval10 = 1;
			int percentval = 0;
			cout << "类别" << j << "验证数据集开始构建.." << endl;
			g = clock();
			for (size_t p = 0; p < tuple_num_val; p++)
			{
				std::string Index = PQgetvalue(Res_valData, p, 0);
				//复制前先创建空文件容器
				FILE *fp;
				sprintf(valDestination, "C:\\CBR\\CnnData\\%d\\validation\\%s.pcd", j, Index.c_str());
				fopen_s(&fp, valDestination, "w+");//w+可读可写
				//复制源文件至数据集文件
				sprintf(valSource, voxelDatafile, Index.c_str());
				CopyFile(valSource, valDestination, FALSE);//false代表覆盖，true不覆盖
				if (--countdownval10 == 0)
				{
					std::cout << "\r" << std::string(percentval, '|') << percentval * 10 << "%";
					percentval++;
					countdownval10 = incrementval;
					std::cout.flush();
				}
			}
			h = clock();
			STDCOUTGREEN;
			cout << "类别" << j << "验证数据集构建完毕！" << "用时" << (double)(h - g) / 1000 << "s" << endl;
			STDCOUTWHITE;
		}
		f = clock();
		cout << "总共"<<classindex<<"类数据" << "全部构建完毕！" << "总用时" << (double)(f - e) / 1000 << "s" << endl;
	}
}

void restoreRGB() {
	pcl::PointCloud<PointType>::Ptr restorecloud(new pcl::PointCloud<PointType>);
	pcl::PointCloud<PointType>::Ptr comparecloud(new pcl::PointCloud<PointType>);
	std::vector<std::string> files;           //保存文件路径信息
	while (!files.empty())           //清空vector
	{
		files.pop_back();
	}
	cout << "请输入文件夹名称：" << endl;
	std::string floderName;
	cin >> floderName;
	if (floderName != "") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //获取文件夹中的点云文件
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "还没有生成点云文件！" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "包含以下文件：" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "【" << i + 1 << "】" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "请输入恢复文件的序号：" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, d;
		STDCOUTYELLOW;
		cout << "开始读取恢复文件" << files[FileNumber - 1].c_str() << "数据..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *restorecloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "读取" << files[FileNumber - 1].c_str() << "数据用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始读取参考文件" << "C:\\CBR\\SimplifiedRawData\\combine.pcd" << "数据..." << endl;
		STDCOUTWHITE;
		pcl::io::loadPCDFile("C:\\CBR\\SimplifiedRawData\\combine.pcd", *comparecloud);
		//pcl::io::loadPCDFile("C:\\Users\\Wangbingqian\\Desktop\\CBR\\Filtering\\cliped2.pcd", *comparecloud);
		int length = restorecloud->size();

		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for
		for (int i = 0; i < length; i++)
		{
			restorecloud->points[i].r = comparecloud->points[i].r;
			restorecloud->points[i].g = comparecloud->points[i].g;
			restorecloud->points[i].b = comparecloud->points[i].b;
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}

		cout << "是否想要保存文件？Y/N" << endl;
		std::string ifsave;
		cin >> ifsave;
		if (ifsave == "Y") {
			cout << "请输入文件名：" << endl;
			std::string filename;
			cin >> filename;
			cout << "正在保存点云数据：" << filename << ".pcd" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\Boundary\\" + filename + ".pcd", *restorecloud);
			STDCOUTGREEN;
			std::cout << "保存点云数据成功！" << std::endl;
			STDCOUTWHITE;
		}
	}
}