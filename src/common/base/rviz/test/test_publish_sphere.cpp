#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "publish_sphere.h"


#define Random(x) (rand() % x)
typedef struct Object
{
	double x;
	double y;
} Object;


float ObjectGetGFunc(void* param) {
	Object *object = (Object *)param;
	float g = object->y / 30.0;
	// std::cout << "ObjectGetGFunc " << g << std::endl;
	if (g < 1) return g;
	else return 1.0;
}

float ObjectGetRFunc(void*) {
	return 0.0;
}

float ObjectGetBFunc(void*) {
	return 0.0;
}

// %Tag(main)%
int main(int argc, char** argv)
{
	ros::init(argc, argv, "test_publish_sphere");
	ros::NodeHandle n;




	ros::NodeHandle nh;
	ros::Publisher pub_pointcloud_ = nh.advertise<visualization_msgs::MarkerArray> ( "test_sphere", 1, true );

	PublishSphere<std::vector<Object>> publish_sphere;
	ros::Rate rate(10);
	std::vector<Object> object_vec;
	object_vec.reserve(1000);
	publish_sphere.SetGFunc(ObjectGetGFunc);
	publish_sphere.SetRFunc(ObjectGetRFunc);
	publish_sphere.SetBFunc(ObjectGetBFunc);

	while (ros::ok()) {
		object_vec.clear();
		for (int i = 0; i < Random(100); i++) {
			Object obj;
			obj.x = 0;
			obj.y = i;
			object_vec.push_back(obj);
		}

		publish_sphere.inner_sphere = object_vec;
		publish_sphere.publish(pub_pointcloud_, 'n');
		ros::spinOnce();
		rate.sleep();
	}


}