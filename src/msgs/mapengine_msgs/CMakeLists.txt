cmake_minimum_required(VERSION 2.8.3)
project(mapengine_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_srvs
  std_msgs
  common_msgs
)

add_message_files(
  FILES
  ReferencePoint.msg
  Point.msg
  DisRange.msg
  LaneList.msg
  LaneLists.msg
  MapInfo.msg
  Navigation.msg
  BusinessArea.msg
  NavMission.msg
  MissionContent.msg
  MissionLine.msg 
  MissionArea.msg
  MissionPoint.msg
  MissionRoute.msg
  RoutePoint.msg
  SimSts.msg 
  TrafficLight.msg 
  ImuInfo.msg
)

add_service_files(
  FILES
  NavCmd.srv
  GlobalPath.srv
  SimDrive.srv
)

generate_messages(
  DEPENDENCIES
  std_srvs
  std_msgs
  common_msgs
)

catkin_package(
#  INCLUDE_DIRS  
#  ${CMAKE_SOURCE_DIR}/../devel/include/mapengine_msgs
#  ${common_msgs_INCLUDE_DIRS}
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs common_msgs
)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)
