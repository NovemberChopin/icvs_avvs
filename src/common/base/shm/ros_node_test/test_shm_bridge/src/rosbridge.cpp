#include "common/base/time/include/node_time.h"
#include "common/base/communication/include/communication_base.h"
#include "common/common_header/fault_vector_handler.h"
#include "common/common_header/time_monitor.h"
#include "rosbridge.h"


namespace avos {
namespace driver {

Rosbridge::Rosbridge() {
    pub_sensor_uwb_ = nh_.advertise<PubMsgType>("tpperception", 100);//
}

int Rosbridge::Init() {
    return 0;
}

int Rosbridge::SpinOnce() {
    ros::spinOnce();
    return 0;
}

int Rosbridge::Publish() {

    pub_sensor_uwb_.publish(pub_msg_);

    return 0;
}


} // namespace driver
} // namespace avos
