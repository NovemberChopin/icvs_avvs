#include "sensor_test.h"
#include "common/base/log/include/avos_node.h"
#include <string>
#include <unistd.h>

int main(int argc, char* argv[])
{
    std::string node_name = "sensor_uwb";
    ros::init(argc, argv, node_name);
    ros::NodeHandle nh;
    bool Enable_GLOG_Screen = true;
    avos::common::AvosNode::Init(node_name, Enable_GLOG_Screen);
    std::shared_ptr<avos::driver::SensorTest> pp = std::make_shared<avos::driver::SensorTest>();
    if (pp->Init() == -1) {
        AERROR << node_name << " Init() failed!";
    }
    if (pp->Start(100.) == -1) {
        AERROR << node_name << " Start() failed!";
    }

    return 0;
}
