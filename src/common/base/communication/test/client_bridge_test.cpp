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

void GenerateParam(common_msgs::LLH &gps_point) {
	gps_point.lat.variable = 12345.0;
	gps_point.lon.variable = 2345.0;
	gps_point.height.variable = 12.0;
}


int main(int argc, char **argv)
{
	ros::init(argc, argv, "client");
	ros::NodeHandle n;

	common_msgs::LLH  gps_data;
	std_msgs::UInt8 result;

	avos::service::ServiceClientBridge test_service_client_bridge("test/service");
	GenerateParam(gps_data);
	int value = test_service_client_bridge.CallFunc<common_msgs::LLH, std_msgs::UInt8>(1, 1, gps_data, result );
	std::cout << "value:" << value << std::endl;


	return 0;
}