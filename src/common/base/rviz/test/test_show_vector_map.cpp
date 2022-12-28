

#include <iostream>
#include <string>

#include "show_wxb_vector_map.h"
#include "publish_pointcloud.h"




float PathGetGFunc(void* param) {
	ShowMapPoint *point = (ShowMapPoint *)param;
	if (point->id == 0) {
		return 1.0;
	}
	return 1.0;
}

float PathGetRFunc(void* param) {
	ShowMapPoint *point = (ShowMapPoint *)param;
	if (point->id == 0) {
		return 0.0;
	}
	return 1.0;
}

float PathGetBFunc(void* param) {
	ShowMapPoint *point = (ShowMapPoint *)param;
	if (point->id == 0) {
		return 0.0;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	/* code */
	ros::init(argc, argv, "test_read_vector_map");
	ros::NodeHandle nh;
	ros::Publisher pub_pointcloud_vector_map = nh.advertise<PathplanPointCloud> ( "read_vector_map", 1, true );

	ObjectPointCloud<std::vector<ShowMapPoint>> vector_map_points;

	ShowWXBVectorMap show_wxb_vector_map;
	show_wxb_vector_map.LoadVectorMap("vectormap.xml");
	vector_map_points.inner_points.clear();

	for (auto& p : show_wxb_vector_map.vector_map_points_) {
		if (p.first == "boundaries") {
			for (int j = 0; j < p.second.size(); j++) {
				p.second[j].id = 0;
				vector_map_points.inner_points.push_back(p.second[j]);
			}
		}

		if (p.first == "roads") {
			for (int j = 0; j < p.second.size(); j++) {
				p.second[j].id = 1;
				vector_map_points.inner_points.push_back(p.second[j]);
			}
		}

		if (p.first == "surfaces") {
			for (int j = 0; j < p.second.size(); j++) {
				p.second[j].id = 2;
				// vector_map_points.inner_points.push_back(p.second[j]);
			}
		}

		if (p.first == "regions") {
			for (int j = 0; j < p.second.size(); j++) {
				p.second[j].id = 3;
				// vector_map_points.inner_points.push_back(p.second[j]);
			}
		}
	}

	vector_map_points.SetGFunc(PathGetGFunc);
	vector_map_points.SetRFunc(PathGetRFunc);
	vector_map_points.SetBFunc(PathGetBFunc);
	std::cout << "size():" << vector_map_points.inner_points.size() <<  std::endl;
	while (ros::ok()) {
		vector_map_points.publish(pub_pointcloud_vector_map, 'n');
		sleep(1);
	}
	return 0;
}