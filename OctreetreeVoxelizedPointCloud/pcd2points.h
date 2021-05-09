#pragma once
#include "common.h"
#include "direct.h"
#include "dataAttributes.h"

void TransformPointformat() {
	clock_t a, b, c, d, e, f,g,h;
	STDCOUTWHITE;
mark:
	cout << "请选择想要转换的点云：" << endl;
	STDCOUTBLUE;
	cout << "【1】.样本点云" << std::endl;
	cout << "【2】.全部点云" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		cout << "请选择想要点云属性：" << endl;
		STDCOUTBLUE;
		cout << "【1】.RGB" << std::endl;
		cout << "【2】.RGB+法向量" << std::endl;
		cout << "【3】.法向量+FPFH" << std::endl;
		cout << "【4】.RGB+粗糙度" << std::endl;
		cout << "【5】.RGB+粗糙度+法向量+FPFH" << std::endl;
		STDCOUTWHITE;
		int function_att;
		std::cin >> function_att;
		switch (function_att)
		{
		case 1: {
			/*RgbAttributes();*/
		}break;
		case 2:{
			/*RgbAndNormalsAttributes();*/
		}break;
		case 3: {
			/*NormalsAndFPFHAttributes();*/
			NormalAndFPFH();
		}break;
		case 4: {
			RgbAndRoughness();
		}break;
		case 5: {
			AllAttributes();
		}break;
		default:
			break;
		}
		
	}break;
	case 2: {
		cout << "请选择想要点云属性：" << endl;
		STDCOUTBLUE;
		cout << "【1】.RGB" << std::endl;
		cout << "【2】.RGB+法向量" << std::endl;
		cout << "【3】.法向量+FPFH" << std::endl;
		cout << "【4】.RGB+粗糙度" << std::endl;
		cout << "【5】.RGB+粗糙度+法向量+FPFH" << std::endl;
		STDCOUTWHITE;
		int function_att;
		std::cin >> function_att;
		switch (function_att)
		{
		case 1: {

		}break;
		case 2: {

		}break;
		case 3: {
			AllDataNormalAndFPFHAttributes();
		}break;
		case 4: {
			AllDataRGBAndRoughnessAttributes();
		}break;
		case 5: {
			AllDataAttributes();
		}break;
		default:
			break;
		}
	}break;
	default:
		break;
	}

}
void dataRepair()
{
	//数据集生成过程中会有一些数据为空，需进行修复
	clock_t a, b, c, d, e, f,g,h;
	cout << "请输入转换类别文件夹个数：" << endl;
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
	//pcd点云所在文件夹
	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
	std::vector<std::string> trainfiles;           //保存文件路径信息
	std::vector<std::string> testfiles;           //保存文件路径信息
	e = clock();
	for (size_t i = 0; i < classindex; i++)
	{
		while (!trainfiles.empty())//清空vector
		{
			trainfiles.pop_back();
		}
		while (!testfiles.empty())//清空vector
		{
			testfiles.pop_back();
		}
		//train文件夹
		sprintf(trainSource, trainDatafile, i);
		getAllFiles(trainSource, trainfiles);  //获取文件夹中的点云文件
		//进度条
		int increment10 = 0.10 * trainfiles.size();
		int countdown = 1;
		int percent10 = 0;
		STDCOUTWHITE;
		cout << "类别" << i << "训练数据集开始修复.." << endl;
		a = clock();
		char voxelDatafile1[] = "C:\\CBR\\VoxelPointData\\%s.pcd";
		/*char voxelDatafile1[] = "H:\\VoxelPointData\\%s.pcd";*/
		for (int j = 0; j < trainfiles.size(); j++)
		{
			//截取pcd文件名
			int pos = trainfiles[j].find_last_of("\\");
			int pos1 = trainfiles[j].find(".");
			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
			sprintf(trainSource, voxelDatafile1, clipName.c_str());
			CopyFile(trainSource, trainfiles[j].c_str(), false);//false代表覆盖，true不覆盖
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
		cout << "类别" << i << "训练数据集修复完毕！" << "用时" << (double)(b - a) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//test文件夹
		sprintf(testSource, testDatafile, i);
		getAllFiles(testSource, testfiles);  //获取文件夹中的点云文件
		//进度条
		int increment = 0.10 * testfiles.size();
		int countdown10 = 1;
		int percent = 0;
		cout << "类别" << i << "测试数据集开始修复.." << endl;
		c = clock();
		for (int k = 0; k < testfiles.size(); k++)
		{
			//截取pcd文件名
			int pos = testfiles[k].find_last_of("\\");
			int pos1 = testfiles[k].find(".");
			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
			sprintf(testSource, voxelDatafile1, clipName.c_str());
			CopyFile(testSource, testfiles[k].c_str(), false);//false代表覆盖，true不覆盖
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
		cout << "类别" << i << "测试数据集修复完毕！" << "用时" << (double)(d - c) / 1000 << "s" << endl;
		STDCOUTWHITE;
	}
	f = clock();
	cout << "总共" << classindex << "类数据" << "全部修复完毕！" << "总用时" << (double)(f - e) / 1000 << "s" << endl;
}
void buildTXT() {
	//由.point文件转换.octree文件，以及.octree文件到lmdb需要构建filelist.txt文件，提供输入。
	clock_t a, b, c, d, e, f,g,h;
	STDCOUTWHITE;
mark:
	cout << "请选择想要进行的工作：" << endl;
	STDCOUTBLUE;
	cout << "【1】.构建.points文件目录TXT" << std::endl;
	cout << "【2】.构建.octree文件目录TXT" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		cout << "请选择：" << endl;
		STDCOUTBLUE;
		cout << "【1】.样本数据" << std::endl;
		cout << "【2】.全部数据" << std::endl;
		STDCOUTWHITE;
		int function_num;
		std::cin >> function_num;
		switch (function_num)
		{
		case 1: {
			int classindex = 9;
			//train
			char trainSource[50];
			char trainPoints[80];
			//test
			char testSource[50];
			char testPoints[80];
			//validation
			char valSource[50];
			char valPoints[80];
			//points点云所在文件夹
			char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train_points";
			char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test_points";
			char valDatafile[] = "C:\\CBR\\CnnData\\%d\\val_points";
			std::vector<std::string> trainfiles;           //保存文件路径信息
			std::vector<std::string> testfiles;           //保存文件路径信息
			std::vector<std::string> valfiles;           //保存文件路径信息
			e = clock();
			for (size_t i = 0; i < classindex; i++)
			{
				while (!trainfiles.empty())//清空vector
				{
					trainfiles.pop_back();
				}
				while (!testfiles.empty())//清空vector
				{
					testfiles.pop_back();
				}
				while (!valfiles.empty())//清空vector
				{
					valfiles.pop_back();
				}
				//train文件夹
				sprintf(trainSource, trainDatafile, i);
				getAllFiles(trainSource, trainfiles);  //获取文件夹中的点云文件
				//进度条
				int increment10 = 0.10 * trainfiles.size();
				int countdown = 1;
				int percent10 = 0;
				STDCOUTWHITE;
				cout << "类别" << i << "训练points数据文件路径信息开始写入TXT.." << endl;
				a = clock();
				sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\filelist.txt", i);
				ofstream in;
				in.open(trainPoints, ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
				for (int j = 0; j < trainfiles.size(); j++)
				{
					in << trainfiles[j].c_str() << "\n";
					if (--countdown == 0)
					{
						std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
						percent10++;
						countdown = increment10;
						std::cout.flush();
					}
				}
				in.close();//关闭文件
				b = clock();
				STDCOUTGREEN;
				cout << "类别" << i << "训练points数据文件路径信息写入TXT完毕！" << "用时" << (double)(b - a) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//test文件夹
				sprintf(testSource, testDatafile, i);
				getAllFiles(testSource, testfiles);  //获取文件夹中的点云文件
				//进度条
				int increment = 0.10 * testfiles.size();
				int countdown10 = 1;
				int percent = 0;
				cout << "类别" << i << "测试points数据文件路径信息开始写入TXT.." << endl;
				c = clock();
				sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\filelist.txt", i);
				ofstream on;
				on.open(testPoints, ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
				for (int k = 0; k < testfiles.size(); k++)
				{
					on << testfiles[k].c_str() << "\n";
					if (--countdown10 == 0)
					{
						std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
						percent++;
						countdown10 = increment;
						std::cout.flush();
					}
				}
				on.close();//关闭文件
				d = clock();
				STDCOUTGREEN;
				cout << "类别" << i << "测试points数据文件路径信息写入TXT完毕！" << "用时" << (double)(d - c) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//val文件夹
				sprintf(valSource, valDatafile, i);
				getAllFiles(valSource, valfiles);  //获取文件夹中的点云文件
				//进度条
				int incrementval10 = 0.10 * valfiles.size();
				int countdownval = 1;
				int percentval10 = 0;
				STDCOUTWHITE;
				cout << "类别" << i << "验证points数据文件路径信息开始写入TXT.." << endl;
				g = clock();
				sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\filelist.txt", i);
				ofstream inval;
				inval.open(valPoints, ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
				for (int j = 0; j < valfiles.size(); j++)
				{
					inval << valfiles[j].c_str() << "\n";
					if (--countdownval == 0)
					{
						std::cout << "\r" << std::string(percentval10, '|') << percentval10 * 10 << "%";
						percentval10++;
						countdownval = incrementval10;
						std::cout.flush();
					}
				}
				inval.close();//关闭文件
				h = clock();
				STDCOUTGREEN;
				cout << "类别" << i << "验证points数据文件路径信息写入TXT完毕！" << "用时" << (double)(h - g) / 1000 << "s" << endl;
				STDCOUTWHITE;
			}
			f = clock();
			cout << "总共" << classindex << "类数据" << "全部路径信息写入TXT完毕！" << "总用时" << (double)(f - e) / 1000 << "s" << endl;
		}break;
		case 2: {
			clock_t a, b;
			char voxelDatafile[] = "C:\\CBR\\VoxelPointData_points";
			std::vector<std::string>files;
			char dataTransform[] = "C:\\CBR\\VoxelPointData_points";
			ofstream in;
			in.open("C:\\CBR\\VoxelPointData_points\\filelist.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
			//进度条
			int increment10 = 0.10 * files.size();
			int countdown = 1;
			int percent10 = 0;
			STDCOUTWHITE;
			cout << "全部points数据路径信息开始写入TXT.." << endl;
			getAllFiles(voxelDatafile, files);
			a = clock();
			for (int j = 0; j < files.size(); j++)
			{
				in << files[j].c_str() << "\n";
				if (--countdown == 0)
				{
					std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
					percent10++;
					countdown = increment10;
					std::cout.flush();
				}
			}
			in.close();//关闭文件
			b = clock();
			STDCOUTGREEN;
			cout << "全体points文件路径信息写入TXT完毕！" << "用时" << (double)(b - a) / 1000 << "s" << endl;
			STDCOUTWHITE;
		}break;
		default:
			break;
		}
	}break;
	case 2: {
		cout << "请选择：" << endl;
		STDCOUTBLUE;
		cout << "【1】.样本数据" << std::endl;
		cout << "【2】.全部数据" << std::endl;
		STDCOUTWHITE;
		int function_num;
		std::cin >> function_num;
		switch (function_num)
		{
		case 1: {
			int classindex = 9;
			//train
			char trainSource[50];
			char trainPoints[80];
			//test
			char testSource[50];
			char testPoints[80];
			//val
			char valSource[50];
			char valPoints[80];
			//octree文件所在文件夹
			char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train_octree";
			char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test_octree";
			char valDatafile[] = "C:\\CBR\\CnnData\\%d\\val_octree";
			std::vector<std::string> trainfiles;           //保存文件路径信息
			std::vector<std::string> testfiles;           //保存文件路径信息
			std::vector<std::string> valfiles;           //保存文件路径信息
			e = clock();
			ofstream in;
			in.open("D:\\experiment\\twenty-fifth\\filelist_train.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
			ofstream on;
			on.open("D:\\experiment\\twenty-fifth\\filelist_test.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
			ofstream on_val;
			on_val.open("D:\\experiment\\twenty-fifth\\filelist_val.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
			for (size_t i = 0; i < classindex; i++)
			{
				while (!trainfiles.empty())//清空vector
				{
					trainfiles.pop_back();
				}
				while (!testfiles.empty())//清空vector
				{
					testfiles.pop_back();
				}
				while (!valfiles.empty())//清空vector
				{
					valfiles.pop_back();
				}
				//train文件夹
				sprintf(trainSource, trainDatafile, i);
				getAllFiles(trainSource, trainfiles);  //获取文件夹中的点云文件
				//进度条
				int increment10 = 0.10 * trainfiles.size();
				int countdown = 1;
				int percent10 = 0;
				STDCOUTWHITE;
				cout << "类别" << i << "训练octree数据文件路径信息开始写入TXT.." << endl;
				a = clock();
				for (int j = 0; j < trainfiles.size(); j++)
				{
					string str = trainfiles[j].c_str();
					//此处11是自定义文件夹名称的长度，“C:/CBR/CnnData/1/train”,需截取为“1/train”可据实际情况设置
					string clipStr = str.substr(15, str.length() - 15);
					in << clipStr.c_str() << " " << i << "\n";
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
				cout << "类别" << i << "训练octree数据文件路径信息写入TXT完毕！" << "用时" << (double)(b - a) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//test文件夹
				sprintf(testSource, testDatafile, i);
				getAllFiles(testSource, testfiles);  //获取文件夹中的点云文件
				//进度条
				int increment = 0.10 * testfiles.size();
				int countdown10 = 1;
				int percent = 0;
				cout << "类别" << i << "测试octree数据文件路径信息开始写入TXT.." << endl;
				c = clock();
				for (int k = 0; k < testfiles.size(); k++)
				{
					string str1 = testfiles[k].c_str();
					string clipStr1 = str1.substr(15, str1.length() - 15);
					on << clipStr1.c_str() << " " << i << "\n";
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
				cout << "类别" << i << "测试octree数据文件路径信息写入TXT完毕！" << "用时" << (double)(d - c) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//val文件夹
				sprintf(valSource, valDatafile, i);
				getAllFiles(valSource, valfiles);  //获取文件夹中的点云文件
				//进度条
				int incrementval = 0.10 * valfiles.size();
				int countdownval10 = 1;
				int percentval = 0;
				cout << "类别" << i << "验证octree数据文件路径信息开始写入TXT.." << endl;
				g = clock();
				for (int p = 0; p< valfiles.size(); p++)
				{
					string strval = valfiles[p].c_str();
					string clipStrval = strval.substr(15, strval.length() - 15);
					on_val << clipStrval.c_str() << " " << i << "\n";
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
				cout << "类别" << i << "验证octree数据文件路径信息写入TXT完毕！" << "用时" << (double)(h - g) / 1000 << "s" << endl;
				STDCOUTWHITE;
			}
			in.close();
			on.close();
			on_val.close();
			f = clock();
			cout << "总共" << classindex << "类数据" << "全部路径信息写入TXT完毕！" << "总用时" << (double)(f - e) / 1000 << "s" << endl;
		}break;
		case 2: {
			clock_t a, b;
			char voxelDatafile[] = "H:\\VoxelPointData_octree";
			std::vector<std::string>files;
			ofstream in;
			in.open("D:\\AllDataTxt\\filelist_all.txt", ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
			//进度条
			int increment10 = 0.10 * files.size();
			int countdown = 1;
			int percent10 = 0;
			STDCOUTWHITE;
			cout << "全部octree数据路径信息开始写入TXT.." << endl;
			getAllFiles(voxelDatafile, files);
			a = clock();
			for (int j = 0; j < files.size(); j++)
			{
				string str = files[j].c_str();
				//此处11是自定义文件夹名称的长度，“C:/CBR/VoxelPointData_octree”,需截取为“/VoxelPointData_octree”可据实际情况设置
				string clipStr = str.substr(3, str.length() - 3);
				in << clipStr.c_str() << " " << -1 << "\n";
				if (--countdown == 0)
				{
					std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
					percent10++;
					countdown = increment10;
					std::cout.flush();
				}
			}
			in.close();//关闭文件
			b = clock();
			STDCOUTGREEN;
			cout << "全体octree文件路径信息写入TXT完毕！" << "用时" << (double)(b - a) / 1000 << "s" << endl;
			STDCOUTWHITE;
		}break;
		default:
			break;
		}
	}
	}
}
void labelSaveToDB()
{
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	clock_t a, b, c, d, e, f;
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		STDCOUTYELLOW;
		cout << "开始写入数据库..." << endl;
		STDCOUTWHITE;
		char sql[1000];
		char sqltext[] = "CREATE TABLE public.%s ( \
		id integer NOT NULL,\
		fileindex integer,\
		classification integer,\
		CONSTRAINT %s_pkey PRIMARY KEY(id));";
		std::string strTime = "label" + GetNowTime();
		sprintf(sql, sqltext, strTime.c_str(), strTime.c_str());
		PQexec(const_cast<PGconn *>(conn), sql); // 创建新的数据表
		STDCOUTGREEN;
		cout << "已创建数据表：" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 3211115;//此处读取待预测txt文件数量大，且其数量已知，直接赋值，避免读取耗时。
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//对应处理器个数，先生成一个储存链接指针的数组
		//ConnectToDB(connArray[0], gphost, pgport, dbname, user, pwd);
		char str[80];
		ifstream fin;
		fin.open("D:\\experiment\\twenty-fifth\\filelist_all_rr_shuffle.txt", ios::in);
		int i = 1;
		a = clock();
		while (fin.getline(str, 80))
		{
			PGresult *Res_insert = NULL;
			char sql_insert[1000];
			//待预测文件字符串截取
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//取11783052:VoxelPointData_octree\11783052_5_2_000.octree -1
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//字符串转int
			std::stringstream iss(clipStr);
			int fileindex;//文件索引
			iss >> fileindex;
			sprintf(sql_insert, "INSERT INTO %s VALUES ('%d','%d','%d');", strTime.c_str(), i, fileindex, -1);
			Res_insert = PQexec(const_cast<PGconn *>(conn), sql_insert);
			int a = PQresultStatus(Res_insert);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_insert) << endl;
			}
			PQclear(Res_insert);
			i++;
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		fin.close();
		b = clock();
		STDCOUTGREEN;
		std::cout << "写入数据完毕，用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始读取预测后文件并对数据库进行更新..." << endl;
		STDCOUTWHITE;
		std::vector<std::string>files;
		getAllFiles("D:\\experiment\\twenty-fifth\\result", files);
		int size = files.size() / 2;
		char labelfile[] = "D:\\experiment\\twenty-fifth\\result\\classfy_test%05d_prob";
		int ID = 1;
		c = clock();
		//进度条
		int increment10_label = 0.10 * size;
		int countdown_label = 1;
		int percent10_label = 0;
		for (size_t i = 0; i < size; i++)
		{
			char labelFileIndex[80];
			sprintf(labelFileIndex, labelfile,i);
			char str[10];
			ifstream finLabel;
			finLabel.open(labelFileIndex, ios::in);
			/*读取文本取每九个预测值进行比较，取最大值对应预测标签*/
			int labelindex = 0;//预测标签范围（0-8）
			int label = 0;//预测标签
			float probValue;//预测值
			while (finLabel.getline(str, 10))
			{
				if (labelindex == 0)
				{
					probValue = atof(str);
					label = 0;
				}
				else if (labelindex >= 0 && labelindex <=8)
				{
					if (probValue < atof(str))
					{
						probValue = atof(str);
						label = labelindex;
					}
					if (labelindex == 8)
					{
						labelindex = -1;
						char sql_update[1000];
						PGresult *Res_update = NULL;
						sprintf(sql_update, "UPDATE %s SET classification=%d WHERE id=%d;", strTime.c_str(), label, ID);
						Res_update = PQexec(const_cast<PGconn *>(conn), sql_update);
						int a = PQresultStatus(Res_update);
						if (a != PGRES_COMMAND_OK) {
							cout << PQresultErrorMessage(Res_update) << endl;
						}
						PQclear(Res_update);
						ID++;
					}
				}
				labelindex++;
			}
			finLabel.close();
			if (--countdown_label == 0)
			{
				std::cout << "\r" << std::string(percent10_label, '|') << percent10_label * 10 << "%";
				percent10_label++;
				countdown_label = increment10_label;
				std::cout.flush();
			}
		}
		d = clock();
		STDCOUTGREEN;
		std::cout << "预测标签更新数据库完毕，用时：" << (double)(d - c) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始由上一步预测标签数据库赋值待分类点云类别标签..." << endl;
		STDCOUTWHITE;
		char sql_select[2000];
		// 获取数据表名
		PGresult *Res_tablename = NULL;
		//待分类点云与标签数据联合查询得到分类点云
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
		std::string Tname = PQgetvalue(Res_tablename, tableindex - 1, 0);
		PQclear(Res_tablename);
		PGresult *Res_All = NULL;
		pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
		char sql_all[] = "select %s.id,%s.xvalue,%s.yvalue,%s.zvalue,%s.rvalue,%s.gvalue,%s.bvalue,%s.rgbavalue,%s.fileindex,%s.classification from %s,%s where %s.fileindex=%s.fileindex order by id";
		sprintf(sql_select, sql_all, Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), strTime.c_str(), Tname.c_str(), strTime.c_str(), Tname.c_str(), strTime.c_str());//数据表为上一步更新的预测标签表
		Res_All = PQexec(const_cast<PGconn *>(conn), sql_select);
		int tuple_num_all = PQntuples(Res_All);
		int field_num_all = PQnfields(Res_All);
		//进度条
		int increment = 0.10 * tuple_num_all;
		int countdown10 = 1;
		int percent = 0;
		cloud->resize(tuple_num_all + 5);
		e = clock();
		STDCOUTYELLOW;
		cout << "开始生成待分类点云.." << endl;
		STDCOUTWHITE;
		omp_set_num_threads(4);  // 四线程并行计算
#pragma omp parallel for
		for (int j = 0; j < tuple_num_all; j++)
		{
			int id = atof(PQgetvalue(Res_All, j, 0));
			cloud->points[id - 1].x = atof(PQgetvalue(Res_All, j, 1));
			cloud->points[id - 1].y = atof(PQgetvalue(Res_All, j, 2));
			cloud->points[id - 1].z = atof(PQgetvalue(Res_All, j, 3));
			cloud->points[id - 1].r = atoi(PQgetvalue(Res_All, j, 4));
			cloud->points[id - 1].g = atoi(PQgetvalue(Res_All, j, 5));
			cloud->points[id - 1].b = atoi(PQgetvalue(Res_All, j, 6));
			cloud->points[id - 1].rgba = atoi(PQgetvalue(Res_All, j, 7));
			cloud->points[id - 1].fileIndex = atof(PQgetvalue(Res_All, j, 8));
			cloud->points[id - 1].Classification = atof(PQgetvalue(Res_All, j, 9));
			if (--countdown10 == 0)
			{
				std::cout << "\r" << std::string(percent, '|') << percent * 10 << "%";
				percent++;
				countdown10 = increment;
				std::cout.flush();
			}
		}
		f = clock();
		PQclear(Res_All);
		PQfinish(conn);//关闭链接
		STDCOUTGREEN;
		std::cout << "生成分类点云完毕，用时：" << (double)(f - e) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		std::string filename = "classication" + GetNowTime();
		cout << "正在保存点云数据：" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\ClassifiedData\\" + filename + ".pcd", *cloud);
		STDCOUTGREEN;
		std::cout << "保存点云数据成功！" << std::endl;
		STDCOUTWHITE;
	}
}
void normalVisualization()
{
	std::vector<std::string> files;           //保存文件路径信息
	//加载点云模型
	getAllFiles(WorkPath + "\\" + "RawData", files);  //获取文件夹中的点云文件
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
	cout << "估计表面法线..." << endl;
	//计算法线
	pcl::NormalEstimationOMP<pcl::PointXYZRGBA, pcl::Normal> n(4);
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	//建立kdtree来进行近邻点集搜索
	pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGBA>);
	//为kdtree添加点云数据
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	//点云法向计算时，需要所搜的近邻点大小
	n.setKSearch(25);
	//开始进行法向计算
	n.compute(*normals);
	cout << "估计表面法线完毕！" << std::endl;
	STDCOUTWHITE;
	cout << "估计快速点特征直方图FPFH..." << endl;
	// 计算FPFH
	//创建FPFH估计对象fpfh,并把输入数据集cloud和法线normals传递给它
	pcl::FPFHEstimationOMP<pcl::PointXYZRGBA, pcl::Normal, pcl::FPFHSignature33> fpfh;
	fpfh.setInputCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr(cloud));
	fpfh.setInputNormals(normals);
	fpfh.setSearchMethod(tree);
	pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs(new pcl::PointCloud<pcl::FPFHSignature33>());

	fpfh.setKSearch(25);
	fpfh.setNumberOfThreads(4);
	fpfh.compute(*fpfhs);
	STDCOUTGREEN;
	cout << "快速点特征直方图FPFH完毕！" << std::endl;
	char normalstxt[] = "C:\\CBR\\normalTXT\\normal%d.txt";
	char data[30];
	sprintf(data, normalstxt, FileNumber);
	ofstream in;
	in.open(data, ios::trunc); //ios::trunc表示在打开文件前将文件清空,由于是写入,文件不存在则创建
	for (size_t i = 0; i < normals->size();i++) {
		for (size_t k=0;k<33;k++)
		{
			in << fpfhs->points[i].histogram[k]<<"\n";
		}
		
	}
	cout << "可视化！" << std::endl;
	/*图形显示模块*/
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D viewer"));
	//viewer->initCameraParameters();

	/*int v1(0), v2(1), v3(2), v4(3);
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	viewer->createViewPort(0.0, 0.5, 0.5, 1.0, v3);
	viewer->createViewPort(0.5, 0.5, 1.0, 1.0, v4);
*/
	////设置背景颜色
	//viewer->setBackgroundColor(0.0, 0.0, 0.0,v1);
	//viewer->setBackgroundColor(25, 55, 10, v2);
	/*int v0(0);
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v0);
	int v1(1);*/
	/*viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v1);*/
	viewer->setBackgroundColor(0.0, 0.0, 0.0); //设置背景色为黑色
	//viewer->setBackgroundColor(0.7451, 0.7451, 0.7451, v1); //设置背景色为黑色
	//设置点云颜色
	pcl::visualization::PointCloudColorHandlerRGBField <pcl::PointXYZRGBA> intensity_distribution(cloud);
	
	viewer->addPointCloud<pcl::PointXYZRGBA>(cloud, intensity_distribution, "intensity");
	viewer->addPointCloudNormals<pcl::PointXYZRGBA, pcl::Normal>(cloud, normals, 50, 1, "normals");

	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
	/*viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "normals", v1);*/
	


	//添加点云法向量的另一种方式；
	//解决来源http://www.pcl-users.org/How-to-add-PointNormal-on-PCLVisualizer-td4037041.html
	//pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	//pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);
	//viewer->addPointCloudNormals<pcl::PointNormal>(cloud_with_normals, 50, 0.01, "normals");
	//
	while (!viewer->wasStopped())
	{
		viewer->spinOnce(100);
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}
}
void testLabelSaveToDB() {
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	clock_t a, b, c, d, e, f;
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		STDCOUTYELLOW;
		cout << "开始写入数据库..." << endl;
		STDCOUTWHITE;
		char sql[1000];
		char sqltext[] = "CREATE TABLE public.%s ( \
		id integer NOT NULL,\
		fileindex integer,\
		classification integer,\
        label integer,\
		CONSTRAINT %s_pkey PRIMARY KEY(id));";
		std::string strTime = "test" + GetNowTime();
		sprintf(sql, sqltext, strTime.c_str(), strTime.c_str());
		PQexec(const_cast<PGconn *>(conn), sql); // 创建新的数据表
		STDCOUTGREEN;
		cout << "已创建数据表：" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 8128;//此处读取待预测txt文件数量大，且其数量已知，直接赋值，避免读取耗时。
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//对应处理器个数，先生成一个储存链接指针的数组
		//ConnectToDB(connArray[0], gphost, pgport, dbname, user, pwd);
		char str[80];
		ifstream fin;
		fin.open("D:\\experiment\\twenty-fifth\\test_all\\filelist_test_shuffle.txt", ios::in);
		int i = 1;
		a = clock();
		while (fin.getline(str, 80))
		{
			PGresult *Res_insert = NULL;
			char sql_insert[1000];
			//待预测文件字符串截取
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//取11783052:0\test_octree\11783052_5_2_000.octree -1
			int posL = fileStr.find(" ");
			std::string label = fileStr.substr(posL, fileStr.size());
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//字符串转int
			std::stringstream iss(clipStr);
			int fileindex;//文件索引
			iss >> fileindex;
			//字符串转int
			std::stringstream Ass(label);
			int labelindex;//文件索引
			Ass >> labelindex;
			sprintf(sql_insert, "INSERT INTO %s VALUES ('%d','%d','%d','%d');", strTime.c_str(), i, fileindex,labelindex,-1);
			Res_insert = PQexec(const_cast<PGconn *>(conn), sql_insert);
			int a = PQresultStatus(Res_insert);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_insert) << endl;
			}
			PQclear(Res_insert);
			i++;
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		fin.close();
		b = clock();
		STDCOUTGREEN;
		std::cout << "写入数据完毕，用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始读取预测后文件并对数据库进行更新..." << endl;
		STDCOUTWHITE;
		std::vector<std::string>files;
		getAllFiles("D:\\experiment\\twenty-fifth\\test_all\\result", files);
		int size = files.size() / 2;
		char labelfile[] = "D:\\experiment\\twenty-fifth\\test_all\\result\\classfy_test%05d_prob";
		int ID = 1;
		c = clock();
		//进度条
		int increment10_label = 0.10 * size;
		int countdown_label = 1;
		int percent10_label = 0;
		for (size_t i = 0; i < size; i++)
		{
			char labelFileIndex[80];
			sprintf(labelFileIndex, labelfile, i);
			char str[10];
			ifstream finLabel;
			finLabel.open(labelFileIndex, ios::in);
			/*读取文本取每九个预测值进行比较，取最大值对应预测标签*/
			int labelindex = 0;//预测标签范围（0-8）
			int label = 0;//预测标签
			float probValue;//预测值
			while (finLabel.getline(str, 10))
			{
				if (labelindex == 0)
				{
					probValue = atof(str);
					label = 0;
				}
				else if (labelindex >= 0 && labelindex <= 8)
				{
					if (probValue < atof(str))
					{
						probValue = atof(str);
						label = labelindex;
					}
					if (labelindex == 8)
					{
						labelindex = -1;
						char sql_update[1000];
						PGresult *Res_update = NULL;
						sprintf(sql_update, "UPDATE %s SET label=%d WHERE id=%d;", strTime.c_str(), label, ID);
						Res_update = PQexec(const_cast<PGconn *>(conn), sql_update);
						int a = PQresultStatus(Res_update);
						if (a != PGRES_COMMAND_OK) {
							cout << PQresultErrorMessage(Res_update) << endl;
						}
						PQclear(Res_update);
						ID++;
					}
				}
				labelindex++;
			}
			finLabel.close();
			if (--countdown_label == 0)
			{
				std::cout << "\r" << std::string(percent10_label, '|') << percent10_label * 10 << "%";
				percent10_label++;
				countdown_label = increment10_label;
				std::cout.flush();
			}
		}
		d = clock();
		STDCOUTGREEN;
		std::cout << "预测标签更新数据库完毕，用时：" << (double)(d - c) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始读取预测后每类正确数，请稍后..." << endl;
		STDCOUTWHITE;
		for (size_t j = 0; j < 9; j++)
		{
			char sql_select[1000];
			PGresult *Res_select = NULL;
			sprintf(sql_select, "SELECT COUNT(*) FROM %s WHERE classification=%d AND label=%d;", strTime.c_str(), j, j);
			Res_select = PQexec(const_cast<PGconn *>(conn), sql_select);
			int a = PQresultStatus(Res_select);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_select) << endl;
			}
			int num = atof(PQgetvalue(Res_select, 0, 0));
			cout<<"第"<<j<<"类，正确数为"<< num <<endl;
			PQclear(Res_select);
		}
	}
}
void DBSaveToFile() {
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	pcl::PointCloud<PointType>::Ptr mycloud(new pcl::PointCloud<PointType>);
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
		mycloud->resize(tuple_num_all);
		int increment10 = 0.10 * tuple_num_all;
		int countdown = 1;
		int percent10 = 0;
		for (int i = 0; i < tuple_num_all; ++i)
		{
			int id = atof(PQgetvalue(Res_All, i, 0));
			mycloud->points[id - 1].x = atof(PQgetvalue(Res_All, i, 1));
			mycloud->points[id - 1].y = atof(PQgetvalue(Res_All, i, 2));
			mycloud->points[id - 1].z = atof(PQgetvalue(Res_All, i, 3));
			mycloud->points[id - 1].r = atoi(PQgetvalue(Res_All, i, 4));
			mycloud->points[id - 1].g = atoi(PQgetvalue(Res_All, i, 5));
			mycloud->points[id - 1].b = atoi(PQgetvalue(Res_All, i, 6));
			mycloud->points[id - 1].rgba = atoi(PQgetvalue(Res_All, i, 7));
			mycloud->points[id - 1].normal_x=atoi(PQgetvalue(Res_All, i, 8));
			mycloud->points[id - 1].normal_y = atoi(PQgetvalue(Res_All, i, 9));
			mycloud->points[id - 1].normal_z = atoi(PQgetvalue(Res_All, i, 10));
			mycloud->points[id - 1].Roughness = atoi(PQgetvalue(Res_All, i, 11));
			for (size_t k = 0; k < 33; k++)
			{
				mycloud->points[id - 1].histogram[k] = 0;
			}
			mycloud->points[id - 1].fileIndex = atof(PQgetvalue(Res_All, i, 13));
			int label = atof(PQgetvalue(Res_All, i, 14));
			if (label==2) {
				mycloud->points[id - 1].Classification =-1;
			}
			else
			{
				mycloud->points[id - 1].Classification = atof(PQgetvalue(Res_All, i, 14));
			}
			
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		PQclear(Res_All); // 清理数据库查询结果数据集
		cout << "请输入文件名：" << endl;
		std::string filename;
		cin >> filename;
		cout << "正在保存点云数据：" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\SelectedData\\" + filename + ".pcd", *mycloud);
		STDCOUTGREEN;
		std::cout << "保存点云数据成功！" << std::endl;
		STDCOUTWHITE;
		//char labelFileIndex[]="D:\\experiment\\eleventh\\test\\fileindex.txt";
		//char str[10];
		//ifstream finLabel;
		//finLabel.open(labelFileIndex, ios::in);
		//while (finLabel.getline(str, 10))
		//{
		//	int fileindex = atof(str);
		//	PGresult *Res_update = NULL;
		//	sprintf(sql, "UPDATE table20191227103819 SET classification=6 WHERE fileindex=%d;", fileindex);
		//	int k = omp_get_thread_num();//获得每个线程的ID
		//	Res_update = PQexec(const_cast<PGconn *>(conn), sql);
		//	int a = PQresultStatus(Res_update);
		//	if (a != PGRES_COMMAND_OK) {
		//		cout << PQresultErrorMessage(Res_update) << endl;
		//	}
		//	PQclear(Res_update);
		//}
		//finLabel.close();	
	}
}
//读取数据库分类后全区结果（除草）
void removeGlass() {
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	pcl::PointCloud<PointType>::Ptr mycloud(new pcl::PointCloud<PointType>);
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
		char sql_all[] = "SELECT * FROM %s where classification!=1 ORDER BY id";
		sprintf(sql, sql_all, tablename.c_str());
		Res_All = PQexec(const_cast<PGconn *>(conn), sql);
		int tuple_num_all = PQntuples(Res_All);
		int field_num_all = PQnfields(Res_All);
		int classification_filed_num;
		mycloud->resize(tuple_num_all);
		int increment10 = 0.10 * tuple_num_all;
		int countdown = 1;
		int percent10 = 0;
		for (int i = 0; i < tuple_num_all; ++i)
		{
			//int id = atof(PQgetvalue(Res_All, i, 0));
			mycloud->points[i].x = atof(PQgetvalue(Res_All, i, 1));
			mycloud->points[i].y = atof(PQgetvalue(Res_All, i, 2));
			mycloud->points[i].z = atof(PQgetvalue(Res_All, i, 3));
			mycloud->points[i].r = atoi(PQgetvalue(Res_All, i, 4));
			mycloud->points[i].g = atoi(PQgetvalue(Res_All, i, 5));
			mycloud->points[i].b = atoi(PQgetvalue(Res_All, i, 6));
			mycloud->points[i].rgba = atoi(PQgetvalue(Res_All, i, 7));
			mycloud->points[i].normal_x = atoi(PQgetvalue(Res_All, i, 8));
			mycloud->points[i].normal_y = atoi(PQgetvalue(Res_All, i, 9));
			mycloud->points[i].normal_z = atoi(PQgetvalue(Res_All, i, 10));
			mycloud->points[i].Roughness = atoi(PQgetvalue(Res_All, i, 11));
			for (size_t k = 0; k < 33; k++)
			{
				mycloud->points[i].histogram[k] = 0;
			}
			mycloud->points[i].fileIndex = atof(PQgetvalue(Res_All, i, 13));
			mycloud->points[i].Classification = atof(PQgetvalue(Res_All, i, 14));

			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		PQclear(Res_All); // 清理数据库查询结果数据集
		cout << "请输入文件名：" << endl;
		std::string filename;
		cin >> filename;
		cout << "正在保存点云数据：" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\Filtering\\" + filename + ".pcd", *mycloud);
		STDCOUTGREEN;
		std::cout << "保存点云数据成功！" << std::endl;
		STDCOUTWHITE;
	}
}
//移除离群点
void removeoOutlier() {
	pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
	pcl::PointCloud<PointType>::Ptr cloud_filtered(new pcl::PointCloud<PointType>);
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
		// Create the filtering object
		pcl::StatisticalOutlierRemoval<PointType> sor;
		sor.setInputCloud(cloud);
		sor.setMeanK(100);
		sor.setStddevMulThresh(1.0);
		sor.filter(*cloud_filtered);
		cout << "处理完毕将保存文件请输入文件名：" << endl;
		std::string filename;
		cin >> filename;
		cout << "正在保存点云数据：" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\Filtering\\" + filename + ".pcd", *cloud_filtered);
		STDCOUTGREEN;
		std::cout << "保存点云数据成功！" << std::endl;
		STDCOUTWHITE;

	}
}
//纯化后测试精度
void TestAccAfterpPurification() {
	PGconn *conn;
	conn = NULL;
	char gphost[] = "localhost";
	char pgport[] = "5432";
	char dbname[] = "CBA";
	char user[] = "admin";
	char pwd[] = "123456";
	clock_t a, b, c, d, e, f;
	if (ConnectToDB(conn, gphost, pgport, dbname, user, pwd)) {
		STDCOUTYELLOW;
		cout << "开始写入数据库..." << endl;
		STDCOUTWHITE;
		char sql[1000];
		char sqltext[] = "CREATE TABLE public.%s ( \
		id integer NOT NULL,\
		fileindex integer,\
		classification integer,\
        label integer,\
		CONSTRAINT %s_pkey PRIMARY KEY(id));";
		std::string strTime = "test" + GetNowTime();
		sprintf(sql, sqltext, strTime.c_str(), strTime.c_str());
		PQexec(const_cast<PGconn *>(conn), sql); // 创建新的数据表
		STDCOUTGREEN;
		cout << "已创建数据表：" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 13568;//此处读取待预测txt文件数量大，且其数量已知，直接赋值，避免读取耗时。
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//对应处理器个数，先生成一个储存链接指针的数组
		//ConnectToDB(connArray[0], gphost, pgport, dbname, user, pwd);
		char str[80];
		ifstream fin;
		fin.open("D:\\experiment\\twenty-fifth\\test_all\\filelist_test_shuffle.txt", ios::in);
		int i = 1;
		a = clock();
		while (fin.getline(str, 80))
		{
			PGresult *Res_insert = NULL;
			PGresult *Res_select = NULL;
			char sql_insert[1000];
			//待预测文件字符串截取
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//取11783052:0\test_octree\11783052_5_2_000.octree -1
			int posL = fileStr.find(" ");
			std::string label = fileStr.substr(posL, fileStr.size());
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//字符串转int
			std::stringstream iss(clipStr);
			int fileindex;//文件索引
			iss >> fileindex;
			//字符串转int
			std::stringstream Ass(label);
			int labelindex;//类别
			Ass >> labelindex;
			//读取纯化后数据表对应文件索引点类别
			char sql_select[100];
			sprintf(sql_select, "SELECT classification FROM %s WHERE fileindex=%d", "purificationresult", fileindex);
			Res_select = PQexec(const_cast<PGconn *>(conn), sql_select);
			int trueLabel;
			if (PQgetvalue(Res_select, 0, 0)==NULL)
			{
				trueLabel = -1;
			}
			else
			{
			    trueLabel = atof(PQgetvalue(Res_select, 0, 0));
			}
			sprintf(sql_insert, "INSERT INTO %s VALUES ('%d','%d','%d','%d');", strTime.c_str(), i, fileindex, labelindex, trueLabel);
			Res_insert = PQexec(const_cast<PGconn *>(conn), sql_insert);
			int a = PQresultStatus(Res_insert);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_insert) << endl;
			}
			PQclear(Res_select);
			PQclear(Res_insert);
			i++;
			if (--countdown == 0)
			{
				std::cout << "\r" << std::string(percent10, '|') << percent10 * 10 << "%";
				percent10++;
				countdown = increment10;
				std::cout.flush();
			}
		}
		fin.close();
		b = clock();
		STDCOUTGREEN;
		std::cout << "写入数据完毕，用时：" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "开始读取纯化后每类正确数，请稍后..." << endl;
		STDCOUTWHITE;
		for (size_t j = 0; j < 9; j++)
		{
			char sql_select[1000];
			PGresult *Res_select = NULL;
			sprintf(sql_select, "SELECT COUNT(*) FROM %s WHERE classification=%d AND label=%d;", strTime.c_str(), j, j);
			Res_select = PQexec(const_cast<PGconn *>(conn), sql_select);
			int a = PQresultStatus(Res_select);
			if (a != PGRES_COMMAND_OK) {
				cout << PQresultErrorMessage(Res_select) << endl;
			}
			int num = atof(PQgetvalue(Res_select, 0, 0));
			cout << "第" << j << "类，正确数为" << num << endl;
			PQclear(Res_select);
		}
	}
}