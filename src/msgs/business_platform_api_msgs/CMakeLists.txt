cmake_minimum_required(VERSION 2.8.3)
project(business_platform_api_msgs)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs  
  sensor_msgs
  common_msgs
  message_generation
   
) 
  
include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)


 add_message_files(
    FILES
    AutomaticAdditionBusiness.msg
    AutomaticBehaviorLimit.msg
    AutomaticTaskConfig.msg
    AutomaticTaskDetail.msg  
    AutomaticTaskInput.msg 
    AutomaticTaskStatus.msg
    AvosConfigParamInfo.msg 
    AvosConfigParam.msg 
    AvosConfigParamQueryResult.msg
    AvosServiceReply.msg
    BusinessCloudInfo.msg

    BagDeleteCommand.msg
    BagInfo.msg
    BagListInfo.msg
    BagrecordTopicInfo.msg
    BagrecordTopicSetting.msg

    CalibrationCommand.msg
    CalibrationParam.msg
    CalibrationStatus.msg

    FileListCommand.msg
    FileInfo.msg
    FileListResult.msg
    FileOperation.msg
            
    GlobalSelfdriveInfo.msg
    GlobalPathStatus.msg

    GlobalMessagSupportCarlist.msg
        
    MapAreaInfo.msg
    MapCollectionCommand.msg
    MapCollectionInfoList.msg
    MapLoadCollection.msg
    MapCollectionInfo.msg
    MapCollection.msg
    MapCollectionStatus.msg
    MapDeleteCommand.msg
    MapLineInfo.msg
    MapPointInfo.msg
    MapTagCommand.msg
    
    NodeResource.msg
       
    SpecialFunctionPoint.msg

    RoadPoint.msg
          
    VehicleBehaviorStatus.msg
    VehicleDeviceStatus.msg
    VehicleDriveStatus.msg
    VehicleLocalization.msg
    VehicleResouceStatus.msg

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
