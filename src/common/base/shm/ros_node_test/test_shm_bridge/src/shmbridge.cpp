#include "common/base/time/include/node_time.h"
#include "common/base/communication/include/communication_base.h"
#include "common/common_header/fault_vector_handler.h"
#include "common/common_header/time_monitor.h"
#include "shmbridge.h"

#include <random>
#include <unistd.h>

#include "header/user_api.h"

using namespace std;
default_random_engine e;
namespace avos {
namespace driver {

Shmbridge::Shmbridge() {
	PerceptionShmApi::InitObjShm("tpperception");
}

void GenerateData(PercetionObjects& objct) {
	objct.index = 0;

	for (int i = 0; i < 10; i++) {

		Object obj;
		obj.id = i;
		obj.x = (float)e() / e.max();
		if (i == 0)
			std::cout << "obj.x " << obj.x << std::endl;
		obj.y = (float)e() / e.max();
		obj.z = (float)e() / e.max();
		obj.vxrel = (float)e() / e.max();
		obj.vyrel = (float)e() / e.max();
		obj.vzrel = (float)e() / e.max();
		obj.vabs = (float)e() / e.max();
		objct.objs[objct.index++] = obj;
	}
}
int Shmbridge::Init() {
	return 0;
}

int Shmbridge::SpinOnce() {
	ros::spinOnce();
	return 0;
}

int Shmbridge::Publish() {
	GenerateData(pub_msg_);
	PerceptionShmApi::SetPerceptionData(pub_msg_);

	return 0;
}


} // namespace driver
} // namespace avos
