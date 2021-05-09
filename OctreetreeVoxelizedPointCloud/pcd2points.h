#pragma once
#include "common.h"
#include "direct.h"
#include "dataAttributes.h"

void TransformPointformat() {
	clock_t a, b, c, d, e, f,g,h;
	STDCOUTWHITE;
mark:
	cout << "��ѡ����Ҫת���ĵ��ƣ�" << endl;
	STDCOUTBLUE;
	cout << "��1��.��������" << std::endl;
	cout << "��2��.ȫ������" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		cout << "��ѡ����Ҫ�������ԣ�" << endl;
		STDCOUTBLUE;
		cout << "��1��.RGB" << std::endl;
		cout << "��2��.RGB+������" << std::endl;
		cout << "��3��.������+FPFH" << std::endl;
		cout << "��4��.RGB+�ֲڶ�" << std::endl;
		cout << "��5��.RGB+�ֲڶ�+������+FPFH" << std::endl;
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
		cout << "��ѡ����Ҫ�������ԣ�" << endl;
		STDCOUTBLUE;
		cout << "��1��.RGB" << std::endl;
		cout << "��2��.RGB+������" << std::endl;
		cout << "��3��.������+FPFH" << std::endl;
		cout << "��4��.RGB+�ֲڶ�" << std::endl;
		cout << "��5��.RGB+�ֲڶ�+������+FPFH" << std::endl;
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
	//���ݼ����ɹ����л���һЩ����Ϊ�գ�������޸�
	clock_t a, b, c, d, e, f,g,h;
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
	//pcd���������ļ���
	char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train";
	char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test";
	std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
	std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
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
		//train�ļ���
		sprintf(trainSource, trainDatafile, i);
		getAllFiles(trainSource, trainfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment10 = 0.10 * trainfiles.size();
		int countdown = 1;
		int percent10 = 0;
		STDCOUTWHITE;
		cout << "���" << i << "ѵ�����ݼ���ʼ�޸�.." << endl;
		a = clock();
		char voxelDatafile1[] = "C:\\CBR\\VoxelPointData\\%s.pcd";
		/*char voxelDatafile1[] = "H:\\VoxelPointData\\%s.pcd";*/
		for (int j = 0; j < trainfiles.size(); j++)
		{
			//��ȡpcd�ļ���
			int pos = trainfiles[j].find_last_of("\\");
			int pos1 = trainfiles[j].find(".");
			std::string clipName = trainfiles[j].substr(pos + 1, pos1 - pos - 1);
			sprintf(trainSource, voxelDatafile1, clipName.c_str());
			CopyFile(trainSource, trainfiles[j].c_str(), false);//false�����ǣ�true������
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
		cout << "���" << i << "ѵ�����ݼ��޸���ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
		STDCOUTWHITE;
		//test�ļ���
		sprintf(testSource, testDatafile, i);
		getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
		//������
		int increment = 0.10 * testfiles.size();
		int countdown10 = 1;
		int percent = 0;
		cout << "���" << i << "�������ݼ���ʼ�޸�.." << endl;
		c = clock();
		for (int k = 0; k < testfiles.size(); k++)
		{
			//��ȡpcd�ļ���
			int pos = testfiles[k].find_last_of("\\");
			int pos1 = testfiles[k].find(".");
			std::string clipName = testfiles[k].substr(pos + 1, pos1 - pos - 1);
			sprintf(testSource, voxelDatafile1, clipName.c_str());
			CopyFile(testSource, testfiles[k].c_str(), false);//false�����ǣ�true������
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
		cout << "���" << i << "�������ݼ��޸���ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
		STDCOUTWHITE;
	}
	f = clock();
	cout << "�ܹ�" << classindex << "������" << "ȫ���޸���ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
}
void buildTXT() {
	//��.point�ļ�ת��.octree�ļ����Լ�.octree�ļ���lmdb��Ҫ����filelist.txt�ļ����ṩ���롣
	clock_t a, b, c, d, e, f,g,h;
	STDCOUTWHITE;
mark:
	cout << "��ѡ����Ҫ���еĹ�����" << endl;
	STDCOUTBLUE;
	cout << "��1��.����.points�ļ�Ŀ¼TXT" << std::endl;
	cout << "��2��.����.octree�ļ�Ŀ¼TXT" << std::endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;
	switch (function_int)
	{
	case 1: {
		cout << "��ѡ��" << endl;
		STDCOUTBLUE;
		cout << "��1��.��������" << std::endl;
		cout << "��2��.ȫ������" << std::endl;
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
			//points���������ļ���
			char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train_points";
			char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test_points";
			char valDatafile[] = "C:\\CBR\\CnnData\\%d\\val_points";
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
				cout << "���" << i << "ѵ��points�����ļ�·����Ϣ��ʼд��TXT.." << endl;
				a = clock();
				sprintf(trainPoints, "C:\\CBR\\CnnData\\%d\\train_points\\filelist.txt", i);
				ofstream in;
				in.open(trainPoints, ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
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
				in.close();//�ر��ļ�
				b = clock();
				STDCOUTGREEN;
				cout << "���" << i << "ѵ��points�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//test�ļ���
				sprintf(testSource, testDatafile, i);
				getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
				//������
				int increment = 0.10 * testfiles.size();
				int countdown10 = 1;
				int percent = 0;
				cout << "���" << i << "����points�����ļ�·����Ϣ��ʼд��TXT.." << endl;
				c = clock();
				sprintf(testPoints, "C:\\CBR\\CnnData\\%d\\test_points\\filelist.txt", i);
				ofstream on;
				on.open(testPoints, ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
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
				on.close();//�ر��ļ�
				d = clock();
				STDCOUTGREEN;
				cout << "���" << i << "����points�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//val�ļ���
				sprintf(valSource, valDatafile, i);
				getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
				//������
				int incrementval10 = 0.10 * valfiles.size();
				int countdownval = 1;
				int percentval10 = 0;
				STDCOUTWHITE;
				cout << "���" << i << "��֤points�����ļ�·����Ϣ��ʼд��TXT.." << endl;
				g = clock();
				sprintf(valPoints, "C:\\CBR\\CnnData\\%d\\val_points\\filelist.txt", i);
				ofstream inval;
				inval.open(valPoints, ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
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
				inval.close();//�ر��ļ�
				h = clock();
				STDCOUTGREEN;
				cout << "���" << i << "��֤points�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
				STDCOUTWHITE;
			}
			f = clock();
			cout << "�ܹ�" << classindex << "������" << "ȫ��·����Ϣд��TXT��ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
		}break;
		case 2: {
			clock_t a, b;
			char voxelDatafile[] = "C:\\CBR\\VoxelPointData_points";
			std::vector<std::string>files;
			char dataTransform[] = "C:\\CBR\\VoxelPointData_points";
			ofstream in;
			in.open("C:\\CBR\\VoxelPointData_points\\filelist.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
			//������
			int increment10 = 0.10 * files.size();
			int countdown = 1;
			int percent10 = 0;
			STDCOUTWHITE;
			cout << "ȫ��points����·����Ϣ��ʼд��TXT.." << endl;
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
			in.close();//�ر��ļ�
			b = clock();
			STDCOUTGREEN;
			cout << "ȫ��points�ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
			STDCOUTWHITE;
		}break;
		default:
			break;
		}
	}break;
	case 2: {
		cout << "��ѡ��" << endl;
		STDCOUTBLUE;
		cout << "��1��.��������" << std::endl;
		cout << "��2��.ȫ������" << std::endl;
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
			//octree�ļ������ļ���
			char trainDatafile[] = "C:\\CBR\\CnnData\\%d\\train_octree";
			char testDatafile[] = "C:\\CBR\\CnnData\\%d\\test_octree";
			char valDatafile[] = "C:\\CBR\\CnnData\\%d\\val_octree";
			std::vector<std::string> trainfiles;           //�����ļ�·����Ϣ
			std::vector<std::string> testfiles;           //�����ļ�·����Ϣ
			std::vector<std::string> valfiles;           //�����ļ�·����Ϣ
			e = clock();
			ofstream in;
			in.open("D:\\experiment\\twenty-fifth\\filelist_train.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
			ofstream on;
			on.open("D:\\experiment\\twenty-fifth\\filelist_test.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
			ofstream on_val;
			on_val.open("D:\\experiment\\twenty-fifth\\filelist_val.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
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
				cout << "���" << i << "ѵ��octree�����ļ�·����Ϣ��ʼд��TXT.." << endl;
				a = clock();
				for (int j = 0; j < trainfiles.size(); j++)
				{
					string str = trainfiles[j].c_str();
					//�˴�11���Զ����ļ������Ƶĳ��ȣ���C:/CBR/CnnData/1/train��,���ȡΪ��1/train���ɾ�ʵ���������
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
				cout << "���" << i << "ѵ��octree�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//test�ļ���
				sprintf(testSource, testDatafile, i);
				getAllFiles(testSource, testfiles);  //��ȡ�ļ����еĵ����ļ�
				//������
				int increment = 0.10 * testfiles.size();
				int countdown10 = 1;
				int percent = 0;
				cout << "���" << i << "����octree�����ļ�·����Ϣ��ʼд��TXT.." << endl;
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
				cout << "���" << i << "����octree�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(d - c) / 1000 << "s" << endl;
				STDCOUTWHITE;
				//val�ļ���
				sprintf(valSource, valDatafile, i);
				getAllFiles(valSource, valfiles);  //��ȡ�ļ����еĵ����ļ�
				//������
				int incrementval = 0.10 * valfiles.size();
				int countdownval10 = 1;
				int percentval = 0;
				cout << "���" << i << "��֤octree�����ļ�·����Ϣ��ʼд��TXT.." << endl;
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
				cout << "���" << i << "��֤octree�����ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(h - g) / 1000 << "s" << endl;
				STDCOUTWHITE;
			}
			in.close();
			on.close();
			on_val.close();
			f = clock();
			cout << "�ܹ�" << classindex << "������" << "ȫ��·����Ϣд��TXT��ϣ�" << "����ʱ" << (double)(f - e) / 1000 << "s" << endl;
		}break;
		case 2: {
			clock_t a, b;
			char voxelDatafile[] = "H:\\VoxelPointData_octree";
			std::vector<std::string>files;
			ofstream in;
			in.open("D:\\AllDataTxt\\filelist_all.txt", ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
			//������
			int increment10 = 0.10 * files.size();
			int countdown = 1;
			int percent10 = 0;
			STDCOUTWHITE;
			cout << "ȫ��octree����·����Ϣ��ʼд��TXT.." << endl;
			getAllFiles(voxelDatafile, files);
			a = clock();
			for (int j = 0; j < files.size(); j++)
			{
				string str = files[j].c_str();
				//�˴�11���Զ����ļ������Ƶĳ��ȣ���C:/CBR/VoxelPointData_octree��,���ȡΪ��/VoxelPointData_octree���ɾ�ʵ���������
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
			in.close();//�ر��ļ�
			b = clock();
			STDCOUTGREEN;
			cout << "ȫ��octree�ļ�·����Ϣд��TXT��ϣ�" << "��ʱ" << (double)(b - a) / 1000 << "s" << endl;
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
		cout << "��ʼд�����ݿ�..." << endl;
		STDCOUTWHITE;
		char sql[1000];
		char sqltext[] = "CREATE TABLE public.%s ( \
		id integer NOT NULL,\
		fileindex integer,\
		classification integer,\
		CONSTRAINT %s_pkey PRIMARY KEY(id));";
		std::string strTime = "label" + GetNowTime();
		sprintf(sql, sqltext, strTime.c_str(), strTime.c_str());
		PQexec(const_cast<PGconn *>(conn), sql); // �����µ����ݱ�
		STDCOUTGREEN;
		cout << "�Ѵ������ݱ�" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 3211115;//�˴���ȡ��Ԥ��txt�ļ�����������������֪��ֱ�Ӹ�ֵ�������ȡ��ʱ��
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//��Ӧ������������������һ����������ָ�������
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
			//��Ԥ���ļ��ַ�����ȡ
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//ȡ11783052:VoxelPointData_octree\11783052_5_2_000.octree -1
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//�ַ���תint
			std::stringstream iss(clipStr);
			int fileindex;//�ļ�����
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
		std::cout << "д��������ϣ���ʱ��" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "��ʼ��ȡԤ����ļ��������ݿ���и���..." << endl;
		STDCOUTWHITE;
		std::vector<std::string>files;
		getAllFiles("D:\\experiment\\twenty-fifth\\result", files);
		int size = files.size() / 2;
		char labelfile[] = "D:\\experiment\\twenty-fifth\\result\\classfy_test%05d_prob";
		int ID = 1;
		c = clock();
		//������
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
			/*��ȡ�ı�ȡÿ�Ÿ�Ԥ��ֵ���бȽϣ�ȡ���ֵ��ӦԤ���ǩ*/
			int labelindex = 0;//Ԥ���ǩ��Χ��0-8��
			int label = 0;//Ԥ���ǩ
			float probValue;//Ԥ��ֵ
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
		std::cout << "Ԥ���ǩ�������ݿ���ϣ���ʱ��" << (double)(d - c) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "��ʼ����һ��Ԥ���ǩ���ݿ⸳ֵ�������������ǩ..." << endl;
		STDCOUTWHITE;
		char sql_select[2000];
		// ��ȡ���ݱ���
		PGresult *Res_tablename = NULL;
		//������������ǩ�������ϲ�ѯ�õ��������
		char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
		Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
		int tuple_num_tablename = PQntuples(Res_tablename);
		cout << "��ѡ���" << endl;
		STDCOUTBLUE;
		for (size_t i = 0; i < tuple_num_tablename; i++)
		{
			cout << "��" << i + 1 << "��  " << PQgetvalue(Res_tablename, i, 0) << endl;
		}
		STDCOUTWHITE;
		int tableindex;
		cin >> tableindex;
		std::string Tname = PQgetvalue(Res_tablename, tableindex - 1, 0);
		PQclear(Res_tablename);
		PGresult *Res_All = NULL;
		pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
		char sql_all[] = "select %s.id,%s.xvalue,%s.yvalue,%s.zvalue,%s.rvalue,%s.gvalue,%s.bvalue,%s.rgbavalue,%s.fileindex,%s.classification from %s,%s where %s.fileindex=%s.fileindex order by id";
		sprintf(sql_select, sql_all, Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), Tname.c_str(), strTime.c_str(), Tname.c_str(), strTime.c_str(), Tname.c_str(), strTime.c_str());//���ݱ�Ϊ��һ�����µ�Ԥ���ǩ��
		Res_All = PQexec(const_cast<PGconn *>(conn), sql_select);
		int tuple_num_all = PQntuples(Res_All);
		int field_num_all = PQnfields(Res_All);
		//������
		int increment = 0.10 * tuple_num_all;
		int countdown10 = 1;
		int percent = 0;
		cloud->resize(tuple_num_all + 5);
		e = clock();
		STDCOUTYELLOW;
		cout << "��ʼ���ɴ��������.." << endl;
		STDCOUTWHITE;
		omp_set_num_threads(4);  // ���̲߳��м���
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
		PQfinish(conn);//�ر�����
		STDCOUTGREEN;
		std::cout << "���ɷ��������ϣ���ʱ��" << (double)(f - e) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		std::string filename = "classication" + GetNowTime();
		cout << "���ڱ���������ݣ�" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\ClassifiedData\\" + filename + ".pcd", *cloud);
		STDCOUTGREEN;
		std::cout << "����������ݳɹ���" << std::endl;
		STDCOUTWHITE;
	}
}
void normalVisualization()
{
	std::vector<std::string> files;           //�����ļ�·����Ϣ
	//���ص���ģ��
	getAllFiles(WorkPath + "\\" + "RawData", files);  //��ȡ�ļ����еĵ����ļ�
	if (files.size() == 0) {
		STDCOUTRED;
		cout << "��û�����ɵ����ļ���" << endl;
		STDCOUTWHITE;
		return;
	}
	cout << "���������ļ���" << endl;
	STDCOUTBLUE;
	for (int i = 0; i < files.size(); i++)
	{
		cout << "��" << i + 1 << "��" << files[i].c_str() << endl;
	}
	STDCOUTWHITE;
	cout << "��������Ҫ��ȡ�ļ�����ţ�" << endl;
	int FileNumber;
	cin >> FileNumber;
	pcl::PointCloud<pcl::PointXYZRGBA>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZRGBA>);;

	STDCOUTYELLOW;
	cout << "��ʼ��ȡ�ļ�" << files[FileNumber - 1].c_str() << "����..." << endl;
	STDCOUTWHITE;

	pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);

	STDCOUTGREEN;
	std::cout << "��ȡ" << files[FileNumber - 1].c_str() << "��ɣ�" << std::endl;
	STDCOUTWHITE;
	cout << "���Ʊ��淨��..." << endl;
	//���㷨��
	pcl::NormalEstimationOMP<pcl::PointXYZRGBA, pcl::Normal> n(4);
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	//����kdtree�����н��ڵ㼯����
	pcl::search::KdTree<pcl::PointXYZRGBA>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZRGBA>);
	//Ϊkdtree��ӵ�������
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	//���Ʒ������ʱ����Ҫ���ѵĽ��ڵ��С
	n.setKSearch(25);
	//��ʼ���з������
	n.compute(*normals);
	cout << "���Ʊ��淨����ϣ�" << std::endl;
	STDCOUTWHITE;
	cout << "���ƿ��ٵ�����ֱ��ͼFPFH..." << endl;
	// ����FPFH
	//����FPFH���ƶ���fpfh,�����������ݼ�cloud�ͷ���normals���ݸ���
	pcl::FPFHEstimationOMP<pcl::PointXYZRGBA, pcl::Normal, pcl::FPFHSignature33> fpfh;
	fpfh.setInputCloud(pcl::PointCloud<pcl::PointXYZRGBA>::Ptr(cloud));
	fpfh.setInputNormals(normals);
	fpfh.setSearchMethod(tree);
	pcl::PointCloud<pcl::FPFHSignature33>::Ptr fpfhs(new pcl::PointCloud<pcl::FPFHSignature33>());

	fpfh.setKSearch(25);
	fpfh.setNumberOfThreads(4);
	fpfh.compute(*fpfhs);
	STDCOUTGREEN;
	cout << "���ٵ�����ֱ��ͼFPFH��ϣ�" << std::endl;
	char normalstxt[] = "C:\\CBR\\normalTXT\\normal%d.txt";
	char data[30];
	sprintf(data, normalstxt, FileNumber);
	ofstream in;
	in.open(data, ios::trunc); //ios::trunc��ʾ�ڴ��ļ�ǰ���ļ����,������д��,�ļ��������򴴽�
	for (size_t i = 0; i < normals->size();i++) {
		for (size_t k=0;k<33;k++)
		{
			in << fpfhs->points[i].histogram[k]<<"\n";
		}
		
	}
	cout << "���ӻ���" << std::endl;
	/*ͼ����ʾģ��*/
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D viewer"));
	//viewer->initCameraParameters();

	/*int v1(0), v2(1), v3(2), v4(3);
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v1);
	viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v2);
	viewer->createViewPort(0.0, 0.5, 0.5, 1.0, v3);
	viewer->createViewPort(0.5, 0.5, 1.0, 1.0, v4);
*/
	////���ñ�����ɫ
	//viewer->setBackgroundColor(0.0, 0.0, 0.0,v1);
	//viewer->setBackgroundColor(25, 55, 10, v2);
	/*int v0(0);
	viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v0);
	int v1(1);*/
	/*viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v1);*/
	viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
	//viewer->setBackgroundColor(0.7451, 0.7451, 0.7451, v1); //���ñ���ɫΪ��ɫ
	//���õ�����ɫ
	pcl::visualization::PointCloudColorHandlerRGBField <pcl::PointXYZRGBA> intensity_distribution(cloud);
	
	viewer->addPointCloud<pcl::PointXYZRGBA>(cloud, intensity_distribution, "intensity");
	viewer->addPointCloudNormals<pcl::PointXYZRGBA, pcl::Normal>(cloud, normals, 50, 1, "normals");

	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
	/*viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "normals", v1);*/
	


	//��ӵ��Ʒ���������һ�ַ�ʽ��
	//�����Դhttp://www.pcl-users.org/How-to-add-PointNormal-on-PCLVisualizer-td4037041.html
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
		cout << "��ʼд�����ݿ�..." << endl;
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
		PQexec(const_cast<PGconn *>(conn), sql); // �����µ����ݱ�
		STDCOUTGREEN;
		cout << "�Ѵ������ݱ�" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 8128;//�˴���ȡ��Ԥ��txt�ļ�����������������֪��ֱ�Ӹ�ֵ�������ȡ��ʱ��
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//��Ӧ������������������һ����������ָ�������
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
			//��Ԥ���ļ��ַ�����ȡ
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//ȡ11783052:0\test_octree\11783052_5_2_000.octree -1
			int posL = fileStr.find(" ");
			std::string label = fileStr.substr(posL, fileStr.size());
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//�ַ���תint
			std::stringstream iss(clipStr);
			int fileindex;//�ļ�����
			iss >> fileindex;
			//�ַ���תint
			std::stringstream Ass(label);
			int labelindex;//�ļ�����
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
		std::cout << "д��������ϣ���ʱ��" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "��ʼ��ȡԤ����ļ��������ݿ���и���..." << endl;
		STDCOUTWHITE;
		std::vector<std::string>files;
		getAllFiles("D:\\experiment\\twenty-fifth\\test_all\\result", files);
		int size = files.size() / 2;
		char labelfile[] = "D:\\experiment\\twenty-fifth\\test_all\\result\\classfy_test%05d_prob";
		int ID = 1;
		c = clock();
		//������
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
			/*��ȡ�ı�ȡÿ�Ÿ�Ԥ��ֵ���бȽϣ�ȡ���ֵ��ӦԤ���ǩ*/
			int labelindex = 0;//Ԥ���ǩ��Χ��0-8��
			int label = 0;//Ԥ���ǩ
			float probValue;//Ԥ��ֵ
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
		std::cout << "Ԥ���ǩ�������ݿ���ϣ���ʱ��" << (double)(d - c) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "��ʼ��ȡԤ���ÿ����ȷ�������Ժ�..." << endl;
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
			cout<<"��"<<j<<"�࣬��ȷ��Ϊ"<< num <<endl;
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

		// ��ȡ���ݱ���
		PGresult *Res_tablename = NULL;
		char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
		Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
		int tuple_num_tablename = PQntuples(Res_tablename);
		cout << "��ѡ���" << endl;
		STDCOUTBLUE;
		for (size_t i = 0; i < tuple_num_tablename; i++)
		{
			cout << "��" << i + 1 << "��  " << PQgetvalue(Res_tablename, i, 0) << endl;
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
		PQclear(Res_All); // �������ݿ��ѯ������ݼ�
		cout << "�������ļ�����" << endl;
		std::string filename;
		cin >> filename;
		cout << "���ڱ���������ݣ�" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\SelectedData\\" + filename + ".pcd", *mycloud);
		STDCOUTGREEN;
		std::cout << "����������ݳɹ���" << std::endl;
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
		//	int k = omp_get_thread_num();//���ÿ���̵߳�ID
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
//��ȡ���ݿ�����ȫ����������ݣ�
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

		// ��ȡ���ݱ���
		PGresult *Res_tablename = NULL;
		char sql_gettablename[] = "SELECT tablename FROM pg_tables WHERE tablename NOT LIKE 'pg%' AND tablename NOT LIKE 'sql_%' ORDER BY tablename;";
		Res_tablename = PQexec(const_cast<PGconn *>(conn), sql_gettablename);
		int tuple_num_tablename = PQntuples(Res_tablename);
		cout << "��ѡ���" << endl;
		STDCOUTBLUE;
		for (size_t i = 0; i < tuple_num_tablename; i++)
		{
			cout << "��" << i + 1 << "��  " << PQgetvalue(Res_tablename, i, 0) << endl;
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
		PQclear(Res_All); // �������ݿ��ѯ������ݼ�
		cout << "�������ļ�����" << endl;
		std::string filename;
		cin >> filename;
		cout << "���ڱ���������ݣ�" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\Filtering\\" + filename + ".pcd", *mycloud);
		STDCOUTGREEN;
		std::cout << "����������ݳɹ���" << std::endl;
		STDCOUTWHITE;
	}
}
//�Ƴ���Ⱥ��
void removeoOutlier() {
	pcl::PointCloud<PointType>::Ptr cloud(new pcl::PointCloud<PointType>);
	pcl::PointCloud<PointType>::Ptr cloud_filtered(new pcl::PointCloud<PointType>);
	std::vector<std::string> files;           //�����ļ�·����Ϣ
	while (!files.empty())           //���vector
	{
		files.pop_back();
	}
	cout << "�������ļ������ƣ�" << endl;
	std::string floderName;
	cin >> floderName;
	if (floderName != "") {
		getAllFiles(WorkPath + "\\" + floderName, files);  //��ȡ�ļ����еĵ����ļ�
		if (files.size() == 0) {
			STDCOUTRED;
			cout << "��û�����ɵ����ļ���" << endl;
			STDCOUTWHITE;
			return;
		}
		cout << "���������ļ���" << endl;
		STDCOUTBLUE;
		for (int i = 0; i < files.size(); i++)
		{
			cout << "��" << i + 1 << "��" << files[i].c_str() << endl;
		}
		STDCOUTWHITE;
		cout << "��������Ҫ��ȡ�ļ�����ţ�" << endl;
		int FileNumber;
		cin >> FileNumber;
		clock_t a, b, c, d;
		STDCOUTYELLOW;
		cout << "��ʼ��ȡ�ļ�" << files[FileNumber - 1].c_str() << "����..." << endl;
		STDCOUTWHITE;
		a = clock();
		pcl::io::loadPCDFile(files[FileNumber - 1].c_str(), *cloud);
		b = clock();
		STDCOUTGREEN;
		std::cout << "��ȡ" << files[FileNumber - 1].c_str() << "������ʱ��" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		// Create the filtering object
		pcl::StatisticalOutlierRemoval<PointType> sor;
		sor.setInputCloud(cloud);
		sor.setMeanK(100);
		sor.setStddevMulThresh(1.0);
		sor.filter(*cloud_filtered);
		cout << "������Ͻ������ļ��������ļ�����" << endl;
		std::string filename;
		cin >> filename;
		cout << "���ڱ���������ݣ�" << filename << ".pcd" << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\Filtering\\" + filename + ".pcd", *cloud_filtered);
		STDCOUTGREEN;
		std::cout << "����������ݳɹ���" << std::endl;
		STDCOUTWHITE;

	}
}
//��������Ծ���
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
		cout << "��ʼд�����ݿ�..." << endl;
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
		PQexec(const_cast<PGconn *>(conn), sql); // �����µ����ݱ�
		STDCOUTGREEN;
		cout << "�Ѵ������ݱ�" << strTime.c_str() << endl;
		STDCOUTWHITE;
		size_t length = 13568;//�˴���ȡ��Ԥ��txt�ļ�����������������֪��ֱ�Ӹ�ֵ�������ȡ��ʱ��
		int increment10 = 0.10 * length;
		int countdown = 1;
		int percent10 = 0;
		//PGconn *connArray[1];//��Ӧ������������������һ����������ָ�������
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
			//��Ԥ���ļ��ַ�����ȡ
			std::string fileStr(str);
			int posF = fileStr.find_last_of("\\");
			int posS = fileStr.find(".");//ȡ11783052:0\test_octree\11783052_5_2_000.octree -1
			int posL = fileStr.find(" ");
			std::string label = fileStr.substr(posL, fileStr.size());
			std::string clipStr = fileStr.substr(posF + 1, posS - 9 - posF);
			//�ַ���תint
			std::stringstream iss(clipStr);
			int fileindex;//�ļ�����
			iss >> fileindex;
			//�ַ���תint
			std::stringstream Ass(label);
			int labelindex;//���
			Ass >> labelindex;
			//��ȡ���������ݱ��Ӧ�ļ����������
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
		std::cout << "д��������ϣ���ʱ��" << (double)(b - a) / 1000 << "s" << std::endl;
		STDCOUTWHITE;
		STDCOUTYELLOW;
		cout << "��ʼ��ȡ������ÿ����ȷ�������Ժ�..." << endl;
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
			cout << "��" << j << "�࣬��ȷ��Ϊ" << num << endl;
			PQclear(Res_select);
		}
	}
}