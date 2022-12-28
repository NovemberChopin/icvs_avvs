cmake_minimum_required(VERSION 2.8.3)
project(control_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
) 

add_message_files(
  FILES
  ControlState.msg
  Latcontrol.msg
  Loncontrol.msg
  LoncontrolFeedback.msg
  LatcontrolFeedback.msg
  LoncontrolDebug.msg
  LatcontrolDebug.msg
  LoncontrolDebugPT.msg
  LatcontrolDebugPT.msg
  control_sys.msg
  Params_FB.msg
  Object.msg
  Cell.msg
  Obstacles.msg
  chassis_status.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
)

catkin_package(
#INCLUDE_DIRS
# ${CMAKE_SOURCE_DIR}/../devel/include/control_msgs
#  ${common_msgs_INCLUDE_DIRS}
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs common_msgs
)

#include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)
