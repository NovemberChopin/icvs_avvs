#ifndef __AVOS_SERVICE_BRIDGE_H__
#define __AVOS_SERVICE_BRIDGE_H__

#include "ros/ros.h"

#include <functional>
#include <string>
#include <map>
#include "ros_serialization_bridge.h"
#include "common_msgs/AvosService.h"
namespace avos {
namespace service {

class AvosServiceBase
{
public:
	AvosServiceBase() {

	}
	virtual ~AvosServiceBase() {

	}

	virtual int RunFunc(std::vector<uint8_t> &input, std::vector<uint8_t>  &output) {
		return 0;
	}

};

template<class T1, class T2>
class AvosService: public AvosServiceBase
{
public:
	typedef std::function < int (const T1 &, T2 &)> pAvosServiceFunction;
	AvosService() {
		// pfunc = std::bind(DefaultFunction, std::placeholders::_1, std::placeholders::_2);
	}
	virtual ~AvosService() {

	}

	int RunFunc(std::vector<uint8_t> &input_array, std::vector<uint8_t>  &output_array) {
		T1 input;
		T2 output;
		RosSerializationBridge::DeSerialization<T1>(input, input_array);
		int result = pfunc(input, output);
		RosSerializationBridge::Serialization<T2>(output_array, output);
		return result;
	}

	void SetServiceFunc(pAvosServiceFunction mm) {
		pfunc = mm;
	}

	int DefaultFunction(const T1 &data_input, T2 &data_output) {
		std::cout << "please SetServiceFunc first" << std::endl;
		return 0;
	}
private:
	pAvosServiceFunction pfunc;
};


class ServiceServerBridge
{
public:
	ServiceServerBridge(const std::string &service_name) {
		ros::NodeHandle n;
		service_server_ = n.advertiseService(service_name, &ServiceServerBridge::ExeServiceCallback, this);
	}
	~ServiceServerBridge() {
		service_server_.shutdown();
	}

	template<class T1, class T2>
	void AddServiceCallback(unsigned int common_id,
	                        unsigned int sub_common_id,
	                        const std::function< int(const T1 &, T2 &)> &callback) {
		if (common_id == 0) {
			std::cout << "error cannot add zero command_id , it's used for avos" << std::endl;
			return ;
		}
		auto service_ptr = std::make_shared<AvosService<T1, T2>>();
		service_ptr->SetServiceFunc(callback);
		service_ptr_collection[common_id][sub_common_id] = service_ptr;
	}

	template<class T1, class T2, class M>
	void AddServiceCallback(unsigned int common_id,
	                        unsigned int sub_common_id,
	                        int(M::*fp)(const T1 &, T2 &) ,
	                        M *obj) {
		auto service_ptr = std::make_shared<AvosService<T1, T2>>();
		auto temp_function = std::bind(fp, obj, std::placeholders::_1, std::placeholders::_2);
		service_ptr->SetServiceFunc(temp_function);
		service_ptr_collection[common_id][sub_common_id] = service_ptr;
	}



	bool ExeServiceCallback(common_msgs::AvosService::Request  &req,
	                        common_msgs::AvosService::Response &res)
	{
		res.command_seq = req.command_seq;
		std::cout << "rev service command id | sub_common_id " << req.command_id << "|" << req.sub_common_id << std::endl;
		auto it_command = service_ptr_collection.find(req.command_id);
		if (it_command == service_ptr_collection.end()) {
			std::cout << "cannot find service command id " << req.command_id << std::endl;
			res.result = -1;
			return false;
		}
		auto it_sub_command = it_command->second.find(req.sub_common_id);
		if (it_sub_command == it_command->second.end()) {
			std::cout << "cannot find service command id | sub_common_id " << req.command_id << "|" << req.sub_common_id << std::endl;
			res.result = -1;
			return false;
		}
		res.result = it_sub_command->second->RunFunc(req.invoke_param, res.result_param);
		return true;
	}



private:
	std::map<unsigned int, std::map<unsigned int, std::shared_ptr<AvosServiceBase>>> service_ptr_collection;
	ros::ServiceServer service_server_;
};


class ServiceClientBridge
{
public:
	ServiceClientBridge(const std::string &service_name) {
		ros::NodeHandle n;
		service_client_ = n.serviceClient<common_msgs::AvosService>(service_name);
		service_name_ = service_name;
		sequence = 0;
	}
	~ServiceClientBridge() {
		service_client_.shutdown();
	}


public:
	template<class T1, class T2>
	int CallFunc(unsigned int command_id, unsigned int sub_common_id,
	             const T1 &input, T2 &output) {
		srv.request.command_seq = sequence++;
		srv.request.command_id = command_id;
		srv.request.sub_common_id = sub_common_id;
		RosSerializationBridge::Serialization<T1>(srv.request.invoke_param, input );
		if (service_client_.call(srv))
		{
			// std::cout << "call success " << srv.response.result << std::endl;
			RosSerializationBridge::DeSerialization<T2>(output, srv.response.result_param);
			return srv.response.result;
		}
		else
		{
			ROS_ERROR("Failed to call service %s", service_name_.c_str());
			return -1;
		}
	}

private:
	std::string service_name_;
	common_msgs::AvosService srv;
	int sequence;
	ros::ServiceClient service_client_;
};
}
}

#endif

