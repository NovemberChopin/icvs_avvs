/******************************************************************************
 * Copyright (C) 2018-2023, idriverplus(BeiJing ZhiXingZhe, Inc.)
 *
 * History:
 * xiayang          2019/03/28    1.0.0        build
 *****************************************************************************/
#ifndef AVOS_SENSORUWB_DATA_POOL_H_
#define AVOS_SENSORUWB_DATA_POOL_H_

#include <condition_variable>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <vector>
#include "common/base/log/include/log.h"
#include "common/base/macros.h"
#include "common_typedef.h"

namespace avos {
namespace driver {

class DataPool {
public:
	SensorUWBInfo* GetMainDataPtr() { return &sensor_uwb_data_; }
	SensorUWBInfo& GetMainDataRef() { return sensor_uwb_data_; }

private:
	SensorUWBInfo sensor_uwb_data_;

	DataPool() {}

	BASE_DECLARE_SINGLETON(DataPool)
};

}  // namespace driver
}  // namespace avos
#endif  // AVOS_DRIVER_DATA_POOL_H_
