cmake_minimum_required(VERSION 2.8.3)
project(mainstream_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
  planning_msgs
  control_msgs
  hmi_msgs
  perception_msgs
  imu
  canbus_msgs
  ivcanbus
  ivlocmsg
)

add_message_files(
  FILES
  Business.msg
  BusinessFeedback.msg
  SystemControl.msg
  SystemOverall.msg
  SystemState.msg
  VehiclePerception.msg
  ivmsglocalization.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
  planning_msgs
  control_msgs
  hmi_msgs
  perception_msgs
  imu
  canbus_msgs
  ivcanbus
  ivlocmsg
)

catkin_package(
  # INCLUDE_DIRS  
  # ${CMAKE_SOURCE_DIR}/../devel/include/mainstream_msgs
  # ${common_msgs_INCLUDE_DIRS}
  # ${planning_msgs_INCLUDE_DIRS}
  # ${control_msgs_INCLUDE_DIRS}
  # ${hmi_msgs_INCLUDE_DIRS}
  # ${perception_msgs_INCLUDE_DIRS}
  # ${imu_INCLUDE_DIRS}
  # ${canbus_msgs_INCLUDE_DIRS}
  CATKIN_DEPENDS message_generation roscpp rospy std_msgs
  common_msgs
  planning_msgs
  control_msgs
  hmi_msgs
  perception_msgs
  imu
  canbus_msgs
  ivcanbus
  ivlocmsg
)