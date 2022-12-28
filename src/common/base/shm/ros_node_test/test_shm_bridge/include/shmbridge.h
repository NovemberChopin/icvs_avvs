#ifndef __AVOS_SENSORUWB_SHMBRIDGE_H__
#define __AVOS_SENSORUWB_SHMBRIDGE_H__

#include "common/base/communication/include/communication_base.h"
#include "data_pool.h"
#include "header/user_api.h"


namespace avos {
namespace driver {

using PubSHMMsgType = PercetionObjects;

class Shmbridge: public avos::common::CommunicationBase
{
public:
	Shmbridge();
	~Shmbridge() {
		PerceptionShmApi::UninitObjShm();
	}

	int Init();
	int SpinOnce();
	int Publish();


private:
	PubSHMMsgType pub_msg_;
};

}
}

#endif // __AVOS_MONITOR_ROSBRIDGE_H__
