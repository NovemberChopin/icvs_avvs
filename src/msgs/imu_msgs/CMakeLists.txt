cmake_minimum_required(VERSION 2.8.3)
project(imu)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  genmsg
  std_msgs
  serial
  common_msgs
)

add_message_files(
  FILES
  filter_status.msg 
  SensorImu.msg
  ivsensorgps.msg
  ivsensorimu.msg
  ivwheelspeed.msg
  query_calibration.msg
  navbox_calibset.msg
  assembleparams.msg
  gnsscalibparams.msg
  navcalibparams.msg
  navbox_calibprocess.msg
  LocalizationControl.msg 
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs
)


