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
using namespace common_msgs;
using namespace avos::service;

class ServiceBridgeClass
{
public:
	ServiceBridgeClass() :
		test_service_server_bridge("test/service") {

	}
	~ServiceBridgeClass() {

	}

	void Start() {
		ros::NodeHandle n;
		test_service_server_bridge.AddServiceCallback((unsigned int)1, (unsigned int)1, &ServiceBridgeClass::Fn, this);

		ros::Rate r(10);
		while (ros::ok()) {
			ros::spinOnce();
			r.sleep();

		}
	}


	int Fn(const common_msgs::LLH &input, std_msgs::UInt8 &output) {
		std::cout << "call Fn" << std::endl;
		return -10;
	}

private:
	avos::service::ServiceServerBridge test_service_server_bridge;

};




int main(int argc, char **argv)
{
	ros::init(argc, argv, "server");
	ros::NodeHandle n;

	ServiceBridgeClass server;

	server.Start();

	return 0;
}
