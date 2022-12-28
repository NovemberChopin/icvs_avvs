#ifndef __SERVICE_DATA_H__
#define __SERVICE_DATA_H__

#include <vector>

#include "service_common/serialization_struct.h"
#include "service_common/service_typedef.h"
#include "shm_transmitter.h"

using namespace avos::common;
namespace avos {
namespace service {




class ServiceData
{
public:
	std::vector<ServiceConfigItem> service_info_;
	ShmTransmiiter<ServiceDetail> service_detail_shm_;

private:
	ServiceData() {
		DeSerializationStructVector<ServiceConfigItem>("/home/idriver/Documents/testnode/test_avos_service/avos_service/service.conf", service_info_);
		for (int i = 0; i < service_info_.size(); i++) {
			service_info_[i].print();
		}

		service_detail_shm_.InitWriter("tp_service_detail", 1);
	}

public:
	static ServiceData *Instance() {
		static ServiceData *instance = nullptr;
		if (!instance) {
			instance = new ServiceData();
		}
		return instance;
	}

private:
	ServiceData(const ServiceData &) = delete;
	ServiceData &operator=(const ServiceData &) = delete;

};











}
}


#endif