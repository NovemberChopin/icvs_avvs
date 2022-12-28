#ifndef __ERROR_CODE_DEFINE_H__
#define __ERROR_CODE_DEFINE_H__


/*--------------------------Driver module error codes start from here-----------------------------------------------*/
#define DFPERCEPTION_LIDAR_HARDWARE_0_ERROR          0x0000           // velodyne_packets_0 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_1_ERROR          0x0001           // lidar_points_0 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_2_ERROR          0x0002           // velodyne_packets_1 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_3_ERROR          0x0003           // lidar_points_1 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_4_ERROR          0x0004           // velodyne_packets_2 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_5_ERROR          0x0005           // lidar_points_2 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_6_ERROR          0x0006           // velodyne_packets_3 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_7_ERROR          0x0007           // lidar_points_3 no data or out of time limit!
#define DFPERCEPTION_LIDAR_HARDWARE_8_ERROR          0x0008           // lidar_points_0 synchronized failed!
#define DFPERCEPTION_LIDAR_HARDWARE_9_ERROR          0x0009           // lidar_points_1 synchronized failed!
#define DFPERCEPTION_LIDAR_HARDWARE_10_ERROR         0x000a           // lidar_points_2 synchronized failed!
#define DFPERCEPTION_LIDAR_HARDWARE_11_ERROR         0x000b           // lidar_points_3 synchronized failed!

#define DFPERCEPTION_VISION_HARDWARE_0_ERROR         0x0100           // No.0(06mm) camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_1_ERROR         0x0101           // No.0(06mm) camera no data or out of time limit!
#define DFPERCEPTION_VISION_HARDWARE_2_ERROR         0x0102           // No.1(25mm) camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_3_ERROR         0x0103           // No.1(25mm) camera no data or out of time limit!

#define DFPCICAN_HARDWARE_0_ERROR                    0x0200           // read mcu data failed!
#define DFPCICAN_HARDWARE_1_ERROR                    0x0201           // can busoff!

#define DFPERCEPTION_VISION_HARDWARE_4_ERROR         0x0300           // No.0 surround view camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_5_ERROR         0x0301           // No.0 surround view camera no data or out of time limit!
#define DFPERCEPTION_VISION_HARDWARE_6_ERROR         0x0302           // No.1 surround view camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_7_ERROR         0x0303           // No.1 surround view camera no data or out of time limit!
#define DFPERCEPTION_VISION_HARDWARE_8_ERROR         0x0304           // No.2 surround view camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_9_ERROR         0x0305           // No.2 surround view camera no data or out of time limit!
#define DFPERCEPTION_VISION_HARDWARE_10_ERROR        0x0306           // No.3 surround view camera init fail!
#define DFPERCEPTION_VISION_HARDWARE_11_ERROR        0x0307           // No.3 surround view camera no data or out of time limit!

#define DFPERCEPTION_ULTRASONIC_HARDWARE_0_ERROR     0x0400           // ultrasonicraw(id 0) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_1_ERROR     0x0401           // ultrasonicraw(id 1) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_2_ERROR     0x0402           // ultrasonicraw(id 2) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_3_ERROR     0x0403           // ultrasonicraw(id 3) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_4_ERROR     0x0404           // ultrasonicraw(id 4) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_5_ERROR     0x0405           // ultrasonicraw(id 5) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_6_ERROR     0x0406           // ultrasonicraw(id 6) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_7_ERROR     0x0407           // ultrasonicraw(id 7) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_8_ERROR     0x0408           // ultrasonicraw(id 8) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_9_ERROR     0x0409           // ultrasonicraw(id 9) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_10_ERROR    0x040a           // ultrasonicraw(id 10) no data!
#define DFPERCEPTION_ULTRASONIC_HARDWARE_11_ERROR    0x040b           // ultrasonicraw(id 11) no data!

//--------------------------------------------------------------------------------------------------------------------


/*--------------------------Perception module error codes start from here-------------------------------------------*/

#define DFPERCEPTION_LIDARPROCESS_HARDWARE_0_ERROR   0x1000           // lidar_points_0 ring dificiency!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_1_ERROR   0x1001           // lidar_points_1 ring dificiency!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_2_ERROR   0x1002           // lidar_points_2 ring dificiency!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_3_ERROR   0x1003           // lidar_points_3 ring dificiency!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_4_ERROR   0x1004           // lidar_points_0 data out of time!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_5_ERROR   0x1005           // lidar_points_1 data out of time!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_6_ERROR   0x1006           // lidar_points_2 data out of time!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_7_ERROR   0x1007           // lidar_points_3 data out of time!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_8_ERROR   0x1008           // lidar_points_0 covered!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_9_ERROR   0x1009           // lidar_points_1 covered!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_10_ERROR  0x100a           // lidar_points_2 covered!
#define DFPERCEPTION_LIDARPROCESS_HARDWARE_11_ERROR  0x100b           // lidar_points_3 covered!

#define DFPERCEPTION_LIDARPROCESS_SOFTWARE_0_ERROR   0x1010           // localization data out of time!
#define DFPERCEPTION_LIDARPROCESS_SOFTWARE_1_ERROR   0x1011           // map data out of time!
#define DFPERCEPTION_LIDARPROCESS_SOFTWARE_2_ERROR   0x1012           // out of freespace!

#define DFPERCEPTION_VISION_SOFTWARE_0_ERROR         0x1100           // No.0 camera out of time!
#define DFPERCEPTION_VISION_SOFTWARE_1_ERROR         0x1101           // No.1 camera out of time!

#define DFPERCEPTION_IMAGEPROCESS_0_ERROR            0x1110           // Loading network model failed!
#define DFPERCEPTION_IMAGEPROCESS_1_ERROR            0x1111           // Loading the original network model failed!
#define DFPERCEPTION_IMAGEPROCESS_2_ERROR            0x1112           // No network model specified!

#define DFPERCEPTION_ULTRASONIC_SOFTWARE_0_ERROR     0x1300           // ultrasonic data error!
#define DPPERCEPTION_ARS_0_HARDWARE_0_ERROR          0x1301 		  // ars_0 no data or out of time limit!
#define DPPERCEPTION_ARS_1_HARDWARE_1_ERROR          0x1302           // ars_1 no data or out of time limit!
#define DPPERCEPTION_DENSO_0_HARDWARE_0_ERROR        0x1303 		  // denso_0 no data or out of time limit!
#define DPPERCEPTION_DENSO_1_HARDWARE_1_ERROR        0x1304           // denso_1 no data or out of time limit!
#define DPPERCEPTION_MATRIX_HARDWARE_0_ERROR         0x1305 		  // matrix no data or out of time limit!
#define DPPERCEPTION_MATRIX_SOFTWARE_0_ERROR         0x1306           // matrix data error!
#define DFPERCEPTION_MOBILEYE_HARDWARE_0_ERROR       0x1307           // mobileye no data!
#define DFPERCEPTION_MOBILEYE_SOFTWARE_0_ERROR       0x1308           // meprocess no data or out of time limit!

#define DFPERCEPTION_OBJECTFUSION_0_ERROR            0x1310           // object id multi detect!

//--------------------------------------------------------------------------------------------------------------------

/*--------------------------Localization module error codes start from here------------------------------------------*/
#define DFLOCALIZATION_IMU_HARDWARE_0_ERROR          0x2000           // BBox open nabbox serial port fail！
#define DFLOCALIZATION_IMU_HARDWARE_1_ERROR          0x2001           // BBox read nabox seoial fail!
#define DFLOCALIZATION_NAVBOX_HARDWARE_2_ERROR       0x2002           // IMU communication is error!
#define DFLOCALIZATION_NAVBOX_HARDWARE_3_ERROR       0x2003           // Imu gyro is abnormal!
#define DFLOCALIZATION_NAVBOX_HARDWARE_4_ERROR       0x2004           // Imu acc is abnormal!
#define DFLOCALIZATION_NAVBOX_HARDWARE_5_ERROR       0x2005           // Imu temperature is abnormal!
#define DFLOCALIZATION_NAVBOX_HARDWARE_6_ERROR       0x2006           // GPS communication is error!
#define DFLOCALIZATION_NAVBOX_HARDWARE_7_ERROR       0x2007           // GPS data is abnormal!
#define DFLOCALIZATION_NAVBOX_HARDWARE_8_ERROR       0x2008           // Antenna of GPS A is not connected!
#define DFLOCALIZATION_NAVBOX_HARDWARE_9_ERROR       0x2009           // Antenna of GPS B is not connected!
#define DFLOCALIZATION_NAVBOX_HARDWARE_A_ERROR       0x200a           // Odom communication is error!
#define DFLOCALIZATION_NAVBOX_HARDWARE_B_ERROR       0x200b           // Odom left wheel speed is abnormal
#define DFLOCALIZATION_NAVBOX_HARDWARE_C_ERROR       0x200c           // Odom right wheel speed is abnormal
#define DFLOCALIZATION_NAVBOX_HARDWARE_D_ERROR       0x200d           // Odom gear is abnormal
#define DFLOCALIZATION_NAVBOX_HARDWARE_E_ERROR       0x200e           // Lidar communication is error!
#define DFLOCALIZATION_NAVBOX_HARDWARE_F_ERROR       0x200f           // Lidar data is abnormal
#define DFLOCALIZATION_NAVBOX_HARDWARE_G_ERROR       0x20a0           // 4G module is abnormal

#define DFLOCALIZATION_LOCPOSE_0_ERROR               0x2010           // Locaization is fail!
#define DFLOCALIZATION_LIDAR_SOFTWARE_0_ERROR        0x2011           //Input cloud is empty or invalid
#define DFLOCALIZATION_LIDAR_SOFTWARE_1_ERROR        0x2012           //Lidar matching keeps initial
#define DFLOCALIZATION_LIDAR_SOFTWARE_2_ERROR        0x2013           //Lidar matching params can not be read 
#define DFLOCALIZATION_LIDAR_SOFTWARE_3_ERROR        0x2014           //Map route is not existing
#define DFLOCALIZATION_LIDAR_SOFTWARE_4_ERROR        0x2015           //Map is received but contents absent
#define DFLOCALIZATION_LIDAR_SOFTWARE_5_ERROR        0x2016           //The params for map loading is wrong
#define DFLOCALIZATION_LIDAR_SOFTWARE_6_ERROR        0x2017           //GMM high resolution map is error
#define DFLOCALIZATION_LOCPOSE_7_ERROR               0x2018           //Navbox assemble params check fail
#define DFLOCALIZATION_LOCPOSE_8_ERROR               0x2019           //Navbox baseline calibration params check fail
#define DFLOCALIZATION_LOCPOSE_9_ERROR               0x201a           //Navbox odom calibration params check fail
#define DFLOCALIZATION_NAVBOX_SOFTWARE_A_ERROR       0x201b           //IMU gyro initial error is large
#define DFLOCALIZATION_NAVBOX_SOFTWARE_B_ERROR       0x201c           //Imu acc initial error is large
#define DFLOCALIZATION_NAVBOX_SOFTWARE_C_ERROR       0x201d           //GPS differential age over time
#define DFLOCALIZATION_NAVBOX_SOFTWARE_D_ERROR       0x201e           //GPS data is over time
#define DFLOCALIZATION_NAVBOX_SOFTWARE_E_ERROR       0x201f           //Lidar data is over time
#define DFLOCALIZATION_NAVBOX_SOFTWARE_F_ERROR       0x20b0           //Odom is over time 
#define DFLOCALIZATION_NAVBOX_SOFTWARE_G_ERROR       0x20b1           //Fusion error is large and dr time is over time 
#define DFLOCALIZATION_NAVBOX_SOFTWARE_H_ERROR       0x20b2           //GPS heading is abnormal
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------------------

/*--------------------------Missionplan module error codes start from here------------------------------------------*/
#define DFPLANNING_MISSIONPLAN_SOFTWARE_0_ERROR      0x2000           // routeplan no data or out of time limit!
#define DFPLANNING_MISSIONPLAN_SOFTWARE_1_ERROR      0x2001           // statuspredict out of range!
#define DFPLANNING_PLANNING_SOFTWARE_0_ERROR         0x3010           // mapengine data delay
#define DFPLANNING_PLANNING_SOFTWARE_1_ERROR         0x3011           // mapengine data cut out
#define DFPLANNING_PLANNING_SOFTWARE_2_ERROR         0x3012           // perception data delay           
#define DFPLANNING_PLANNING_SOFTWARE_3_ERROR         0x3013           // perception data cut out
#define DFPLANNING_PLANNING_SOFTWARE_4_ERROR         0x3014           // task data delay
#define DFPLANNING_PLANNING_SOFTWARE_5_ERROR         0x3015           // task data cut out
#define DFPLANNING_PLANNING_SOFTWARE_6_ERROR         0x3016           // localization jump
#define DFPLANNING_PLANNING_SOFTWARE_7_ERROR         0x3017           // current point cannot auto drive
#define DFPLANNING_PLANNING_SOFTWARE_8_ERROR         0x3018           // avoid failure
#define DFPLANNING_PLANNING_SOFTWARE_9_ERROR         0x3019           // current loc is in collision
#define DFPLANNING_PLANNING_SOFTWARE_10_ERROR        0x301A           // imu data delay 
#define DFPLANNING_PLANNING_SOFTWARE_11_ERROR        0x301B           // imu data cut out
//--------------------------------------------------------------------------------------------------------------------



/*--------------------------mapengin module error codes start from here-------------------------------------------*/
#define READMAP_ERROR                                0x4010
#define GLOBAL_ROUTING_ERROR                         0x4011
#define NAVIGATION_START_ERROR                       0x4012
#define NAVIGATION_RUNNING_ERROR                     0x4013
#define SET_AVOIDING_IN_NAVIGATION_ERROR             0x4014
//--------------------------------------------------------------------------------------------------------------------


/*--------------------------Control module error codes start from here-------------------------------------------*/
#define DFCONTROL_HARDWARE_0_ERROR                   0x5000           // motor drive fault
#define DFCONTROL_HARDWARE_1_ERROR                   0x5001           // motor brake fault
#define DFCONTROL_HARDWARE_2_ERROR                   0x5002           // cannot shiftposition fault
#define DFCONTROL_HARDWARE_3_ERROR                   0x5003           // eps angular position sensor fault
#define DFCONTROL_HARDWARE_4_ERROR                   0x5004           // spi communicate fault
#define DFCONTROL_HARDWARE_5_ERROR                   0x5005           // eeprom vehicle params error

#define DFCONTROL_SOFTWARE_0_ERROR                   0x5010           // receive planning topic delay
#define DFCONTROL_SOFTWARE_1_ERROR                   0x5011           // lat error big
#define DFCONTROL_SOFTWARE_2_ERROR                   0x5012           // planning topic not update
#define DFCONTROL_SOFTWARE_3_ERROR                   0x5013           // eeprom vehicle params diff from config
#define DFCONTROL_SOFTWARE_4_ERROR                   0x5014           // eeprom wheel_center diff from config
//--------------------------------------------------------------------------------------------------------------------


#define DFVEHICLE_EPS_0_ERROR     0x7000
#define DFVEHICLE_EPS_1_ERROR     0x7001

#define DFVEHICLE_BRAKE_0_ERROR   0x7010
#define DFVEHICLE_BRAKE_1_ERROR   0x7011

#define DFVEHICLE_DRIVE_0_ERROR   0x7020

#define DFVEHICLE_EPB_0_ERROR     0x7030
#define DFVEHICLE_EPB_1_ERROR     0x7031

#define DFVEHICLE_POWER_0_ERROR   0x7040
#define DFVEHICLE_POWER_1_ERROR   0x7041
#define DFVEHICLE_POWER_2_ERROR   0x7042
#define DFVEHICLE_POWER_3_ERROR   0x7043

#define DFVEHICLE_CPU_0_ERROR     0x7050
#define DFVEHICLE_VCU_1_ERROR     0x7051


/*--------------------------Businee module error codes start from here----------------------------------------------*/
#define DFMONITOR_APP_CPU_HGIH_ERROR                 0x6000              // cpu of nodes high!
#define DFMONITOR_CLOUDSERVICE_CPU_HGIH_ERROR        0x6001
#define DFMONITOR_OBJECTFUSION_CPU_HGIH_ERROR        0x6002
#define DFMONITOR_LOCFUSION_CPU_HGIH_ERROR           0x6003
//--------------------------------------------------------------------------------------------------------------------

#endif
