#define PCL_NO_PRECOMPILE

#include "login.h"
#include "pcd2points.h"

int main()
{
	//std::vector<int> a(7);
	//a[0] = 1;
	//a[1] = 1;
	//a[2] = 1;
	//a[3] = 3;
	//a[4] = 3;
	//a[5] = 3;
	//a[6] = 3;
	//cout<<findMode(a);
	// ��֤�ļ��нṹ�Ƿ����Ҫ��
	do {
		cout << "Please input Work Path:" << endl;
		std::cin >> WorkPath;
	} while (!CheckFolderValidity(WorkPath));
	STDCOUTGREEN;
	cout << "����֤���ļ�����Ч��" << endl;
	STDCOUTWHITE;
Mark:
	cout << "��ѡ����Ҫ���еĹ�����" << endl;
	STDCOUTBLUE;
	cout << "��1��.���ƿ��ӻ���༭" << endl;
	cout << "��2��.����˲���" << endl;
	cout << "��3��.�������������ݿ�" << endl;
	cout << "��4��.��������" << endl;
	cout << "��5��.�ϲ���������" << endl;
	cout << "��6��.�������" << endl;
	cout << "��7��.������" << endl;
	cout << "��8��.�ָ�RGB" << endl;
	cout << "��9��.���ֽ���" << endl;
	cout << "��10��.���ݼ�����" << endl;
	cout << "��11��.���ݼ��޸�" << endl;
	cout << "��12��.���ݸ�ʽת��" << endl;
	cout << "��13��.����TXT�ļ�" << endl;
	cout << "��14��.Ԥ���ǩ���" << endl;
	cout << "��15��.���㷨���������ӻ�" << endl;
	cout << "��16��.�鿴points�ṹ" << endl;
	cout << "��17��.���Լ�Ԥ��" << endl;
	cout << "��18��.���ݿ���������Ϊ�ļ�" << endl;
	cout << "��19��.����" << endl;
	cout << "��20��.�޳���Ⱥ��" << endl;
	cout << "��21��.��������ͳ��" << endl;
	STDCOUTWHITE;
	int function_int;
	std::cin >> function_int;

	switch (function_int)
	{
	case 1:VisualizationData(); goto Mark;
	case 2:Caculate(); goto Mark;
	case 3:saveInfoToDB(); goto Mark;
	case 4:CBA(); goto Mark;
	case 5:CombinePointCloud(); goto Mark;
	case 6:NormalizedClassification(); goto Mark;
	case 7:Triangulation(); goto Mark; 
	case 8:restoreRGB(); goto Mark;
	case 9:DistinguishingBoundary(); goto Mark;
	case 10:GenerateData(); goto Mark;
	case 11:dataRepair(); goto Mark;
	case 12:TransformPointformat(); goto Mark;
	case 13:buildTXT(); goto Mark;
	case 14:labelSaveToDB(); goto Mark;
	case 15:normalVisualization(); goto Mark;
	case 16:readPoints(); goto Mark;
	case 17:testLabelSaveToDB(); goto Mark;
	case 18:DBSaveToFile(); goto Mark;
	case 19:removeGlass(); goto Mark;
	case 20:removeoOutlier(); goto Mark;
	case 21:TestAccAfterpPurification(); goto Mark;
	default:
		goto Mark;
	}
	return 0;
}