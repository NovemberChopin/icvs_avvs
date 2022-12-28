cmake_minimum_required(VERSION 2.8.3)
project(canbus_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
)

add_message_files(
  FILES
  CanDebug.msg
  CanInfo.msg
  CanSensor.msg
  CanMmwSensor.msg
  Ultrasonicradar.msg
  CanFault.msg
  MmwLidar.msg
  Ars408Obj.msg
  SensorArs408Obj.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs
)

