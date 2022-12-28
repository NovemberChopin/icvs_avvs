cmake_minimum_required(VERSION 2.8.3)
project(task_msgs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs  
  sensor_msgs
  common_msgs
  message_generation
) 

 add_message_files(
    FILES 
    BusinessStatus.msg
    MissionPoint.msg
    VehicleDriveStatus.msg
    CalibrationParam.msg
    CalibrationStatus.msg
    MissionSequence.msg
    VehicleLocpos.msg
    FaultInfo.msg
    MissionStatus.msg
    VehicleResourceInfo.msg
    MapCollection.msg
    NavigationCommand.msg
    VehicleSensorStatus.msg
    MissionAction.msg
    PathConstraint.msg
    VehicleStatus.msg
    MissionContent.msg
    SubSystemStatus.msg
    VehicleSystemStatus.msg 
    MissionLine.msg
    VehicleBehavior.msg

    SpecialFunctionPoint.msg

    ControlboxToTask.msg
    TaskToControlbox.msg

    PerceptionCalibration.msg

    CanbusSignal.msg
    TaskCanbusSignal.msg

    BoardResource.msg
    NodeResource.msg

 ) 

 generate_messages(
   DEPENDENCIES
   std_msgs
   sensor_msgs
   common_msgs
 )

catkin_package(
   CATKIN_DEPENDS roscpp rospy std_msgs sensor_msgs common_msgs
   message_runtime
)

include_directories(include 
   ${catkin_INCLUDE_DIRS}
)  
