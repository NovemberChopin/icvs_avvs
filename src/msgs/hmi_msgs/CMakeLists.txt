cmake_minimum_required(VERSION 2.8.3)
project(hmi_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
)

add_message_files(
  FILES
  AdditionCmd.msg
  CarStatus.msg
  MissionStatus.msg
  NavigationRoad.msg
  BussinessDemand.msg
  MissionPlanMsg.msg
  NavigationLane.msg
  StopPoint.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
)

catkin_package(
#INCLUDE_DIRS
# ${CMAKE_SOURCE_DIR}/../devel/include/hmi_msgs
#  ${common_msgs_INCLUDE_DIRS}
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs common_msgs
)

#include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)
