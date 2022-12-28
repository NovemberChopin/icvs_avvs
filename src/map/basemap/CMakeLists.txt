cmake_minimum_required(VERSION 2.8.3)
project(basemap)

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/common/cmake")
SET(PROTOBUF_INCLUDE_DIR ${CMAKE_SOURCE_DIR}) 

set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
find_package(catkin REQUIRED COMPONENTS roscpp genmsg std_msgs roslib)
find_package(GFLAGS REQUIRED)
find_package(OpenCV REQUIRED)

find_package(catkin REQUIRED COMPONENTS
    message_generation
    genmsg
    roscpp
    rospy
    std_msgs
    )
generate_messages(DEPENDENCIES  std_msgs)

catkin_package(
  CATKIN_DEPENDS roscpp
  DEPENDS roscpp genmsg gencpp genpy  std_msgs
  LIBRARIES ${PROJECT_NAME}_lib
)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)

include_directories(
  include
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/common/common_header
  ${CMAKE_SOURCE_DIR}/../devel/include/
  ${CMAKE_SOURCE_DIR}/../devel/include/proto/
  ${GFLAGS_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)


link_directories(
  common/common_lib/glog/lib
  ${CMAKE_SOURCE_DIR}/common/common_lib
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/../devel/lib/
)

FILE(GLOB SRCFILES ${CMAKE_SOURCE_DIR}/src/*.cpp)

list(APPEND PROTO_INCLUDE_DIRS ${CMAKE_SOURCE_DIR})# put this one front add_executable dont put it to the top of the file 
set(utilcpp
    ${CMAKE_SOURCE_DIR}/common/util/file.cpp
    )
add_library(${PROJECT_NAME}_lib
  src/basemap.cc 
  src/distance.cc
  protocol_parser/png_parser.cc 
  shm_dynamic/shm_parser.cc 
  shm_dynamic/basemap_shm_util.cc
  shm_dynamic/shm_update_util.cc
  ${utilcpp}
  )
target_link_libraries(${PROJECT_NAME}_lib
  ${catkin_LIBRARIES}
  ${GFLAGS_LIBRARIES}
  ${OpenCV_LIBRARIES}
  proj
  glog
  geotool
)
add_dependencies(${PROJECT_NAME}_lib
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
)

# add_executable(png_parser_test protocol_parser/png_parser_test.cc)
# target_link_libraries(png_parser_test
#   ${PROJECT_NAME}
#   proto_proto
#   # gtest
# )

# add_executable(shm_test shm_dynamic/shm_test.cc
# )
# target_link_libraries(shm_test
#   geotool
#   ${OpenCV_LIBRARIES}
#   ${PROJECT_NAME}
#   # gtest
#   map_loader
# )
# add_executable(shm_test2 shm_dynamic/shm_test2.cc
# )
# target_link_libraries(shm_test2
#   geotool
#   ${OpenCV_LIBRARIES}
#   ${PROJECT_NAME}
#   # gtest
#   map_loader
# )
# add_executable(shm_test3 shm_dynamic/shm_test3.cc
# )
# target_link_libraries(shm_test3
#   geotool
#   ${OpenCV_LIBRARIES}
#   ${PROJECT_NAME}
#   # gtest
#   map_loader
# )
# add_dependencies(${PROJECT_NAME}
#   ${${PROJECT_NAME}_EXPORTED_TARGETS}
#   ${catkin_EXPORTED_TARGETS}
# )

