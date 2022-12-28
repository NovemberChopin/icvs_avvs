#ifndef __PUBLISH_BASE_MAP_H__
#define __PUBLISH_BASE_MAP_H__

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <iomanip>
#include <unistd.h>
#include <pthread.h>

#include <pcl_ros/point_cloud.h>
#include <pcl/point_types.h>
#include <pcl_conversions/pcl_conversions.h>
#include <geometry_msgs/Point32.h>
#include <vector>

#include "map/vectormap/include/geotool.h"
using namespace avos::vectormap;
using namespace std;
using namespace cv;
#define PIXEL_DIS  0.05
typedef pcl::PointCloud<pcl::PointXYZRGBA> MapPointCloudRGB;

typedef struct ColorRGB
{
	unsigned char r;
	unsigned char b;
	unsigned char g;
} ColorRGB;

typedef struct PositionColor
{
	PointGCCS gccs;
	ColorRGB color;
} PositionColor;


typedef struct ImagePointcloud
{
	int times;
	int x_index;
	int y_index;
	std::vector<PositionColor> position_color_vec;
	ImagePointcloud() {
		times = 0;
		position_color_vec.clear();
	}

	std::string getMapFileName(const std::string &image_path) {
		stringstream ss;
		ss << image_path << y_index << "_" << x_index << "_50.image";
		return ss.str();
	}

	bool isFileExist(const string file_name) {
		if (file_name == "")
			return false;
		if (access(file_name.c_str(), F_OK) == 0)
			return true;
		return false;
	}

	bool GetPointCloud(const std::string &image_path, double self_xg, double self_yg, double self_heading) {
		std::string file_name = getMapFileName(image_path);
		if (isFileExist(file_name) == false) {
			return false;
		}
		Mat image = imread(file_name);
		position_color_vec.clear();

		PositionColor position_color;
		int pixel_num = 2;
		for (int i = pixel_num; i < image.rows - pixel_num; i++) {
			if (i - pixel_num < 0 || i + pixel_num >= image.rows)
				continue;
			for (int j = pixel_num; j < image.cols - pixel_num; j++) {
				if ( j - pixel_num < 0 || j + pixel_num >= image.cols)
					continue;

				if (image.at<Vec3b>(i, j)[0] == 0 && image.at<Vec3b>(i, j)[1] == 0 && image.at<Vec3b>(i, j)[2] == 0 )
					continue;
				if (image.at<Vec3b>(i, j)[0] == image.at<Vec3b>(i + pixel_num, j)[0] &&
				        image.at<Vec3b>(i, j)[1] == image.at<Vec3b>(i + pixel_num, j)[1] &&
				        image.at<Vec3b>(i, j)[2] == image.at<Vec3b>(i + pixel_num, j)[2] &&
				        image.at<Vec3b>(i, j)[0] == image.at<Vec3b>(i - pixel_num, j)[0] &&
				        image.at<Vec3b>(i, j)[1] == image.at<Vec3b>(i - pixel_num, j)[1] &&
				        image.at<Vec3b>(i, j)[2] == image.at<Vec3b>(i - pixel_num, j)[2] &&
				        image.at<Vec3b>(i, j)[0] == image.at<Vec3b>(i , j + pixel_num)[0] &&
				        image.at<Vec3b>(i, j)[1] == image.at<Vec3b>(i , j + pixel_num)[1] &&
				        image.at<Vec3b>(i, j)[2] == image.at<Vec3b>(i , j + pixel_num)[2] &&
				        image.at<Vec3b>(i, j)[0] == image.at<Vec3b>(i , j - pixel_num)[0] &&
				        image.at<Vec3b>(i, j)[1] == image.at<Vec3b>(i , j - pixel_num)[1] &&
				        image.at<Vec3b>(i, j)[2] == image.at<Vec3b>(i , j - pixel_num)[2]) {
					continue;
				}

				position_color.gccs.xg =  (double)x_index * 50  + j * PIXEL_DIS;
				position_color.gccs.yg =  (double)y_index * 50 + (image.rows - 1 - i) * PIXEL_DIS;

				position_color.color.b = image.at<Vec3b>(i, j)[0];
				position_color.color.g = image.at<Vec3b>(i, j)[1];
				position_color.color.r = image.at<Vec3b>(i, j)[2];
				position_color_vec.push_back(position_color);
			}
		}
		return true;
	}
} ImagePointcloud, *pImagePointcloud;


class PublishBasemap
{
public:
	PublishBasemap() {
		show_map_max_dis_ = 100;

	}
	~PublishBasemap() {}

	void Init(const std::string &path) {
		map_path_ = path;
	}

	void Publish(std::string frame_id, ros::Publisher &pub, const double xg, const double yg, const double heading) {
		if (pub.getNumSubscribers() == 0 )  return;
		int self_x_index = xg / 50;
		int self_y_index = yg / 50;
		for (int i = -6; i < 6; i++) {
			for (int j = -6; j < 6; j++) {
				int target_x_index = self_x_index + i;
				int target_y_index = self_y_index + j;
				if (isFileExist(target_x_index, target_y_index) == false) {
					continue;
				}
				if (IsInImageCache(target_x_index, target_y_index)) {
					continue;
				}
				ImagePointcloud image_point_cloud;
				image_point_cloud.x_index = target_x_index;
				image_point_cloud.y_index = target_y_index;
				image_point_cloud.GetPointCloud(map_path_, xg, yg, heading);
				image_index_cache.push_back(image_point_cloud);
				if (image_index_cache.size() > 60) {
					image_index_cache.erase(image_index_cache.begin());
				}
			}
		}

		GeoTool geo_tool_;
		PointGCCS target_gccs, self_gccs;
		PointVCS pointvcs;
		self_gccs.xg = xg;
		self_gccs.yg = yg;
		self_gccs.angle = heading;

		pub_image_point_ptr.reset(new MapPointCloudRGB);
		pub_image_point_ptr->header.frame_id = frame_id;
		pub_image_point_ptr->height =  1;
		pub_image_point_ptr->width = 0;
		pub_image_point_ptr->points.clear();
		for (unsigned int i = 0; i < image_index_cache.size(); i++) {
			for (unsigned int j = 0; j < image_index_cache[i].position_color_vec.size(); j++) {
				pub_image_point_ptr->width++;
				pcl::PointXYZRGBA temp;
				geo_tool_.GCCS2VCS(self_gccs, image_index_cache[i].position_color_vec[j].gccs, pointvcs);
				temp.x = pointvcs.x;
				temp.y = pointvcs.y;
				temp.z = 0;
				temp.b = image_index_cache[i].position_color_vec[j].color.b;
				temp.g = image_index_cache[i].position_color_vec[j].color.g;
				temp.r = image_index_cache[i].position_color_vec[j].color.r;
				temp.a = 255;
				pub_image_point_ptr->points.push_back(temp);
			}
		}
		pub.publish(pub_image_point_ptr);
	}

private:
	bool isFileExist(int x, int y) {
		stringstream ss;
		ss << map_path_ << y << "_" << x << "_50.image";
		return isFileExist(ss.str());
	}

	bool isFileExist(const string file_name) {
		if (file_name == "")
			return false;
		if (access(file_name.c_str(), F_OK) == 0)
			return true;
		return false;
	}

	bool IsInImageCache(int x, int y) {
		// std::cout << "image_index_cache:" << image_index_cache.size() << std::endl;
		for (unsigned int i = 0; i < image_index_cache.size(); i++) {
			// std::cout << "cache_x |x :" << image_index_cache[i].x_index << "|" << x << std::endl;
			// std::cout << "cache_y | y:" << image_index_cache[i].y_index << "|" << y << std::endl;
			if (image_index_cache[i].x_index == x && image_index_cache[i].y_index == y) {
				return true;
			}
		}
		return false;
	}

private:
	std::string map_path_;
	double show_map_max_dis_;
	std::vector<ImagePointcloud> image_index_cache;
	MapPointCloudRGB::Ptr pub_image_point_ptr;
};


#endif
