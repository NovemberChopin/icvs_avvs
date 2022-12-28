#ifndef __USER_HEADER_H__
#define __USER_HEADER_H__

#include <sstream>
#include "macros.h"
#include <vector>
#include "lib/shared_memory.h"


#define MAX_OBJECT_SIZE 200
using namespace avos::common;
using namespace std;

typedef struct Object
{
	int id;
	double x;
	double y;
	double z;
	double vxrel;
	double vyrel;
	double vzrel;
	double vabs;
	std::string ToString(const std::string &prefix = "") {
		stringstream ss;
		ss << prefix << " id:" << id << "\n";
		ss << prefix << " x:" << x << "\n";
		ss << prefix << " y:" << y << "\n";
		ss << prefix << " z:" << z << "\n";
		ss << prefix << " vxrel:" << vxrel << "\n";
		ss << prefix << " vyrel:" << vyrel << "\n";
		ss << prefix << " vzrel:" << vzrel << "\n";
		ss << prefix << " vabs:" << vabs << "\n";
		return ss.str();
	}
} Object;

typedef struct PercetionObjects
{
	int index;
	Object objs[MAX_OBJECT_SIZE];
	std::string ToString(const std::string &prefix = "") {

		stringstream ss;
		ss << "------------\n";
		ss << "index:" << index << "\n";
		for (int i = 0; i < index; i++) {
			ss << "-------\n";
			ss << objs[i].ToString("\t");
		}
		return ss.str();
	}
	PercetionObjects() {
		index = 0;
	}

} PercetionObjects;


typedef struct PercetionObjectsShmArea
{
	ShmAreaHeader area_header;
	PercetionObjects objects;
} PercetionObjectsShmArea;

typedef struct PercetionShm
{
	ShmHeader header;
	PercetionObjectsShmArea objects_area[2];
	// other area
} PercetionShm;

class PerceptionObjectMem
{
public:
	PercetionShm *objs_mem_ptr;
	int object_shmid_ ;
	void *object_shmaddr;
	std::string topic;
private:
	PerceptionObjectMem() {
		objs_mem_ptr = nullptr;
		object_shmaddr = nullptr;
		object_shmid_ = -1;
	}

public:
	static PerceptionObjectMem *Instance() {
		static PerceptionObjectMem *instance = nullptr;
		if (!instance) {
			instance = new PerceptionObjectMem();
		}
		return instance;
	}

	// BASE_DECLARE_SINGLETON(PerceptionObjectMem)

};












#endif