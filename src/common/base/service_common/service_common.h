#ifndef __SERVICE_COMMON_H__
#define __SERVICE_COMMON_H__

#include <signal.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <stdio.h>

#include "service_data.h"
#include "serialization_struct.h"



namespace avos {
namespace service {

class ServiceCommon
{
public:
	ServiceCommon() {

	}
	~ServiceCommon() {

	}

	static void SignalHandle(int sig_num) {
		std::cout << "service stop" << std::endl;
		std::cout << "SignalHandle " << ", " << sig_num << std::endl;
		// exit(-1);
		abort();

	}

	static int Init(const std::string name ) {
		std::cout << name << ": service start " << std::endl;
		std::cout << "pid:" << getpid() << std::endl;
		signal(SIGUSR1, ServiceCommon::SignalHandle);
		auto service_data = ServiceData::Instance();
		std::cout << "size:" << service_data->service_info_.size() << std::endl;
		int index = -1;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			std::cout << "name|service_name:" << name << "|" << service_data->service_info_[i].service_name << std::endl;
			if (name == service_data->service_info_[i].service_name) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "cannot find the service in service list " <<  name << std::endl;
			exit(0);
		}
		int service_id = service_data->service_info_[index].service_id;

		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}
		service_detail->service_info[service_id].pid = getpid();
		service_detail->service_info[service_id].fault_num = 0;
		service_detail->service_info[service_id].status = 1;

	}

	static int Stop(const std::string name ) {
		auto service_data = ServiceData::Instance();
		int index = -1;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			if (name == service_data->service_info_[i].service_name) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "cannot find the service in service list" << name << std::endl;
			exit(0);
		}
		int service_id = service_data->service_info_[index].service_id;

		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}
		if (service_detail->service_info[service_id].status != 1) {
			std::cout << "serivce " << name << " is not running" << std::endl;
			return 0;
		}

		if (kill(service_detail->service_info[service_id].pid, SIGUSR1) == -1) {
			std::cout << "failed to kill service " << name << std::endl;
			return -1;
		}
		service_detail->service_info[service_id].Reset();
		return 0;
	}

	static int Start(const std::string name ) {
		auto service_data = ServiceData::Instance();
		int index = -1;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			if (name == service_data->service_info_[i].service_name) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "cannot find the service in service list" << name << std::endl;
			exit(0);
		}
		int service_id = service_data->service_info_[index].service_id;
		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}
		if (service_detail->service_info[service_id].status == 1) {
			std::cout << "serivce " << name << " is still running, please stop first" << std::endl;
			return 0;
		}

		std::string service_path = service_data->service_info_[index].service_path;
		pid_t childPid = fork();
		if (childPid != 0)
		{
			sleep(1);
			printf("now we are in parent progress,pid=%d\n", (int)getpid());
			printf("now we are in parent progress,childPid = %d\n", childPid);
		}
		else
		{
			// excel(service_path.c_str(), service_path.c_str(), NULL);
			system(service_path.c_str());
		}
		return 0;
	}



	static void Status() {
		auto service_data = ServiceData::Instance();
		int index = -1;

		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}

		std::cout << "serivce_id       service_name       status          pid          fault_num       service_path" << std::endl;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			stringstream ss;
			Print<int>(ss, (int)(sizeof("serivce_id") - 1),  service_data->service_info_[i].service_id );
			ss << "       ";
			Print<std::string>(ss, int(sizeof("service_name") - 1),  service_data->service_info_[i].service_name );
			ss << "       ";
			// ss << service_data->service_info_[i].service_id << " \t " << service_data->service_info_[i].service_name << " \t ";
			int service_id = service_data->service_info_[i].service_id;
			service_detail->service_info[service_id].PrintToSS(ss);
			Print<std::string>(ss, int(sizeof("service_path") - 1),  service_data->service_info_[i].service_path );
			ss <<  "\n";
			std::cout << ss.str();
		}

	}

	static void Restart(const std::string name ) {
		Stop(name);
		sleep(1);
		Start(name);
	}

	static void SetFaultNum( int fault_num , const std::string name = "") {
		auto service_data = ServiceData::Instance();
		int index = -1;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			if (name == service_data->service_info_[i].service_name) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "cannot find the service in service list" << name << std::endl;
			exit(0);
		}
		int service_id = service_data->service_info_[index].service_id;

		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}
		service_detail->service_info[service_id].fault_num |= (1 << fault_num % 31);
	}

	static void ResetFaultNum(int fault_num , const std::string name = "")  {
		auto service_data = ServiceData::Instance();
		int index = -1;
		for (int i = 0; i < service_data->service_info_.size(); i++) {
			if (name == service_data->service_info_[i].service_name) {
				index = i;
				break;
			}
		}
		if (index == -1) {
			std::cout << "cannot find the service in service list" << name << std::endl;
			exit(0);
		}
		int service_id = service_data->service_info_[index].service_id;

		ServiceDetail *service_detail = nullptr;
		while (service_detail == nullptr) {
			service_data->service_detail_shm_.GetUniqueDataPtr(service_detail);
			sleep(1);
		}
		service_detail->service_info[service_id].fault_num &= ~(1 << fault_num % 31);
	}


};







}
}

#endif
