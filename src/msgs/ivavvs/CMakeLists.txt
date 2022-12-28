cmake_minimum_required(VERSION 2.8.3)
project(ivavvs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs  
  sensor_msgs
  message_generation
  common_msgs
  std_srvs
)

 add_message_files(
    FILES
    avvs2record.msg
    ivbusiness.msg
    params.msg
    ivcalibration.msg
    ivmapacq.msg
    ivota.msg
    ivselfdriving.msg
    tpavvs.msg
    fileinfo.msg
    topicinfo.msg
 )

add_service_files(
  FILES
  ParamConfig.srv
  ParamConfigCheck.srv
  ParamFlash.srv
  FileListRequest.srv
  FileListDelete.srv
  RecordBag.srv
  DefaultMapConfig.srv
  TopicConfig.srv
)

 generate_messages(
   DEPENDENCIES
   std_msgs
   common_msgs
   std_srvs
 )

catkin_package(
   CATKIN_DEPENDS roscpp rospy std_msgs
   message_runtime
)

include_directories(include 
   ${catkin_INCLUDE_DIRS}
)  
