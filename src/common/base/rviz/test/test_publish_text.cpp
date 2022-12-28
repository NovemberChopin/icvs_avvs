#include "ros/ros.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <sstream>
#include "publish_text.h"


#define Random(x) (rand() % x)
typedef struct Object
{
	double x;
	double y;
	std::string ToString() {
		std::stringstream out;
		out << std::setprecision(2) << x;
		return out.str();
	}
} Object;


std::string ObjectTostringFunc(void* param) {
	Object *object = (Object *)param;
	std::stringstream out;
	out << std::setprecision(2) << object->x;
	return out.str();
}


// %Tag(main)%
int main(int argc, char** argv)
{
	ros::init(argc, argv, "test_publish_text");
	ros::NodeHandle n;




	ros::NodeHandle nh;
	ros::Publisher pub_pointcloud_ = nh.advertise<visualization_msgs::MarkerArray> ( "test_text", 1, true );

	PublishText<std::vector<Object>> publish_text;
	publish_text.SetToStringFunc(ObjectTostringFunc);
	ros::Rate rate(10);
	std::vector<Object> object_vec;
	object_vec.reserve(1000);

	while (ros::ok()) {
		object_vec.clear();
		for (int i = 0; i < Random(300); i++) {
			Object obj;
			obj.x = i;
			obj.y = i;
			object_vec.push_back(obj);
		}

		publish_text.inner_text = object_vec;
		publish_text.publish(pub_pointcloud_, 'g');
		ros::spinOnce();
		rate.sleep();
	}


}