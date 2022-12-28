#ifndef __PUBLISH_POINTCLOUD_H__
#define __PUBLISH_POINTCLOUD_H__

#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <geometry_msgs/Point32.h>
#include <vector>

typedef pcl::PointCloud<pcl::PointXYZRGBA> PathplanPointCloud;

struct StationPointStruct {
	int point_id;
	double x;
	double y;
	bool whether_stop;
	std::string belong_lane_id;

	StationPointStruct() {
		reset();
	}
	void reset() {
		point_id = 0;
		x = 0.0;
		y = 0.0;
		whether_stop = false;
	}
};

template <class T>
struct ObjectPointCloud
{
	// PathplanPointCloud publish_points;
	PathplanPointCloud::Ptr publish_points;
	bool is_path;
	T inner_points;
	ObjectPointCloud() {
		publish_points.reset( new PathplanPointCloud());
		publish_points->header.frame_id = "base_link";
		publish_points->height =  1;
		publish_points->width = 0;
		publish_points->points.clear();
		inner_points.clear();
		is_path = false;
	}
	void Reset() {
		publish_points->width = 0;
		publish_points->points.clear();
		inner_points.clear();
	}
	void publish(std::string frame_id, ros::Publisher &pub, const char &color = 'y') {
		// Reset();
		publish_points->header.frame_id = frame_id;
		publish_points->width = 0;
		publish_points->points.clear();
		for (auto &ip : inner_points) {
			publish_points->width++;
			pcl::PointXYZRGBA temp;
			temp.x = ip.xg;
			temp.y = ip.yg;
			// std::cout << "x|y" << temp.x << "|" << temp.y << std::endl;
			temp.z = 0;

			if (is_path) {
				temp.b = 0;
				temp.g = 0;
				temp.r = 255;
			} else {
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
				default:
					temp.b = 0;
					temp.g = 255;
					temp.r = 255;
				}
			}

			temp.a = 128;
			publish_points->points.push_back(temp);
		}
		// std::cout << "publish_points->points " << publish_points->points.size() << std::endl;
		pub.publish(publish_points);
	}

};

#endif