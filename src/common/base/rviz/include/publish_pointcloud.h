#ifndef __PUBLISH_POINTCLOUD_H__
#define __PUBLISH_POINTCLOUD_H__



#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <geometry_msgs/Point32.h>
#include <vector>

#include "publish_base_header.h"

typedef pcl::PointCloud<pcl::PointXYZRGBA> PathplanPointCloud;

template <class T>
struct ObjectPointCloud
{
	PathplanPointCloud::Ptr publish_points;
	T inner_points;
	pColorFunction pget_g_func;
	pColorFunction pget_r_func;
	pColorFunction pget_b_func;
	float DefaultGetGFunc(void*) {
		return 1.0;
	}

	float DefaultGetRFunc(void*) {
		return 1.0;
	}

	float DefaultGetBFunc(void*) {
		return 0.0;
	}
	ObjectPointCloud() {
		publish_points.reset( new PathplanPointCloud());
		publish_points->header.frame_id = "base_link";
		publish_points->height =  1;
		publish_points->width = 0;
		publish_points->points.clear();
		inner_points.clear();


		pget_g_func = std::bind(&ObjectPointCloud::DefaultGetGFunc, this, std::placeholders::_1);
		pget_r_func = std::bind(&ObjectPointCloud::DefaultGetRFunc, this, std::placeholders::_1);
		pget_b_func = std::bind(&ObjectPointCloud::DefaultGetBFunc, this, std::placeholders::_1);
	}
	void Reset() {
		publish_points->width = 0;
		publish_points->points.clear();
		inner_points.clear();
	}
	void SetGFunc(pColorFunction usr_get_g_func) {
		pget_g_func = std::move(usr_get_g_func);
	}
	void SetRFunc(pColorFunction usr_get_r_func) {
		pget_r_func = std::move(usr_get_r_func);
	}
	void SetBFunc(pColorFunction usr_get_b_func) {
		pget_b_func = std::move(usr_get_b_func);
	}
	void publish(ros::Publisher &pub, const char &color = 'y') {
		if (pub.getNumSubscribers() == 0 )  return;
		publish_points->width = 0;
		publish_points->points.clear();
		for (auto &ip : inner_points) {
			publish_points->width++;
			pcl::PointXYZRGBA temp;
			temp.x = ip.x;
			temp.y = ip.y;
			temp.z = 0;


			switch (color) {
			case 'y':
				temp.b = 0;
				temp.g = 255;
				temp.r = 255;
				break;
			case 'r':
				temp.b = 0;
				temp.g = 0;
				temp.r = 255;
				break;
			case 'b':
				temp.b = 255;
				temp.g = 0;
				temp.r = 0;
				break;
			case 'g':
				temp.b = 0;
				temp.g = 255;
				temp.r = 0;
				break;
			case 'p':
				temp.b = 128;
				temp.g = 0;
				temp.r = 128;
				break;
			case 'w':
				temp.b = 255;
				temp.g = 255;
				temp.r = 255;
				break;
			default:
				temp.b = pget_b_func((void *)&ip) * 255;
				temp.g = pget_g_func((void *)&ip) * 255;
				temp.r = pget_r_func((void *)&ip) * 255;
			}


			temp.a = 128;
			publish_points->points.push_back(temp);
		}

		pub.publish(publish_points);
	}

};












#endif