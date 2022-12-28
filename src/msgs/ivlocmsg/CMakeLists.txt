cmake_minimum_required(VERSION 2.8.3)
project(ivlocmsg)

#set(CMAKE_BUILD_TYPE "Release")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs	
  sensor_msgs
  message_generation
  geometry_msgs
  common_msgs
)


add_message_files(
    FILES
    ndt_status.msg
    ivmsgeuler.msg
    ivcalib.msg
    ivgmminit.msg
    ivgmminitpose.msg
    comfusion.msg
    comgnss.msg
    ivlidarmatching.msg
    ivmsglocpos.msg
    ivsensorgps.msg
    ivsensorimu.msg
    ivsensorodom.msg
    #SensorImu.msg
    #LocalizationControl.msg
 )

 generate_messages(
   DEPENDENCIES
   std_msgs
   geometry_msgs
   common_msgs
 )

catkin_package(
   CATKIN_DEPENDS roscpp rospy std_msgs
   message_runtime
   common_msgs
   geometry_msgs
)

include_directories(include 
   ${catkin_INCLUDE_DIRS}
)  
