#ifndef __USER_HEADER_H__
#define __USER_HEADER_H__

#include <sstream>
#include <vector>
#include "common/base/macros.h"
#include "common/base/IPC/shared_memory.h"


#define MAX_OBJECT_SIZE 200
#define MAX_FAULT_SIZE 200
#define DATA_MAX 1024
using namespace avos::common;
using namespace std;

typedef struct Object
{
	int id;
	double x;
	double y;
	double z;
	double vxrel;
	double vyrel;
	double vzrel;
	double vabs;
	std::string ToString(const std::string &prefix = "") {
		stringstream ss;
		ss << prefix << " id:" << id << "\n";
		ss << prefix << " x:" << x << "\n";
		ss << prefix << " y:" << y << "\n";
		ss << prefix << " z:" << z << "\n";
		ss << prefix << " vxrel:" << vxrel << "\n";
		ss << prefix << " vyrel:" << vyrel << "\n";
		ss << prefix << " vzrel:" << vzrel << "\n";
		ss << prefix << " vabs:" << vabs << "\n";
		return ss.str();
	}
} Object;

typedef struct PercetionObjects
{
	double time_gap;
	int index;
	Object objs[MAX_OBJECT_SIZE];
	char mem[DATA_MAX];
	std::string ToString(const std::string &prefix = "") {

		stringstream ss;
		ss << "------------\n";
		ss << "index:" << index << "\n";
		for (int i = 0; i < index; i++) {
			ss << "-------\n";
			ss << objs[i].ToString("\t");
		}
		return ss.str();
	}
	PercetionObjects() {
		index = 0;
	}

} PercetionObjects;


typedef struct PercetionObjectsShmArea
{
	ShmAreaHeader area_header;
	PercetionObjects objects;
} PercetionObjectsShmArea;

typedef struct PercetionShm
{
	ShmHeader header;
	PercetionObjectsShmArea objects_area[2];
	// other area
} PercetionShm;

typedef struct LocaPos
{
    float   xpos;
    float   ypos;
    float   zpos;
    float   angle;
    LocaPos(){
        xpos                    = 0.0;
        ypos                    = 0.0;
        zpos                    = 0.0;
        angle                   = 0.0;
    }
    void Reset(){
        xpos                    = 0.0;
        ypos                    = 0.0;
        zpos                    = 0.0;
        angle                   = 0.0;
    }
}LocaPos;

typedef struct SensorGPS
{
    int   gps_topic_hz;
    float lon;
    float lat;
    float height;
    float heading;
    float velocity;
    float track_angle;
    float utctime;
    float hdop;
    float diff_age;
    float base_length;
    float heading_std;
    uint  status;
    uint  satenum;
    uint  status_yaw;
    uint  is_heading_valid;
    SensorGPS(){
        gps_topic_hz = 0;
        lon = 0;
        lat = 0;
        height = 0;
        heading = 0;
        velocity = 0;
        track_angle = 0;
        utctime = 0;
        hdop = 0;
        diff_age = 0;
        base_length = 0;
        heading_std = 0;
        status = 0;
        satenum = 0;
        status_yaw = 0;
        is_heading_valid = 0;
    }
    void Reset(){
        gps_topic_hz = 0;
        lon = 0;
        lat = 0;
        height = 0;
        heading = 0;
        velocity = 0;
        track_angle = 0;
        utctime = 0;
        hdop = 0;
        diff_age = 0;
        base_length = 0;
        heading_std = 0;
        status = 0;
        satenum = 0;
        status_yaw = 0;
        is_heading_valid = 0;
    }
}SensorGPS;

typedef struct LocationStatus
{
    bool    data_valid;
    float   lon;
    float   lat;
    float   height;
    float   heading;
    float   utctime;
    float   base_length;
    float   velocity;
    float   velocity_left_wheel;
    float   velocity_right_wheel;
    float   trans_stable_theshold;
    float   trans_probability;
    int     gps_status;
    int     gps_num;
    int     location_status;
    int     localization_mode;
    double  gps_topic_hz;
    LocaPos fusion_pos;
    LocaPos gps_pos;
    LocaPos ndt_pos;
    LocationStatus(){
        data_valid              = false;
        lon                     = 0.0;
        lat                     = 0.0;
        height                  = 0.0;
        heading                 = 0.0;
        utctime                 = 0.0;
        base_length             = 0.0;
        velocity                = 0.0;
        velocity_left_wheel     = 0.0;
        velocity_right_wheel    = 0.0;
        trans_stable_theshold   = 0.0;
        trans_probability       = 0.0;
        gps_status              = 0;
        gps_num                 = 0;
        location_status         = 6;    //定位启动中
        localization_mode       = 0;
        gps_topic_hz            = 0;
    }
    void Reset(){
        data_valid              = false;
        lon                     = 0.0;
        lat                     = 0.0;
        height                  = 0.0;
        heading                 = 0.0;
        utctime                 = 0.0;
        base_length             = 0.0;
        velocity                = 0.0;
        velocity_left_wheel     = 0.0;
        velocity_right_wheel    = 0.0;
        trans_stable_theshold   = 0.0;
        trans_probability       = 0.0;
        gps_status              = 0;
        gps_num                 = 0;
        location_status         = 6;    //定位启动中
        localization_mode       = 0;
        gps_topic_hz            = 0;
        fusion_pos.Reset();
        gps_pos.Reset();
        ndt_pos.Reset();
    }
}LocationStatus;

typedef struct FaultInfo
{
    double  timestamp_sec;    //故障发生时间
    char  	module_name[100]; //故障模块或节点名称
    char  	msg[200];         //故障描述
    int     sequence_num;     //消息数
    char    version[40];          //版本号
    int     error_code;       //故障码
    int     fault_level;      //故障等级
    int     fault_type;       //0:其他故障;1:硬件故障:2软件故障;3:算法故障;4:外设故障;5:电路故障1;6:电路故障2;7:电源故障1;8:电源故障2;9:通讯故障
    int     deal_type;        //0:默认项不处理；1:硬件日常维修；2:硬件日常维修；3:现场抢修；4:软件后台记录；5:软件技术支持（一般）；6:软件技术支持（紧急）；7:平行驾驶；
    int     status_type;      //是否为状态类故障 0:否，1是
    int     clean_level;      //是否影响清扫 0:否，1是
    int     notify_type;
    FaultInfo(){
        timestamp_sec   = 0.0;
        sequence_num    = 0;
        error_code      = 0;
        fault_level     = 0;
        fault_type      = 0;
        deal_type       = 0;
        status_type     = 0;
        clean_level     = 0;
        notify_type     = 0;
    }
    void Reset(){
        timestamp_sec   = 0.0;
        sequence_num    = 0;
        error_code      = 0;
        fault_level     = 0;
        fault_type      = 0;
        deal_type       = 0;
        status_type     = 0;
        clean_level     = 0;
        notify_type     = 0;
    }
}FaultInfo;


typedef struct MonitorInfo
{
	FaultInfo fault_info[MAX_FAULT_SIZE];
	int   fault_size;
	int   fault_level;        //整车故障等级 0:不处理;1:减速;2:急停;
	int   auto_drive;		   //自动驾驶能力 0:不具备,1:具备
	int   parallel_drive;	   //##平行驾驶能力 0:不具备,1:具备
	int   romote_drive;	   //##遥控驾驶能力 0:不具备,1:具备
	float target_angle;       //##横向目标转角
	int   steer_torque;       //##横向驱动扭矩
	float target_torque;      //##驱动扭矩[0,1000]
	int   actuator_mode;      //##执行器模式:0:减速；1:加速;2:紧急制动
	int   shift_position;     //##档位信息:0:无效;1:R档;2:N档;3:D档
	int   current_eps_state;  //##EPS状态0：停止工作状态；0x10：手动控制状态；0x20: 自动控制状态；0x55：错误状态
	int   uisteer_angle;      //##实际转向角度
	float real_strtorque;     //##实际转向力矩
	float current_batsoc;     //##电池组剩余电量
	int   start_status;
	MonitorInfo()
	{
        fault_size 			= 0;
		fault_level 		= 0;
		auto_drive 			= 0;
		parallel_drive 		= 0;
		romote_drive 		= 0;
		target_angle 		= 0;
		steer_torque 		= 0;
		target_torque 		= 0;
		actuator_mode 		= 0;
		shift_position 		= 0;
		current_eps_state 	= 0;
		uisteer_angle 		= 0;
		real_strtorque 		= 0;
		current_batsoc 		= 0;
		start_status 		= 0;
	}
	void Reset()
	{
        fault_size 			= 0;
		fault_level 		= 0;
		auto_drive 			= 0;
		parallel_drive 		= 0;
		romote_drive 		= 0;
		target_angle 		= 0;
		steer_torque 		= 0;
		target_torque 		= 0;
		actuator_mode 		= 0;
		shift_position 		= 0;
		current_eps_state 	= 0;
		uisteer_angle 		= 0;
		real_strtorque 		= 0;
		current_batsoc 		= 0;
		start_status 		= 0;
	}
}MonitorInfo;


typedef struct FaultsInfo
{
    vector<FaultInfo> info_vec;     //故障信息
    double velocity;           //整车速度
    double heading;            //相对真北朝向
    double longitude;          //经度
    double latitude;           //纬度
    double globle_x;           //全局坐标系横轴
    double globle_y;           //全局坐标系纵轴
    double target_angle;       //横向目标转角
    double target_torque;      //驱动扭矩[0,1000]
    double real_strtorque;     //实际转向力矩
    double current_batsoc;     //电池组剩余电量
    int   fault_level;         //整车故障等级 0:不处理;1:减速;2:急停;
    int   auto_drive;          //自动驾驶能力 0:不具备,1:具备
    int   parallel_drive;      //平行驾驶能力 0:不具备,1:具备
    int   romote_drive;        //遥控驾驶能力 0:不具备,1:具备
    int   vehicle_state;       //车辆状态 0：初始化,1：休眠,2：遥控就绪,3：自动就绪,4：遥控驾驶,5：自动驾驶,6：人工干预,7：急停,8：故障
    int   gps_status;          //GPS状态:１:单点定位；２:伪距差分；３:预留；４:RTK固定解；５:RTK浮点解
    int   location_status;     //０:定位成功;１:定位一般;２:定位失败;３:IMU初始化中;４:激光初始化中;５:地图加载中;
    int   steer_torque;        //横向驱动扭矩
    int   actuator_mode;       //执行器模式:0:减速；1:加速;2:紧急制动
    int   shift_position;      //档位信息:0:无效;1:R档;2:N档;3:D档
    int   current_eps_state;   //EPS状态0：停止工作状态；0x10：手动控制状态；0x20: 自动控制状态；0x55：错误状态
    int   uisteer_angle;       //实际转向角度
    int   start_status;
    FaultsInfo(){
        info_vec.reserve(50);
        velocity            = 0.0;
        heading             = 0.0;
        longitude           = 0.0;
        latitude            = 0.0;
        globle_x            = 0.0;
        globle_y            = 0.0;
        target_angle        = 0.0;
        target_torque       = 0.0;
        real_strtorque      = 0.0;
        current_batsoc      = 0.0;
        fault_level         = 0;
        auto_drive          = 0;
        parallel_drive      = 0;
        romote_drive        = 0;
        vehicle_state       = 0;
        gps_status          = 0;
        location_status     = 0;
        steer_torque        = 0;
        actuator_mode       = 0;
        shift_position      = 0;
        current_eps_state   = 0;
        uisteer_angle       = 0;
        start_status        = 2;
    }
    void Reset(){
        velocity            = 0.0;
        heading             = 0.0;
        longitude           = 0.0;
        latitude            = 0.0;
        globle_x            = 0.0;
        globle_y            = 0.0;
        target_angle        = 0.0;
        target_torque       = 0.0;
        real_strtorque      = 0.0;
        current_batsoc      = 0.0;
        fault_level         = 0;
        auto_drive          = 0;
        parallel_drive      = 0;
        romote_drive        = 0;
        vehicle_state       = 0;
        gps_status          = 0;
        location_status     = 0;
        steer_torque        = 0;
        actuator_mode       = 0;
        shift_position      = 0;
        current_eps_state   = 0;
        uisteer_angle       = 0;
        start_status        = 2;
    }
}FaultsInfo;


typedef struct MapCenter
{
    double xg;
    double yg;
    double zg;
    MapCenter()
    {
        xg  = 0.0;
        yg  = 0.0;
        zg  = 0.0;
    }
    void Reset()
    {
        xg  = 0.0;
        yg  = 0.0;
        zg  = 0.0;      
    }
}MapCenter;

typedef struct TaskPoint
{
    int     type;//1:途经点，２，任务终点
    float   x;
    float   y;
    float   heading;
    TaskPoint()
    {
        type = 1;
        x = 0.0;
        y = 0.0;
        heading = 0.0;
    }
    void Reset()
    {
        type = 1;
        x = 0.0;
        y = 0.0;
        heading = 0.0;    
    }
}TaskPoint;


typedef struct SensorStatus
{
    int     lidar_status;//激光雷达状态
    int     nav_status;//组合导航状态
    int     battery_soc;//电池电量
    int     traffic_light;//红绿灯状态
    int     turn_left_light_status;//左转向灯状态
    int     turn_right_light_status;//左转向灯状态
    int     head_lamp_low_beam_status;//前大灯近光
    int     head_lamp_high_beam_status;//前大灯远光
    int     can_status[2];//CAN连接状态，CAN102,CAN103
    int     mill_radar[5];//毫米波雷达状态
    int     ult_radar[12];//超声波雷达状态
    int     camera_status[5];//相机状态
    int     eps_status;//EPS状态
    int     vcu_status;//VCU状态
    int     nboost_status;//NBoost状态
    int     v2v_status;//1:停等让行，３：可以通行
    SensorStatus()
    {
        lidar_status = -1;
        nav_status = -1;
        battery_soc = 0;
        traffic_light = -1;
        turn_left_light_status = -1;
        turn_right_light_status = -1;
        head_lamp_low_beam_status = -1;
        head_lamp_high_beam_status = -1;
        eps_status = 0;
        vcu_status = 0;
        nboost_status = 0;
        v2v_status = 0;//1r,3g
        for (int i = 0; i < 2; ++i)
        {
            can_status[i] = -1;
        }
        for (int i = 0; i < 5; ++i)
        {
            mill_radar[i] = -1;
            camera_status[i] = -1;
        }
        for (int i = 0; i < 12; ++i)
        {
            ult_radar[i] = -1;
        }
    }
    void Reset()
    {
        lidar_status = -1;
        nav_status = -1;
        battery_soc = 0;
        traffic_light = -1;
        turn_left_light_status = -1;
        turn_right_light_status = -1;
        head_lamp_low_beam_status = -1;
        head_lamp_high_beam_status = -1;
        eps_status = 0;
        vcu_status = 0;
        nboost_status = 0;
        v2v_status = 0;//1r,3g
        for (int i = 0; i < 2; ++i)
        {
            can_status[i] = -1;
        }
        for (int i = 0; i < 5; ++i)
        {
            mill_radar[i] = -1;
            camera_status[i] = -1;
        }
        for (int i = 0; i < 12; ++i)
        {
            ult_radar[i] = -1;
        }   
    }
}SensorStatus;


typedef struct ControlStatus
{
    float   lat_targetangle;
    float   lat_targettorque;
    float   lon_targettorque;
    int     vehicle_shiftposition;
    int     vehicle_sys_status;
    int     actuatormode;
    int     shiftposition;
    int     brakelight;
    int     reverselight;
    int     epb_status;
    uint    PowerFault;
    uint    ComFault;
    uint    USSFault;
    uint    ChassisFault;
    bool    vehicle_auto_enable;
    bool    vehicle_lat_enable;
    bool    vehicle_lon_enable;
    bool    vehicle_epb_status;
    bool    road_vaild_flag;
    bool    calibration_flag;
    bool    write_enable_flag;
    ControlStatus()
    {
        lat_targetangle = 0;
        lat_targettorque = 0;
		lon_targettorque = 0;
        vehicle_shiftposition = 0;
        vehicle_sys_status = 0;
        actuatormode = 0;
        shiftposition = 0;
        brakelight = 0;
        reverselight = 0;
        epb_status = 0;
        PowerFault = 0;
        ComFault = 0;
        USSFault = 0;
        ChassisFault = 0;
        vehicle_auto_enable = false;
        vehicle_lat_enable = false;
        vehicle_lon_enable = false;
        vehicle_epb_status = false;
        road_vaild_flag = false;
        calibration_flag = false;
        write_enable_flag = false;
    }
    void Reset()
    {
        lat_targetangle = 0;
        lat_targettorque = 0;
		lon_targettorque = 0;
        vehicle_shiftposition = 0;
        vehicle_sys_status = 0;
        actuatormode = 0;
        shiftposition = 0;
        brakelight = 0;
        reverselight = 0;
        epb_status = 0;
        PowerFault = 0;
        ComFault = 0;
        USSFault = 0;
        ChassisFault = 0;
        vehicle_auto_enable = false;
        vehicle_lat_enable = false;
        vehicle_lon_enable = false;
        vehicle_epb_status = false;
        road_vaild_flag = false;
        calibration_flag = false;
        write_enable_flag = false;    
    }
}ControlStatus;

typedef struct BusinessStatus
{
    int     intensity4g;
    int     avos_status;//0:模块缺失；１：启动正常,-1:状态未知    
    int     perception;
    int     localization;
    int     planning;
    int     control;
    int     map_engine;
    string  map_id;
    string  avos_version;
    string  vehicle_vin;
    BusinessStatus()
    {
        intensity4g = 0;
        avos_status = -1;
        map_id = "";
        avos_version = "V0.0.0.0";
        vehicle_vin = "";
        perception = 0;//0:正常；２：异常;
        localization = 0;//0:正常；２：异常;
        planning = 0;//0:正常；２：异常;
        control = 0;//0:正常；２：异常;
        map_engine = 0;//0:正常；２：异常;
    }
    void Reset()
    {
        intensity4g = 0;  
        map_id = "";
        avos_version = "V0.0.0.0";
        vehicle_vin = "";
        avos_status = -1;
        perception = 0;//0:正常；２：异常;
        localization = 0;//0:正常；２：异常;
        planning = 0;//0:正常；２：异常;
        control = 0;//0:正常；２：异常;
        map_engine = 0;//0:正常；２：异常;
    }
}BusinessStatus;


typedef struct AutodriverStatus//0:未启动；１：已启动
{
    int perception;
    int localization;
    int planning;
    int control;
    int canbus;
    int business;
    int navigation;
    int lidar_32;
    int mm_lidar;
    int camera;
    AutodriverStatus()
    {
        perception = 0;
        localization = 0;
        planning = 0;
        control = 0;
        canbus = 0;
        business = 0;
        navigation = 0;
        lidar_32 = 0;
        mm_lidar = 0;
        camera = 0;
    }
    void Reset()
    {
        perception = 0;
        localization = 0;
        planning = 0;
        control = 0;
        canbus = 0;
        business = 0;
        navigation = 0;
        lidar_32 = 0;
        mm_lidar = 0;
        camera = 0;
    }
}AutodriverStatus;


typedef struct AvvsCommonInfo
{
    LocationStatus      location_info;
    MonitorInfo         monitor_info;
    MapCenter           map_center;
    SensorStatus        sensor_status;
    ControlStatus       control_status;
    BusinessStatus      business_status;
    SensorGPS           sensor_gps;
    AutodriverStatus    autodriver_status;
    void Reset(){
        location_info.Reset();
        monitor_info.Reset();
        map_center.Reset();
        sensor_status.Reset();
        control_status.Reset();
        business_status.Reset();
        sensor_gps.Reset();
        autodriver_status.Reset();
    }
}AvvsCommonInfo;


typedef struct AvvsCommonInfoShmArea
{
	ShmAreaHeader area_header;
	AvvsCommonInfo info;
} AvvsCommonInfoShmArea;

typedef struct AvvsCommonInfoShm
{
	ShmHeader header;
	AvvsCommonInfoShmArea info_area[2];
	// other area
} AvvsCommonInfoShm;

// typedef struct PercetionObjectsShmArea
// {
// 	ShmAreaHeader area_header;
// 	PercetionObjects objects;
// } PercetionObjectsShmArea;

// typedef struct PercetionShm
// {
// 	ShmHeader header;
// 	PercetionObjectsShmArea objects_area[2];
// 	// other area
// } PercetionShm;

class PerceptionObjectMem
{
public:
	PercetionShm *objs_mem_ptr;
	int object_shmid_ ;
	void *object_shmaddr;
	std::string topic;
	int read_seq;
private:
	PerceptionObjectMem() {
		objs_mem_ptr = nullptr;
		object_shmaddr = nullptr;
		object_shmid_ = -1;
		read_seq = 0;
	}

public:
	static PerceptionObjectMem *Instance() {
		static PerceptionObjectMem *instance = nullptr;
		if (!instance) {
			instance = new PerceptionObjectMem();
		}
		return instance;
	}

	// BASE_DECLARE_SINGLETON(PerceptionObjectMem)

};

class AvvsCommonInfoMem
{
public:
	AvvsCommonInfoShm *info_mem_ptr;
	int info_shmid_ ;
	void *info_shmaddr;
	std::string topic;
	int read_seq;
private:
	AvvsCommonInfoMem() {
		info_mem_ptr = nullptr;
		info_shmaddr = nullptr;
		info_shmid_ = -1;
		read_seq = 0;
	}

public:
	static AvvsCommonInfoMem *Instance() {
		static AvvsCommonInfoMem *instance = nullptr;
		if (!instance) {
			instance = new AvvsCommonInfoMem();
		}
		return instance;
	}

	// BASE_DECLARE_SINGLETON(AvvsCommonInfoMem)

};












#endif