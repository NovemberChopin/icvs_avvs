#include "ros/ros.h"
#include <iostream>
#include <string>

#include <unistd.h>
#include <string.h>

#include "ros/serialization.h"
#include "avos_service_bridge.h"

#include "common_msgs/LLH.h"
#include "std_msgs/UInt8.h"
using namespace std;



int Fn(const common_msgs::LLH &input, std_msgs::UInt8 &output) {
	std::cout << "call Fn" << std::endl;
	return -10;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "listener");
	ros::NodeHandle n;


	avos::service::ServiceServerBridge test_service_server_bridge("test/service");
	test_service_server_bridge.AddServiceCallback<common_msgs::LLH, std_msgs::UInt8>(1, 1, Fn);

	ros::Rate r(10);
	while (ros::ok()) {
		ros::spinOnce();
		r.sleep();

	}

	return 0;
}