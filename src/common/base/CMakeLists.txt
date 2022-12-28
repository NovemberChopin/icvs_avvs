cmake_minimum_required(VERSION 2.8.3)
project(common_base)

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/../devel/include/proto/)
find_package(catkin REQUIRED COMPONENTS 
    roscpp 
    genmsg 
    std_msgs 
     
     
    )

catkin_package(
  INCLUDE_DIRS  
    ./
    ../common_define
    communication/include
    data_pool/include
    log/include
    thread_pool/include
    time/include
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../devel/include/proto
  LIBRARIES 
    ${PROJECT_NAME}_time
  CATKIN_DEPENDS roscpp roscpp genmsg gencpp genpy std_msgs  
)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)


include_directories(
  log/include/
  time/include/
  config/include/
  /usr/include/eigen3/
  communication/include
  ${CMAKE_SOURCE_DIR}/../devel/include/
  ${CMAKE_SOURCE_DIR}/../devel/include/proto
)

link_directories(
  common/common_lib/glog/lib
  ${CMAKE_SOURCE_DIR}/common/common_lib
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/../devel/lib/
)


add_library(${PROJECT_NAME}_time 
  time/src/duration.cc
  time/src/node_time.cc
  time/src/rate.cc
  config/src/param_config_manager.cpp
)

target_link_libraries(${PROJECT_NAME}_time
    ${catkin_LIBRARIES}
    ${OpenCV_LIBRARIES}
    glog
    proto_proto
)



# add_executable(service_bridge communication/test/service_bridge_test.cpp)
# add_dependencies(service_bridge ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(service_bridge
#   ${catkin_LIBRARIES}
# )

# add_executable(client_bridge communication/test/client_bridge_test.cpp)
# add_dependencies(client_bridge ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
# target_link_libraries(client_bridge
#   ${catkin_LIBRARIES}
# )

