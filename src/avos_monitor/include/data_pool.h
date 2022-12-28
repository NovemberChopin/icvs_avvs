/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: ivbusinesscentre
* FileName: centrecommon.h
*
* Description: xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
    
*
* History:
* xxxxxx         xxxx/xx/xx    1.0.0    build this module.
******************************************************************************/
#ifndef CENTRE_COMMON_H
#define CENTRE_COMMON_H

#pragma once
using namespace std;

#include <ros/ros.h>
#include <rosbag/bag.h>
#include "ros/time.h"
#include "serial/serial.h"
#include <stdio.h>
#include <sys/types.h>  
#include <sys/mount.h>  
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <vector>
#include <pthread.h>
#include <typeinfo>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <assert.h>
#include <fcntl.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <dirent.h> 
#include <semaphore.h>
#include "errno.h"
#include <sys/utsname.h>  
#include <ctype.h>
#include <iomanip>
#include <string>
#include <iterator>
#include <sstream>
#include <map>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <boost/thread.hpp>
#include <time.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <netdb.h>
#include <malloc.h>
#include "json.hpp"
#include <thread>
#include "shm_transmitter.h"
#include <yaml-cpp/yaml.h>
#include "user_header.h"
#include <QString>
#include <QFileDialog>
#include <QMessageBox>

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#define MAXLINE 1024
#define EPSINON 1e-5
#define M_PI    3.14159265358979323846
using json = nlohmann::json;

typedef enum{
BUTTON_1,
BUTTON_2,
BUTTON_3,
BUTTON_4
}BUTTON;

enum switch_mode  
{  
    mode_minute,    //按每分钟生成日志文件名
    mode_hour,      //按每小时生成日志文件名
    mode_day,       //按每天生成日志文件名
    mode_month      //按每月生成日志文件名
};  

struct ProcessInfoStruct{    
    char            node_name[100];  //The name of the instruction given
    char            user[100];       //The process belongs to that user account.
    char            pro_stat[40];    //STAT: the current state of the program.   
    unsigned int    pid;            //The process ID number of the process.
    double          cpu_rate;       //The percentage of CPU resources used by the process
    double          mem_rate;       //The percentage of physical memory occupied by the process
    double          topic_hz;
    double          delay_time;
    unsigned int    mem_vsz;        //The virtual memory used by the process(Kbytes)
    unsigned int    mem_rss;        //The process occupies a fixed amount of memory(Kbytes)
    ProcessInfoStruct(){
        pid = 0;
        cpu_rate = 0;
        mem_rate = 0;
        topic_hz = 0;
        delay_time = 0;
        mem_rss = 0;
        mem_vsz = 0;
    }
    void Reset()
    {
        pid = 0;
        cpu_rate = 0;
        mem_rate = 0;
        topic_hz = 0;
        delay_time = 0;
        mem_rss = 0;
        mem_vsz = 0;  
        memset(node_name,0,sizeof(node_name));
        memset(user,0,sizeof(user));
        memset(pro_stat,0,sizeof(pro_stat));      
    }
};
// STAT: the current state of the program.The main states are:
// R: the program is currently in operation or can be operated;
// S: the program is currently asleep, but can be awakened by some signal.
// T: the program is currently detecting or stopping.
// Z: the program should have been terminated, but the parent program 
//could not normally terminate it, causing the zombie state


struct Point
{
    int     index;
    double  xg;
    double  yg;
    double  angle;
    Point(){
        index   = 0;
        xg      = 0.0;
        yg      = 0.0;
        angle   = 0.0;
    }
    void Reset(){
        index   = 0;
        xg      = 0.0;
        yg      = 0.0;
        angle   = 0.0;
    }
};


//used for cloud get fault bag list
struct BagInfo
{
    string bag_name;
    unsigned long bag_time;
    unsigned long bag_size;
    string bag_msg;
    string str_time;
    BagInfo()
    {
        bag_name = "";
        bag_time = 0;
        bag_size = 0;
        bag_msg = "";
        str_time = "";
    }
    void Reset()
    {
        bag_name = "";
        bag_time = 0;
        bag_size = 0;
        bag_msg = "";   
        str_time = "";     
    }
};

struct BagInfoList
{
    vector<BagInfo> bag_list;
    BagInfoList(){
    }
    void Reset(){
        bag_list.clear();
    }
};

struct MapInfo
{
    string map_name;       //地图名字  describe="测试图北京市腾讯众创空间"
    string map_id;         //地图ID  name="N4007823E11633873"
    string map_date;       //地图生成时间  date="2019-06-26 10:35:33"
    string map_version;    //地图协议版本号  version="1.5"
    MapInfo(){
        map_name    = "";
        map_id      = "";
        map_date    = "";
        map_version = "";
    }
    void Reset(){
        map_name    = "";
        map_id      = "";
        map_date    = "";
        map_version = "";
    }
};

struct Version{
    string kernel_version;
    string file_system_version;
    string map_version;
    string avos_version;
    string launch_version;
    string app_version;
    string bcm_version;
    string dmcu_version;
    string avcu_version;
    string cmcu_version;
    string cgw_version;

    string vin_number;
    string order_number;
    string product_number;
    string hardware_avcu_version;
    string hardware_bcm_version;
    string hardware_dmcu_version;

    void Reset(){
        kernel_version          = "V00.00.00.00";
        file_system_version     = "V00.00.00.00";
        map_version             = "V00.00.00.00";
        avos_version            = "V00.00.00.00";
        launch_version          = "V00.00.00.00";
        app_version             = "V00.00.00.00";
        bcm_version             = "V00.00.00.00";
        dmcu_version            = "V00.00.00.00";
        avcu_version            = "V00.00.00.00";
        cmcu_version            = "V00.00.00.00";
        cgw_version             = "V00.00.00.00";

        vin_number              = "000000000";
        order_number            = "000000000";
        product_number          = "WXBSxxx";
        hardware_avcu_version   = "00";
        hardware_bcm_version    = "00";
        hardware_dmcu_version   = "00";
    }
};

enum CALIBRATION_STATUS{                        //标定状态
    CALIBRATION_IDLE,                           //标定空闲(无效值)
    CALIBRATION_READY,                          //标定准备成功
    CALIBRATION_BUSY,                           //标定中
    CALIBRATION_SUCCESS,                        //标定成功
    CALIBRATION_FAILED,                         //标定失败
    CALIBRATION_VERIFICATION                    //标定认证
};  

typedef struct my_flags{
    bool android_data_req_flag;
    bool calibrationresult_flag;
    bool orderreply_flag;
    bool orderreply_packet_error_flag;
    bool anstask_flag;
    bool anstasktable_flag;
    bool expect_end_time_flag;
}T_my_flags,*P_my_flags;


struct TimeStatus
{
    float dtime;
    string source_node_name;
    string destination_node_name;
    TimeStatus(){
        dtime                 = 0.0;
        source_node_name      = "";
        destination_node_name = "";
    }
    void Reset(){
        dtime                 = 0.0;
        source_node_name      = "";
        destination_node_name = "";
    }
};


struct TimeStatistics
{
    vector<TimeStatus> dev_time_status_msg;
    float sending_timestamp;
    TimeStatistics(){
        sending_timestamp = 0.0;
        dev_time_status_msg.clear();
    }
    void Reset(){
        sending_timestamp = 0.0;
        dev_time_status_msg.clear();
    }
};


typedef struct{
    int    type;
    double time;
}voice_info_types;


struct NetStat
{
    int net_loss;
    float net_avg_rate;

    NetStat()
    {
        net_loss = 100;
        net_avg_rate = -1;
    }
    void Reset()
    {
        net_loss = 100;
        net_avg_rate = -1;
    }

};


typedef struct CubeSphereObject
{
    double x;
    double y;
    double z;
    double width;
    double length;
    double height;
    double heading;
    char addition_message[20];

    CubeSphereObject() {
        x = 0.0;
        y = 0.0;
        z = 0.0;
        width = 0.0;
        length = 0.0;
        height = 1.0;
        for (int i = 0; i < 20; i++) {
            addition_message[i] = 0;
        }
    }
} CubeSphereObject;

typedef struct PerceptionDebugAddition
{
    int source;
} PerceptionDebugAddition;

typedef struct PerceptionAddition
{
    int id;
    int type;
    float vxrel;
    float vyrel;
    float speed;

    PerceptionAddition() {
        id = 0;
        type = 4;
        vxrel = 0.0;
        vyrel = 0.0;
        speed = 0.0;
    }
} PerceptionAddition;

typedef struct LinePoint
{
    double x;
    double y;
    double option_message;
    LinePoint() {
        x = 0.0;
        y = 0.0;
        option_message = 0.0;
    }
} LinePoint;

typedef struct LaneParameter {
    int Lanetype;
    int LaneConfidence;
    double a;
    double b;
    double c;
    LaneParameter() {
        Lanetype = 0;
        LaneConfidence = 0;
        a = 0.0;
        b = 0.0;
        c = 0.0;
    }
} LaneParameter;

#define VISION_SOURCE     0x0000
#define LIDAR_SOURCE      0x0100
#define RADAR_SOURCE      0x0200
#define SOURCE_MASK       0xFF00

#define VISION_COLOR      0x00FF7F
#define RADAR_COLOR       0xFFFF00
#define LIDAR_COLOR       0x7B68EE

#define UP_SPEED 40.0/3.6
#define SPEED_INDEX 2

extern pthread_mutex_t      get_monitor_mutex_;      //文件读写操作互斥量
extern AvvsCommonInfo               g_common_info_;
extern FaultsInfo                   g_monitor_info_;


void InitCommonParams();

int myexec(const char *cmd, std::vector<std::string> &resvec);
bool getPackagePath(std::string package_name, std::string &res);
int IsFolderExist(const char* path);
int IsFileExist(const char* path);
void SetFileDialog(QFileDialog &fd, QFileDialog::FileMode f_mode, QFileDialog::ViewMode v_mode, QFileDialog::AcceptMode a_mode);
void SetMessageBox(QMessageBox &box);

QString GetVehicleStateInfo(int state);
QString GetDriverModeStateInfo(int state);
QString GetLocationStateInfo(int state);
QString GetEpsStateInfo(int state);
QString GetActuatorStateInfo(int state);
QString GetShiftVlpositionStateInfo(int state);
QString GetGpsStateInfo(int state);    
QString GetStartStateInfo(int state);
QString GetOpenCloseInfo(int state);//0:关闭；1：开启
QString GetVCUStateInfo(int state);
QString GetNboostStateInfo(int state);
QString GetAvosStateInfo(int state);
    
#endif /*CENTRE_COMMON_H*/
