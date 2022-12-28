cmake_minimum_required(VERSION 2.8.3)
project(ivcanbus)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
)

add_message_files(
  FILES
  actuatorstate.msg
  batterystate.msg
  businesssensorstate.msg
  hardwarefaults.msg
  ivcontrolfeedback.msg
  ivcansensor.msg
  ivultrasonicradar.msg
  ivwheelspeed.msg
  ivsidesweepingradar.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs
)