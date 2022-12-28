/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: monitor
* FileName: main.cpp
*
* Description: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

*
* History:
* xxxxxx         xxxx/xx/xx    1.0.0    build this module.
******************************************************************************/

#ifndef _SENSORTEST_DRIVER_H_
#define _SENSORTEST_DRIVER_H_


//c++ header
#include <functional>
#include <thread>
#include <memory>
#include <string>
#include <future>
#include <ctime>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common/base/communication/include/communication_base.h"
#include "common/base/config/include/param_config_manager.h"
#include "common/base/time/include/rate.h"
#include "common/base/log/include/log.h"
#include "data_pool.h"
#include "rosbridge.h"
#include "shmbridge.h"
#include "common_typedef.h"


#include "serial/serial.h"


using namespace std;
using namespace avos::common;

using CommunicationMethd = avos::common::CommunicationBase;

namespace avos {
namespace driver {

class SensorTest {
public:
	SensorTest();
	virtual ~SensorTest();
	int Init();
	int Start(double fre);

private:
	std::shared_ptr<CommunicationMethd> communication_;

private:
	int InitVars();


};

}  // namespace driver
}  // namespace avos

#endif
