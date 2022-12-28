cmake_minimum_required(VERSION 2.8.3)
project(business_platform)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  sensor_msgs
  common_msgs
  business_platform_api_msgs
)

 add_message_files(
    FILES
    BusinessControl.msg
    BusinessStatus.msg      
    SelfDriveBehaviorStatus.msg  
    VehicleResouceStatus.msg
    GlobalSelfdriveInfo.msg  
    VehicleDeviceStatus.msg
    Locpos.msg               
    VehicleDriveStatus.msg 
    BoardResourceStatus.msg    
    MonitorItem.msg
    MonitorTable.msg
    ModuleMonitor.msg
    HmiObject.msg
    HmiPoint.msg
    HmiStatus.msg
    BusinessPlatformDebug.msg
 ) 

generate_messages(
  DEPENDENCIES
  std_msgs
  sensor_msgs
  common_msgs
  business_platform_api_msgs
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs
)

##include(/opt/ros/kinetic/share/catkin/cmake/env/CMakeLists.txt)
