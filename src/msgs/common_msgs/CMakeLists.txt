cmake_minimum_required(VERSION 2.8.3)
project(common_msgs)

find_package(catkin REQUIRED COMPONENTS
  message_generation
  roscpp
  rospy
  std_msgs
)

add_message_files(
  FILES
  EulerWithCovariance.msg
  LLH.msg
  NavStatus.msg
  Point2D.msg
  Point3D.msg
  PointENU.msg
  Pose.msg
  PoseEuler.msg
  PoseQuaternion.msg
  Quaternion.msg
  Twist.msg
  UnsurePoint.msg
  UnsurePosition.msg
  UnsureVar.msg
  Vector3.msg
  Vector3WithCovariance.msg
  WheelSpeed.msg
  AvosClock.msg
  AvosTime.msg
  FaultInfo.msg
  FaultVec.msg
  Header.msg
  TimeStatistics.msg
  TimeStatus.msg
  DRPoseWithTime.msg
  fault_info.msg
  fault_vec.msg
  time_statistics.msg
)

add_service_files(
  FILES
  AvosService.srv
)

generate_messages(
  DEPENDENCIES
  std_msgs
)

catkin_package(
#  INCLUDE_DIRS  ${CMAKE_SOURCE_DIR}/../devel/include/common_msgs
  CATKIN_DEPENDS message_generation roscpp rospy std_msgs
)

