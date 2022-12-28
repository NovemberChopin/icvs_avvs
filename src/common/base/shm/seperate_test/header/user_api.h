#ifndef __MAP_ENGINE_API_H__
#define __MAP_ENGINE_API_H__
#include <iostream>

#include "user_header.h"
#include "lib/shared_memory.h"

namespace avos {
namespace common {
class PerceptionShmApi
{
public:
	PerceptionShmApi() {

	}
	~PerceptionShmApi() {

	}

public:
	static void PrintShmInfo() {
		auto perception_obj = PerceptionObjectMem::Instance();
		if (perception_obj->objs_mem_ptr != nullptr) {
			perception_obj->objs_mem_ptr->header.PrintInfo();
		}
	}
	static int InitObjShm(const std::string& topic) {
		std::cout << "Init Shm" << std::endl;
		auto perception_obj = PerceptionObjectMem::Instance();
		perception_obj->topic = topic;
		perception_obj->object_shmid_ = avos::common::SharedMemory::sharedMemoryCreateOrGet(
		                                    avos::common::SharedMemory::BKDRHash(topic.c_str()),
		                                    (sizeof(PercetionShm) / 4096 + 1) * 4096);

		if (perception_obj->object_shmid_ == -1) return -1;

		perception_obj->object_shmaddr = avos::common::SharedMemory::sharedMemoryAttach(perception_obj->object_shmid_);
		perception_obj->objs_mem_ptr = (PercetionShm*)perception_obj->object_shmaddr;
		perception_obj->objs_mem_ptr->header.shm_attash_times++;
		return 0;
	}

	static void UninitObjShm() {
		std::cout << "UninitObjShm" << std::endl;
		auto perception_obj = PerceptionObjectMem::Instance();
		perception_obj->objs_mem_ptr->header.shm_attash_times--;
		if (perception_obj->objs_mem_ptr->header.shm_attash_times == 0) {
			avos::common::SharedMemory::sharedMemoryDelete(perception_obj->object_shmid_);
			perception_obj->objs_mem_ptr = nullptr;
			return;
		}
		if (perception_obj->object_shmid_ != -1) {
			avos::common::SharedMemory::sharedMemoryDetatch(perception_obj->object_shmaddr);
		}
		perception_obj->objs_mem_ptr = nullptr;
	}


	static bool GetPerceptionData(PercetionObjects &objects) {
		auto perception_obj = PerceptionObjectMem::Instance();
		if (perception_obj->objs_mem_ptr == nullptr) {
			InitObjShm(perception_obj->topic );
			if (perception_obj->objs_mem_ptr == nullptr) {
				return false;
			}
		}
		int area_index = 0;
		if (perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.load() != eShmAreaStatus::UPDATED) {
			area_index = 1;
		}
		if (perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.load() != eShmAreaStatus::UPDATED) {
			return false;
		}
		perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.store(eShmAreaStatus::READING);
		objects = perception_obj->objs_mem_ptr->objects_area[area_index].objects;
		perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.store(eShmAreaStatus::IDLE);
		return true;
	}

	static bool SetPerceptionData(const PercetionObjects &objects) {
		auto perception_obj = PerceptionObjectMem::Instance();
		if (perception_obj->objs_mem_ptr == nullptr) {
			InitObjShm(perception_obj->topic );
			if (perception_obj->objs_mem_ptr == nullptr) {
				return false;
			}
		}
		int area_index = 0;
		if (perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.load() == eShmAreaStatus::READING) {
			area_index = 1;
		}

		perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.store(eShmAreaStatus::WRITING);
		perception_obj->objs_mem_ptr->objects_area[area_index].objects = objects;
		perception_obj->objs_mem_ptr->objects_area[area_index].area_header.shm_status.store(eShmAreaStatus::UPDATED);

		return true;
	}




};

}
}











#endif