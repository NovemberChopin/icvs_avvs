cmake_minimum_required(VERSION 2.8.3)
project(perception_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  geometry_msgs
  sensor_msgs
  common_msgs
)

add_message_files(
  FILES
  ivmsglocpos.msg
  ivmsglocalization.msg
  ivultrasonicradar.msg
  probcell.msg
  probcells.msg
  visionobj.msg
  Object.msg
  ObstacleCell.msg
  PerceptionObjects.msg
  ParkingSpace.msg
  RadarObjectList.msg
  RadarObject.msg
  PerceptionEnvironment.msg
  PerceptionLane.msg
  PerceptionRoad.msg
  SingleTrafficLight.msg
  TrafficLightDetection.msg
  UltraCell.msg
  UltrasonicParking.msg
  UnsureVar.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  geometry_msgs
  sensor_msgs
  common_msgs
)

catkin_package(
#INCLUDE_DIRS
# ${CMAKE_SOURCE_DIR}/../devel/include/perception_msgs
#  ${common_msgs_INCLUDE_DIRS}
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs geometry_msgs sensor_msgs common_msgs
)
