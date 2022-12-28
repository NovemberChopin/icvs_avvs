cmake_minimum_required(VERSION 2.8.3)
project(wxbmap)

add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS roscpp
  roslib
)
catkin_package(
  CATKIN_DEPENDS roscpp roslib
)

include_directories(
	${PROJECT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/../lib
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../devel/include/
    ${CMAKE_SOURCE_DIR}/common/common_header
    ${CMAKE_SOURCE_DIR}
    )


include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)

link_directories(
    common/common_lib/glog/lib
    ${CMAKE_SOURCE_DIR}/common/common_lib
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../devel/lib
    )
aux_source_directory(${PROJECT_SOURCE_DIR}/src/xml_parser/ xmlparsercpp)

add_library(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/read_xml.cc
    ${PROJECT_SOURCE_DIR}/src/freespace_check.cc
    ${PROJECT_SOURCE_DIR}/../common/coordination_transform/coordination_transform.cc
    )
target_link_libraries(${PROJECT_NAME}    
    tinyxml2
    ${catkin_LIBRARIES}
    ${geotool_LIBRARIES}
    proj geotool
    map_loader
    )
