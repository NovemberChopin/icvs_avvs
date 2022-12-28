#include "sensor_test.h"

using namespace avos::common;
//using namespace avos::common::config;

namespace avos {
namespace driver {

SensorTest::SensorTest() {}
SensorTest::~SensorTest() {}

int SensorTest::Init() {
  RETURN_VAL_IF(InitVars() == -1, -1);
  communication_->Init();


  return 0;
}

int SensorTest::InitVars() {
  communication_ = std::make_shared<Shmbridge>();
  RETURN_VAL_IF_NULL(communication_, -1);

  return 0;
}

int SensorTest::Start(double fre) {
  ros::Rate rate(fre);
  communication_->SpinOnce();

  while (ros::ok()) {
    std::cout << "publish ok" << std::endl;
    PerceptionShmApi::PrintShmInfo();
    communication_->Publish();
    rate.sleep();
  }
  return 0;
}


} // namespace driver
} // namespace avos


