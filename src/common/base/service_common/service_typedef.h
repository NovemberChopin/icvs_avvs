#ifndef __SERVICE_TYPEDEF_H__
#define __SERVICE_TYPEDEF_H__

#include <sstream>
#include <vector>
#include <memory>
#include <atomic>

#include "serialization_struct.h"

typedef struct ServiceConfigItem
{
	int service_id;
	std::string service_name;
	std::string service_path;
	void Serialization(stringstream &ss) {
		REGISTER_SERIALIZATION_ATTRIBUTE( service_id);
		REGISTER_SERIALIZATION_ATTRIBUTE(service_name);
		REGISTER_SERIALIZATION_ATTRIBUTE(service_path);
		REGISTER_ENDLINE();
	}

	void DeSerialization(stringstream &ss) {
		REGISTER_DESERIALIZATION_ATTRIBUTE( service_id, SPLIT_CHAR);
		REGISTER_DESERIALIZATION_ATTRIBUTE( service_name, SPLIT_CHAR);
		REGISTER_DESERIALIZATION_ATTRIBUTE( service_path, SPLIT_CHAR);
	}

	void print() {
		std::cout << "id|name|path:" << service_id << "|" << service_name << "|" << service_path << std::endl;
	}

} ServiceConfigItem;

#pragma pack(push, 1)
typedef struct ServiceDetailItem
{
	unsigned int pid;
	unsigned int fault_num;
	unsigned int status;   // 0 idle  1 running 2 stopping
	ServiceDetailItem() {
		pid = 0;
		fault_num = 0;
		status = 0;
	}
	void PrintToSS(stringstream &ss) {
		Print(ss, sizeof("status") - 1,  status);
		ss << "       ";
		Print(ss, sizeof("   pid   ") - 1,  pid);
		ss << "       ";
		Print(ss, sizeof("fault_num") - 1,  fault_num);
	}
	void Reset() {
		pid = 0;
		fault_num = 0;
		status = 0;
	}


} ServiceDetailItem;

typedef struct ServiceDetail
{
	ServiceDetailItem service_info[4096];
} ServiceDetail;
#pragma pack(pop)


typedef struct UniqueObject
{
	std::atomic<int> data1;
	std::atomic<int> data2;
	std::atomic<int> data3;
	std::atomic<int> data4;
	std::atomic<int> data5;
	UniqueObject() {
		data1.store(0);
		data2.store(0);
		data3.store(0);
		data4.store(0);
		data5.store(0);
	}
	void Print() {
		std::cout << "data1:" << data1.load() << std::endl;
		std::cout << "data2:" << data2.load() << std::endl;
		std::cout << "data3:" << data3.load() << std::endl;
		std::cout << "data4:" << data4.load() << std::endl;
		std::cout << "data5:" << data5.load() << std::endl;
	}

} UniqueObject;












#endif