#ifndef __SENSORUWB_COMMON_TYPEDEF_H__
#define __SENSORUWB_COMMON_TYPEDEF_H__
#include "common/common_header/global_define.h"

namespace avos {
namespace driver {


#define MAX_BUFFER_LENGTH 4096
#define NAV_DATA_LENGTH 58



typedef struct SensorUWBInfo {
	double x;
	double y;
	SensorUWBInfo() {
		x = 0.0;
		y = 0.0;
	}
} SensorUWBInfo;



















}
}


#endif
