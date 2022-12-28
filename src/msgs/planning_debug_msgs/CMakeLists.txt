cmake_minimum_required(VERSION 2.8.3)  
project(planning_debug_msgs)  

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
  common_msgs
  planning_msgs
  perception_msgs
)

add_message_files(
  FILES
  #debug msg  
  PlanningDebug.msg
  DebugState.msg
 
  DebugCognition.msg
  DebugDecision.msg
  DebugMotionPlan.msg
  DebugPrediction.msg
  DebugLine.msg
  STGraph.msg
  Sentence.msg
  DecisionPoint.msg
  DebugSTGraph.msg
  DebugImu.msg

  # spcical car
  motiondebug.msg
  DebugRefpath.msg
  DebugDwa.msg
  DebugAstar.msg
  DebugSemantic.msg
  DebugUnsCollid.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
  common_msgs
  planning_msgs
  perception_msgs
)

catkin_package(
 CATKIN_DEPENDS message_generation roscpp rospy std_msgs common_msgs planning_msgs perception_msgs
)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)