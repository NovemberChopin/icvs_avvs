cmake_minimum_required(VERSION 2.8.3)
project(prediction_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
  
)

add_message_files(
  FILES
  PredictionTrajectoryPoint.msg
  PredictionTrajectory.msg
  PredictionObject.msg
  PredictionObjects.msg
  DebugState.msg
  PredictionDebug.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
  
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs common_msgs
)