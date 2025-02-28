// PCL_object_vector.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <pcl/point_types.h>
#include <pcl/io/pcd_io.h>
#include <pcl/kdtree/kdtree_flann.h>
#include <pcl/features/normal_3d.h>
#include <pcl/surface/gp3.h>
#include <pcl/visualization/pcl_visualizer.h>


int main()
{
	//加载点云模型
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

	if (pcl::io::loadPCDFile<pcl::PointXYZ>("D:\\project\\PCL_vector\\PCL_object_vector\\piece_voxel.pcd", *cloud) == -1)
	{
		PCL_ERROR("Could not read file\n");
	}

	//计算法线
	pcl::NormalEstimation<pcl::PointXYZ, pcl::Normal> n;
	pcl::PointCloud<pcl::Normal>::Ptr normals(new pcl::PointCloud<pcl::Normal>);
	//建立kdtree来进行近邻点集搜索
	pcl::search::KdTree<pcl::PointXYZ>::Ptr tree(new pcl::search::KdTree<pcl::PointXYZ>);
	//为kdtree添加点运数据
	tree->setInputCloud(cloud);
	n.setInputCloud(cloud);
	n.setSearchMethod(tree);
	//点云法向计算时，需要所搜的近邻点大小
	n.setKSearch(20);
	//开始进行法向计算
	n.compute(*normals);

	/*图形显示模块*/
	boost::shared_ptr<pcl::visualization::PCLVisualizer> viewer(new pcl::visualization::PCLVisualizer("3D viewer"));
	//viewer->initCameraParameters();

	int v1(0), v2(1), v3(2), v4(3);
	viewer->createViewPort(0.0, 0.0, 0.5, 0.5, v1);
	viewer->createViewPort(0.5, 0.0, 1.0, 0.5, v2);
	viewer->createViewPort(0.0, 0.5, 0.5, 1.0, v3);
	viewer->createViewPort(0.5, 0.5, 1.0, 1.0, v4);

	//设置背景颜色
	viewer->setBackgroundColor(5, 55, 10, v1);
	//设置点云颜色
	pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> single_color(cloud, 0, 225, 0);
	//添加坐标系
	viewer->addCoordinateSystem(0.5,  "reference", v1);

	viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud", v1);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud0", v2);
	viewer->addPointCloudNormals<pcl::PointXYZ, pcl::Normal>(cloud, normals, 50, 0.01, "normals", v2);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, single_color, "sample cloud1", v3);
	viewer->addPointCloud<pcl::PointXYZ>(cloud, "sample cloud3", v4);

	//设置点云大小
	viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 2, "sample cloud3", 4);


	//添加点云法向量的另一种方式；
	//解决来源http://www.pcl-users.org/How-to-add-PointNormal-on-PCLVisualizer-td4037041.html
	//pcl::PointCloud<pcl::PointNormal>::Ptr cloud_with_normals(new pcl::PointCloud<pcl::PointNormal>);
	//pcl::concatenateFields(*cloud, *normals, *cloud_with_normals);
	//viewer->addPointCloudNormals<pcl::PointNormal>(cloud_with_normals, 50, 0.01, "normals");
	//
	

	return 0;
}

