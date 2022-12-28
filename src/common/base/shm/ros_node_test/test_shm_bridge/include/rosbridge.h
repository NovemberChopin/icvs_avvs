#ifndef __AVOS_SENSORUWB_ROSBRIDGE_H__
#define __AVOS_SENSORUWB_ROSBRIDGE_H__

#include "common/base/communication/include/communication_base.h"
#include "data_pool.h"
#include <ros/ros.h>
#include "test_shm_bridge/Sensorobj.h"


namespace avos {
namespace driver {

using PubMsgType = test_shm_bridge::Sensorobj;

class Rosbridge: public avos::common::CommunicationBase
{
public:
	Rosbridge();
	~Rosbridge() {}

	int Init();
	int SpinOnce();
	int Publish();


private:
	ros::Publisher pub_sensor_uwb_;
	ros::NodeHandle nh_;
	PubMsgType pub_msg_;
};

}
}

#endif // __AVOS_MONITOR_ROSBRIDGE_H__
