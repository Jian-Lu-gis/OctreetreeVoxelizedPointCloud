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
	// 验证文件夹结构是否符合要求
	do {
		cout << "Please input Work Path:" << endl;
		std::cin >> WorkPath;
	} while (!CheckFolderValidity(WorkPath));
	STDCOUTGREEN;
	cout << "经验证，文件夹有效！" << endl;
	STDCOUTWHITE;
Mark:
	cout << "请选择想要进行的工作：" << endl;
	STDCOUTBLUE;
	cout << "【1】.点云可视化与编辑" << endl;
	cout << "【2】.计算八叉树" << endl;
	cout << "【3】.案例保存至数据库" << endl;
	cout << "【4】.案例推理" << endl;
	cout << "【5】.合并点云数据" << endl;
	cout << "【6】.纯化类别" << endl;
	cout << "【7】.三角网" << endl;
	cout << "【8】.恢复RGB" << endl;
	cout << "【9】.划分界限" << endl;
	cout << "【10】.数据集生成" << endl;
	cout << "【11】.数据集修复" << endl;
	cout << "【12】.数据格式转换" << endl;
	cout << "【13】.构建TXT文件" << endl;
	cout << "【14】.预测标签入库" << endl;
	cout << "【15】.计算法向量并可视化" << endl;
	cout << "【16】.查看points结构" << endl;
	cout << "【17】.测试集预测" << endl;
	cout << "【18】.数据库样本保存为文件" << endl;
	cout << "【19】.除草" << endl;
	cout << "【20】.剔除离群点" << endl;
	cout << "【21】.纯化精度统计" << endl;
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