#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "publish_cube.h"


#define Random(x) (rand() % x)
typedef struct Object
{
	double x;
	double y;
	double width;
	double length;
	double height;
} Object;


// %Tag(main)%
int main(int argc, char** argv)
{
	ros::init(argc, argv, "test_publish_cube");
	ros::NodeHandle n;




	ros::NodeHandle nh;
	ros::Publisher pub_pointcloud_ = nh.advertise<visualization_msgs::MarkerArray> ( "test_cube", 1, true );

	PublishCube<std::vector<Object>> publish_cube;
	ros::Rate rate(10);
	std::vector<Object> object_vec;
	object_vec.reserve(1000);

	while (ros::ok()) {
		object_vec.clear();
		for (int i = 0; i < Random(100); i++) {
			Object obj;
			obj.x = i;
			obj.y = 0;
			obj.width = 1;
			obj.length = 1;
			obj.height = 1;
			object_vec.push_back(obj);
		}

		publish_cube.inner_cubes = object_vec;
		publish_cube.publish(pub_pointcloud_, 'y');
		ros::spinOnce();
		rate.sleep();
	}


}