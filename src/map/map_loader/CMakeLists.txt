cmake_minimum_required(VERSION 2.8.3)
project(map_loader)

add_compile_options(-std=c++11)

set(PCL_DIR /usr/lib/aarch64-linux-gnu/cmake/pcl)

find_package(catkin REQUIRED COMPONENTS roscpp
  roslib
  basemap
)


catkin_package(
  CATKIN_DEPENDS roscpp roslib
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME}
)

# PCL

find_package(OpenCV)

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)

include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS}) 
add_definitions(${PCL_DEFINITIONS})



include_directories(
	${PROJECT_SOURCE_DIR}/../include
    ${PROJECT_SOURCE_DIR}/../lib
    ${PROJECT_SOURCE_DIR}/..
    ${PROJECT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/../devel/include/
    ${CMAKE_SOURCE_DIR}/common/common_header
    ${CMAKE_SOURCE_DIR}
    ${OpenCV_INCLUDE_DIRS}
    )

link_directories(
    common/common_lib/glog/lib
    ${CMAKE_SOURCE_DIR}/common/common_lib
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../devel/lib
    ${PCL_LIBRARY_DIRS}
    )

add_library(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/map_loader.cc
    ${PROJECT_SOURCE_DIR}/src/map_info.cc
    ${PROJECT_SOURCE_DIR}/src/link_map.cpp
    ${PROJECT_SOURCE_DIR}/src/app_map.cpp
    ${PROJECT_SOURCE_DIR}/src/map_info.cc
  	${PROJECT_SOURCE_DIR}/src/show_vectormap.cpp
    )

target_link_libraries(${PROJECT_NAME}
    tinyxml2
    ${catkin_LIBRARIES}
    ${geotool_LIBRARIES}
    proj geotool
    vectormap
    xmlmap
    proto_proto
    vectormap_proto_proto
    ${PROTOBUF_LIBRARIES}
    ${OpenCV_LIBRARIES}
    )

add_executable(${PROJECT_NAME}_test
    ${PROJECT_SOURCE_DIR}/test/main.cc
    )

target_link_libraries(${PROJECT_NAME}_test 
    ${PROJECT_NAME}
    )

add_executable( proto_map_generator
    ${PROJECT_SOURCE_DIR}/tools/proto_map_generator.cc
    ${CMAKE_SOURCE_DIR}/common/util/util.cpp
    )
target_link_libraries(proto_map_generator
    ${PROJECT_NAME}
    ${PROTOBUF_LIBRARIES}
    )


add_executable(png2bin tools/png2bin.cc
)
target_link_libraries(png2bin
${OpenCV_LIBRARIES}
${PROJECT_NAME}
)

add_executable(creat_shared_memory tools/creat_shared_memory.cc
)
target_link_libraries(creat_shared_memory
  ${PROJECT_NAME}
)

add_executable(free_shared_memory tools/free_shared_memory.cc
)
target_link_libraries(free_shared_memory
  ${PROJECT_NAME}
)

# add_executable( geotool_test
#     ${PROJECT_SOURCE_DIR}/test/geotool_test.cc
#     )
# target_link_libraries(geotool_test
#     geotool gtest
#     pthread
#     )
