#ifndef __MAP_ENGINE_API_H__
#define __MAP_ENGINE_API_H__
#include <iostream>

#include "user_header.h"
#include "shared_memory.h"


namespace avos {
namespace common {
template <class T>
class ShmTransmiiter
{
public:
	ShmTransmiiter() {
		header_buffer_pointer = nullptr;
		topic = "";
		shmid_header_ = -1;
		start_fre = -1;
		start_time = -1.0;
		is_data_empty = true;

	}
	~ShmTransmiiter() {
		std::cout << "Uninit ShmTransmiiter" << std::endl;
		if (header_buffer_pointer != nullptr) {
			header_buffer_pointer->shm_attach_times--;
			if (header_buffer_pointer->shm_attach_times == 0) {
				avos::common::SharedMemory::sharedMemoryDelete(shmid_header_);
				header_buffer_pointer = nullptr;
				avos::common::SharedMemory::sharedMemoryDelete(shmid_data_);
				data_buffer_pointer = nullptr;
				return;
			}
		}
		if (shmid_header_ != -1) {
			avos::common::SharedMemory::sharedMemoryDetatch((void*)header_buffer_pointer);
		}
		if(shmid_data_ != -1){
			avos::common::SharedMemory::sharedMemoryDetatch((void*)data_buffer_pointer);
		}
		header_buffer_pointer = nullptr;
	}

public:

	int InitWriter(const std::string &_topic, int _buffer_len = 10) {
		std::cout << "Init ShmTransmiiter writer" << std::endl;
		topic = _topic;
		data_topic = topic + "data";
		buffer_len = _buffer_len;
		if (_buffer_len <= 0) _buffer_len = 10;
		shmid_header_ = avos::common::SharedMemory::sharedMemoryCreateOrGet(
		                    avos::common::SharedMemory::BKDRHash(topic.c_str()),
		                    ( sizeof(ShmHeader) / 4096 + 1) * 4096);

		if (shmid_header_ == -1) return -1;
		//std::cout << "object_shmid_header_:" << shmid_header_ << std::endl;
		header_buffer_pointer = (ShmHeader *)avos::common::SharedMemory::sharedMemoryAttach(shmid_header_);
		header_buffer_pointer->buffer_len.store(_buffer_len);


		if (header_buffer_pointer->buffer_len.load() == -1) {
			header_buffer_pointer->buffer_len.store(_buffer_len);
		}
		if (buffer_len > header_buffer_pointer->buffer_len.load()) {
			buffer_len = header_buffer_pointer->buffer_len.load();
		}
		read_loc = header_buffer_pointer->lowest_loc;
		header_buffer_pointer->shm_attach_times++;

		// data topic
		shmid_data_ = avos::common::SharedMemory::sharedMemoryCreateOrGet(
		                  avos::common::SharedMemory::BKDRHash(data_topic.c_str()),
		                  ( (sizeof(T) + sizeof(ShmAreaHeader)) * buffer_len / 4096 + 1) * 4096);
		if (shmid_data_ == -1) return -1;
		std::cout << "shmid_data_:" << shmid_data_ << std::endl;
		data_buffer_pointer = (char *)avos::common::SharedMemory::sharedMemoryAttach(shmid_data_);


		return 0;
	}

	int InitReader(const std::string &_topic) {
		//std::cout << "Init ShmTransmiiter reader" << std::endl;
		topic = _topic;
		data_topic = topic + "data";
		shmid_header_ = avos::common::SharedMemory::sharedMemoryCreateOrGet(
		                    avos::common::SharedMemory::BKDRHash(topic.c_str()),
		                    ( sizeof(ShmHeader) / 4096 + 1) * 4096);

		if (shmid_header_ == -1) return -1;
		//std::cout << "object_shmid_header_:" << shmid_header_ << std::endl;
		header_buffer_pointer = (ShmHeader *)avos::common::SharedMemory::sharedMemoryAttach(shmid_header_);

		if (header_buffer_pointer->buffer_len.load() <= 0) {
			return -1;
		}

		read_loc = header_buffer_pointer->highest_loc;
		header_buffer_pointer->shm_attach_times++;

		buffer_len = header_buffer_pointer->buffer_len.load();
		// data topic
		shmid_data_ = avos::common::SharedMemory::sharedMemoryCreateOrGet(
		                  avos::common::SharedMemory::BKDRHash(data_topic.c_str()),
		                  ( (sizeof(T) + sizeof(ShmAreaHeader)) * buffer_len / 4096 + 1) * 4096);
		if (shmid_data_ == -1) return -1;
		std::cout << "shmid_data_:" << shmid_data_ << std::endl;
		is_data_empty = false;
		data_buffer_pointer = (char *)avos::common::SharedMemory::sharedMemoryAttach(shmid_data_);
		return 0;
	}

	bool GetData(T &data) {
		if (is_data_empty) {
			InitReader(topic);
			return false;
		}
		if (header_buffer_pointer == nullptr || data_buffer_pointer == nullptr) {
			std::cout << "GetData pointer is null" << std::endl;
			return false;
		}
		pShmAreaHeader pointer_header;
		// std::cout << "read_loc " << read_loc.array_index << "|" << read_loc.seq_num << std::endl;
		int index = GetNextReadIndex(read_loc, pointer_header);

		if (index == -1) return false;
		pointer_header->shm_status.store(eShmAreaStatus::READING);
		memcpy((char *)&data, (char*)pointer_header + sizeof(ShmAreaHeader), sizeof(T));
		pointer_header->shm_status.store(eShmAreaStatus::IDLE);
		// std::cout << "gap_times:" << avos::common::NodeTime::Now().ToSecond() - pointer_header->send_time_stamp << std::endl;
		return true;
	}


	bool SetData(const T &data) {
		if (header_buffer_pointer == nullptr || data_buffer_pointer == nullptr) {
			return false;
		}
		pShmAreaHeader pointer_header;
		int res = GetNextWriteIndex(header_buffer_pointer->highest_loc, pointer_header);
		if (res == -1) return false;
		// pointer_header->send_time_stamp.store(avos::common::NodeTime::Now().ToSecond());
		pointer_header->shm_status.store(eShmAreaStatus::WRITING);
		memcpy((char*)pointer_header + sizeof(ShmAreaHeader), (char *)&data,  sizeof(T));
		pointer_header->shm_status.store(eShmAreaStatus::IDLE);
		FillInnerLoc(header_buffer_pointer->lowest_loc, header_buffer_pointer->highest_loc);
		return true;
	}


	void PrintShmInfo() {
		if (header_buffer_pointer != nullptr) {
			header_buffer_pointer->PrintInfo();
		}
	}

	double GetFrequence() {
		if (header_buffer_pointer == nullptr || data_buffer_pointer == nullptr) {
			return 0.0;
		}
		if (start_fre <= 0 || start_time < 0) {
			start_fre = header_buffer_pointer->highest_loc.seq_num;
			// start_time = avos::common::NodeTime::Now().ToSecond();
			return 0.0;
		}
		return 0.0;

		// return  (header_buffer_pointer->highest_loc.seq_num - start_fre) / (avos::common::NodeTime::Now().ToSecond() - start_time) ;
	}



private:
	ShmAreaHeader* GetIndexHeader(const int &index) {
		if (index < 0 || index >= buffer_len) {
			return nullptr;
		}
		return (ShmAreaHeader*)((char*)data_buffer_pointer + index * (sizeof(T) + sizeof(ShmAreaHeader)));
	}


	int GetNextReadIndex(AreaLocation &loc, pShmAreaHeader &next_p) {
		for (int i = 1; i < buffer_len; i++) {
			ShmAreaHeader* p = GetIndexHeader((loc.array_index + i) % buffer_len);
			if (p == nullptr) {
				std::cout << "p is nullptr" << std::endl;
				return -1;
			}
			// std::cout << "AreaLocation loc:" << loc.seq_num << "|" << p->sequence << std::endl;
			if (p->sequence > loc.seq_num && p->shm_status.load() != eShmAreaStatus::WRITING) {
				next_p = p;
				loc.array_index = (loc.array_index  + i) % buffer_len;
				loc.seq_num = p->sequence;
				return 0;
			}
		}
		return -1;
	}



	int GetNextWriteIndex(AreaLocation &loc, pShmAreaHeader &area) {
		for (int i = 1; i < buffer_len; i++) {
			ShmAreaHeader* p = GetIndexHeader((loc.array_index + i) % buffer_len);
			if (p == nullptr) {
				std::cout << "p is nullptr" << std::endl;
				return -1;
			}
			if ( p->shm_status.load() != eShmAreaStatus::READING) {
				loc.seq_num++;
				p->sequence = loc.seq_num;
				loc.array_index =  (loc.array_index  + i) % buffer_len;
				area = p;
				return 0;
			}
		}
		return -1;
	}


	void FillInnerLoc(AreaLocation &lowest, AreaLocation &highest) {
		int min_seq_num = 99999999;
		int max_seq_num = -1;

		for (int i = 0; i < buffer_len; i++) {
			ShmAreaHeader* p = GetIndexHeader(i);
			if (p == nullptr) {
				std::cout << "p is nullptr" << std::endl;
			}
			if (p->sequence < min_seq_num) {
				min_seq_num = p->sequence ;
				lowest.array_index = i;
				lowest.seq_num = min_seq_num;
			}
			if (p->sequence > max_seq_num) {
				max_seq_num = p->sequence;
				highest.array_index = i;
				highest.seq_num = max_seq_num;
			}
		}
	}

private:
	ShmHeader *header_buffer_pointer;
	char *data_buffer_pointer;
	std::string topic, data_topic;
	int shmid_header_ ;
	int shmid_data_ ;

	AreaLocation read_loc;

	int buffer_len;
	bool is_data_empty;


	int start_fre;
	double start_time;
};

}
}











#endif
