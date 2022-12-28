/*************************************************************************
    > File Name: globaldefine.h
    > Author:
    > Mail:
    > Created Time: 2018年03月04日 星期日 17时21分20秒
 ************************************************************************/

#ifndef _GLOBALDEFINE_H__
#define _GLOBALDEFINE_H__


typedef unsigned char uint8;
typedef float float32;
typedef double float64;
typedef char int8;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;



typedef struct TFData
{
    float x;
    float y;
    float z;
    float roll;
    float pitch;
    float yaw;
    TFData() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        roll = 0.0;
        pitch = 0.0;
        yaw = 0.0;
    }
} TFData;

#endif
