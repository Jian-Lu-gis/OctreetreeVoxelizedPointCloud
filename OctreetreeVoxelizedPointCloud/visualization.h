#pragma once
#include "common.h"

template <typename PointT>
struct callback_args {
	// structure used to pass arguments to the callback function  
	typename pcl::PointCloud<PointT>::Ptr cloud_xyzi;
	pcl::visualization::PCLVisualizer::Ptr viewerPtr;
	pcl::RGB RGBArray[10];
	int classnumber;
	int num;
	int selectcount;
};
// �����¼�
template <typename PointT>
void keyboardEventOccurred(const pcl::visualization::KeyboardEvent& event, void* args)
{
	callback_args<PointT>* data = (callback_args<PointT>*)args;
	if (std::is_same<PointT,PointType>::value && event.getKeySym() == "p" && event.keyDown()) {
		pcl::visualization::Camera cameraParameter;
		int p = 0;
		cout << "�������ӽǲ�����" << endl;
		cin >> p;
		if (p == 1) {
			data->viewerPtr->loadCameraParameters("screenshot.cam");
		}
		if (p == 2) {
			data->viewerPtr->loadCameraParameters("screenshot1.cam");
		}
		if (p == 3) {
			data->viewerPtr->loadCameraParameters("screenshot2.cam");
		}
		//cout << cameraParameter;
	}
	if (std::is_same<PointT,PointType>::value && event.getKeySym() == "x" && event.keyDown()) {
		data->num++;
		if (data->num % 2 == 1) {
			cout << "���" << endl;
			cin >> data->classnumber;
			
			cout << "��ʼѡ�������:" << data->classnumber << endl;
		}
		else {
			cout << "��� " << data->classnumber << " ѡ�����" << endl;
			if (data->selectcount == 0) {
				STDCOUTYELLOW;
				cout << "��ѡ����0���㣬�������м�¼��" << endl;
				STDCOUTWHITE;
				//data->classnumber--;
			}
			data->selectcount = 0;
		}
	}
	else if (std::is_same<PointT, PointType>::value && event.getKeySym() == "space" && event.keyDown()) {
		cout << "�������ļ�����" << endl;
		std::string name;
		cin >> name;
		cout << "���ڱ���ѡ�������ԭʼ��������..." << endl;
		pcl::io::savePCDFileBinary(WorkPath + "\\SelectedData\\" + name + ".pcd", *data->cloud_xyzi);
		STDCOUTGREEN;
		std::cout << "����򻯺��ԭʼ�������ݳɹ���" << std::endl;
		STDCOUTWHITE;

	}
	else if (event.getKeySym() == "d" && event.keyDown()) {
		std::cout << "�����������˲��ֶε����" << std::endl;
		STDCOUTBLUE;
		std::cout << "��1��" << "ContainsPointsSize" << std::endl;
		std::cout << "��2��" << "Classification" << std::endl;
		std::cout << "��3��" << "MaximumSimilarity" << std::endl;
		std::cout << "��4��" << "AverageReflectionIntensity" << std::endl;
		std::cout << "��5��" << "StatisticalOutlierRemoval" << std::endl;
		std::cout << "��6��" << "Roughness" << std::endl;
		std::cout << "��7��" << "BanClassification" << std::endl;
		STDCOUTWHITE;
		int FiedName;  // �ֶ�����
		std::cin >> FiedName;
		std::cout << "�����������˲���ֵ��" << std::endl;
		double value;  // ֵ
		std::cin >> value;
		typename pcl::PointCloud<PointT>::Ptr cloud_filtered(new pcl::PointCloud<PointT>);
		typename pcl::ConditionBase<PointT>::Ptr range_cond;
		pcl::ConditionalRemoval<PointT> condrem;
		// build the condition
		if (std::is_same<PointT, MyPointType>::value && FiedName == 1) { // ���ð������Ƹ����˲�
			typename pcl::ConditionAnd<PointT>::Ptr range_and(new pcl::ConditionAnd<PointT>());
			range_and->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("ContainsPointsSize", pcl::ComparisonOps::GT, value)));
			range_cond = range_and;
		}
		else if (FiedName == 2) {
			typename pcl::ConditionOr<PointT>::Ptr range_or(new pcl::ConditionOr<PointT>());
			range_or->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("Classification", pcl::ComparisonOps::EQ, value)));
			range_cond = range_or;
		}
		else if (FiedName == 3) {
			typename pcl::ConditionAnd<PointT>::Ptr range_and(new pcl::ConditionAnd<PointT>());
			range_and->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("MaximumSimilarity", pcl::ComparisonOps::GT, value)));
			range_cond = range_and;
		}
		else if (FiedName == 4) {
			typename pcl::ConditionAnd<PointT>::Ptr range_and(new pcl::ConditionAnd<PointT>());
			range_and->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("AverageReflectionIntensity", pcl::ComparisonOps::GT, value)));
			range_cond = range_and;
		}
		else if (FiedName == 5) {
			pcl::StatisticalOutlierRemoval<PointT> sor;
			sor.setInputCloud(data->cloud_xyzi);
			sor.setMeanK(value);
			sor.setStddevMulThresh(1.0);
			sor.filter(*cloud_filtered);
			goto MarkF;
		}
		else if (FiedName == 6) {
			std::cout << "�����������˲���Сֵ��" << std::endl;
			double minvalue;  // ֵ
			std::cin >> minvalue;
			std::cout << "�����������˲��Ĵ�ֵ��" << std::endl;
			double maxvalue;  // ֵ
			std::cin >> maxvalue;
			typename pcl::ConditionAnd<PointT>::Ptr range_and(new pcl::ConditionAnd<PointT>());
			range_and->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("Roughness", pcl::ComparisonOps::GT, minvalue)));
			range_and->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("Roughness", pcl::ComparisonOps::LT, maxvalue)));
			range_cond = range_and;
		}
		else if (FiedName == 7) {
			typename pcl::ConditionOr<PointT>::Ptr range_or(new pcl::ConditionOr<PointT>());
			range_or->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("Classification", pcl::ComparisonOps::GT, value)));
			range_or->addComparison(pcl::FieldComparison<PointT>::ConstPtr(new pcl::FieldComparison<PointT>("Classification", pcl::ComparisonOps::LT, value)));
			range_cond = range_or;
		}
		else {
			return;
		}
		// build the filter
		condrem.setCondition(range_cond);
		condrem.setInputCloud(data->cloud_xyzi);
		condrem.setKeepOrganized(false);
		condrem.filter(*cloud_filtered);
	MarkF:
		pcl::visualization::PointCloudColorHandlerRGBField<PointT> red(cloud_filtered);
		data->viewerPtr->removePointCloud("intensity");
		data->viewerPtr->addPointCloud(cloud_filtered, red, "intensity");
		cout << "�Ƿ���Ҫ�������ļ���Y/N" << endl;
		std::string ifsave;
		cin >> ifsave;
		if (ifsave == "Y") {
			cout << "�������ļ�����" << endl;
			std::string filename;
			cin >> filename;
			cout << "���ڱ����˲����ԭʼ�������ݣ�" << filename << ".pcd" << endl;
			pcl::io::savePCDFileBinary(WorkPath + "\\Filtering\\" + filename + ".pcd", *cloud_filtered);
			STDCOUTGREEN;
			std::cout << "�����˲����ԭʼ�������ݳɹ���" << std::endl;
			STDCOUTWHITE;
		}
	}
}
// ����ѡ�¼�
template <typename PointT>
void pp_callback(const pcl::visualization::AreaPickingEvent& event, void* args)
{
	pcl::PointCloud<PointType>::Ptr clicked_points_3d(new pcl::PointCloud<PointType>);
	callback_args<PointT>* data = (callback_args<PointT>*)args;
	if (data->classnumber != -1) {
		std::vector< int > indices;
		if (event.getPointsIndices(indices) == -1)
			return;
		data->selectcount = indices.size();
		/*cout << "�����״" << endl;
		float tendency = 220.03, inclination = 3.618;
		std::string isAllCheckdata = "N";
		cout << "����(0-360)��" << endl;
		cin >> tendency;
		cout << "���(0-90)��" << endl;
		cin >> inclination;*/
		for (int i = 0; i < indices.size(); ++i)
		{
			clicked_points_3d->points.push_back(data->cloud_xyzi->points.at(indices[i]));
			data->cloud_xyzi->points[indices[i]].Classification = data->classnumber;
			/*data->cloud_xyzi->points[indices[i]].tendency = 62;
			data->cloud_xyzi->points[indices[i]].inclination = 33;*/
		}
		pcl::visualization::PointCloudColorHandlerCustom<PointT> red(clicked_points_3d, data->RGBArray[data->classnumber - 1].r,
			data->RGBArray[data->classnumber].g, data->RGBArray[data->classnumber].b);
		char id[8];
		_itoa(rand(), id, 10);
		data->viewerPtr->addPointCloud(clicked_points_3d, red, id);
	}
}
// ��굥ѡ�¼�
void pp_single_MyPointType(const pcl::visualization::PointPickingEvent& event, void* args)
{
	callback_args<PointType>* data = (callback_args<PointType>*)args;
	int index = event.getPointIndex();
	if (index == -1)
		return;

	std::cout << "id:" << index << endl;
	std::cout << "xyz:" << data->cloud_xyzi->points[index].x << " " << data->cloud_xyzi->points[index].y << " " << data->cloud_xyzi->points[index].z << std::endl;

	std::cout << "Classification:" << data->cloud_xyzi->points[index].Classification << endl;
	//std::cout << "AverageReflectionIntensity:" << data->cloud_xyzi->points[index].AverageReflectionIntensity << endl;
	//std::cout << "Roughness:" << data->cloud_xyzi->points[index].Roughness << endl;
	//std::cout << "isCheckData:" << data->cloud_xyzi->points[index].ischeckdata << endl;
	//// ��ʾֱ��ͼ
	//pcl::visualization::PCLPlotter plotter;
	//plotter.addFeatureHistogram<MyPointType>(*data->cloud_xyzi, "pfh", index);
	/*while (!plotter.wasStopped())
	{
		plotter.plot();
		boost::this_thread::sleep(boost::posix_time::microseconds(100000));
	}*/
}

// ���ӻ�
template <typename PointT>
void Visualization(boost::shared_ptr<pcl::PointCloud<PointT> > cloud, boost::shared_ptr<pcl::PointCloud<PointT> > comparecloud = NULL, bool isRegisterCallback = true,std::string selecttrueclassification="no",bool rawdataShow=false) {
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("PointCloudShow"));
	viewer->initCameraParameters();   //��ʼ���������
	if (comparecloud != NULL)
	{
		int v0(0);
		viewer->createViewPort(0.0, 0.0, 0.5, 1.0, v0);
		// ʹ���ֶθ�����ɫ
		pcl::visualization::PointCloudColorHandlerRGBField <PointT> intensity_distribution(cloud);
		viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity", v0);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity", v0);
		viewer->setBackgroundColor(0.0, 0.0, 0.0, v0); //���ñ���ɫΪ��ɫ
		int v1(1);
		viewer->createViewPort(0.5, 0.0, 1.0, 1.0, v1);
		pcl::visualization::PointCloudColorHandlerRGBField <PointT> intensity_distribution_compare(comparecloud);
		viewer->addPointCloud<PointT>(comparecloud, intensity_distribution_compare, "comparecloud", v1);
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "comparecloud", v1);
		viewer->setBackgroundColor(0.0, 0.0, 0.0, v1); //���ñ���ɫΪ��ɫ
	}
	else {
		//�˴�����rawdatashow�ж���Ϊ����ʾ����PointT�������Զ������Ͳ�ͬʱ����������ѡ��ֻ������1
		if (std::is_same<PointT,PointType>::value||rawdataShow==true) {
		Mark:
			cout << "��ѡ����ɫ�ֶΣ�" << endl;
			STDCOUTBLUE;
			cout << "��1��.RGB" << endl;
			cout << "��2��.trueClassification" << endl;
			cout << "��3��.Roughness" << endl;
			cout << "��4��.Classification" << endl;
			cout << "��5��.rvalue" << endl;
			cout << "��6��.gvalue" << endl;
			cout << "��7��.bvalue" << endl;
			STDCOUTWHITE;
			int function_int;
			std::cin >> function_int;

			switch (function_int)
			{
			case 1: {
				pcl::visualization::PointCloudColorHandlerRGBField <PointT> intensity_distribution(cloud);
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 2: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "trueClassification");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 3: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "Roughness");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 4: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "Classification");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 5: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "r");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 6: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "g");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			case 7: {
				pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "b");
				viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
				viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
				viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
			}; break;
			default:
				goto Mark;
			}
		}
		else {
			pcl::visualization::PointCloudColorHandlerGenericField <PointT> intensity_distribution(cloud, "Classification");
			viewer->addPointCloud<PointT>(cloud, intensity_distribution, "intensity");
			viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, "intensity");
			viewer->setBackgroundColor(0.0, 0.0, 0.0); //���ñ���ɫΪ��ɫ
		}
	}

	if (std::is_same<PointT, PointType>::value &&isRegisterCallback) { // ֻ��������ΪMyPointTypeʱ��ע��ص�����
						  // �������
		callback_args<PointT> cb_args;
		cb_args.cloud_xyzi = cloud;
		cb_args.viewerPtr = pcl::visualization::PCLVisualizer::Ptr(viewer);
		cb_args.classnumber = 0;
		cb_args.num = 0;
		cb_args.selectcount = 0;
		for (size_t i = 0; i < 10; i++)
		{
			cb_args.RGBArray[i].r = rand() % (255 - 0 + 1) + 0;
			cb_args.RGBArray[i].g = rand() % (255 - 0 + 1) + 0;
			cb_args.RGBArray[i].b = rand() % (255 - 0 + 1) + 0;
		}
		viewer->registerAreaPickingCallback(pp_callback<PointType>, (void*)&cb_args); //ע������ѡ�¼��ص�
		viewer->registerKeyboardCallback(keyboardEventOccurred<PointType>, (void*)&cb_args); //ע������¼��ص�
		/*viewer->registerPointPickingCallback(pp_single_MyPointType, (void*)&cb_args);*/
	}

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
