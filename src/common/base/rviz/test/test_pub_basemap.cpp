#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "publish_base_map.h"




// %Tag(main)%
int main(int argc, char** argv)
{
	ros::init(argc, argv, "test_publish_basemap");
	ros::NodeHandle n;




	ros::NodeHandle nh;
	ros::Publisher pub_base_map_ = nh.advertise<MapPointCloudRGB> ( "test_basemap", 1, true );

	ros::Rate rate(10);
	PublishBasemap publish_image;
	publish_image.Init("/home/idriver/avos2_0/restruct_avos2.0/src/map/basemap/map/tile_map/");
	double xg = 434250, yg = 4446250;
	int index = 0;
	while (ros::ok()) {
		double now_x, now_y;
		now_x = xg + index;
		now_y = yg + index;

		index++;
		if (index > 100) index = 0;
		publish_image.PublishBaseMap(pub_base_map_, now_x, now_y, 50);
		ros::spinOnce();
		rate.sleep();
	}


}