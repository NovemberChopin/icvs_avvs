cmake_minimum_required(VERSION 2.8.3)
project(vectormap)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/common/cmake")
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
find_package(catkin REQUIRED COMPONENTS
    message_generation
    genmsg
    roscpp
    rospy
    std_msgs
    roslib
    )
# find_package(GFLAGS REQUIRED)

find_package(Protobuf REQUIRED)
if(PROTOBUF_FOUND)
    message(STATUS "protobuf library found")
else()
    message(FATAL_ERROR "protobuf library is needed but cant be found")
endif()

# set(ADDAPTER_FILES ${CMAKE_SOURCE_DIR}/common/adapters/avos_app.cpp)
set(utilcpp
    ${CMAKE_SOURCE_DIR}/common/util/file.cpp
    ${CMAKE_SOURCE_DIR}/common/util/string_tokenizer.cpp
    ${CMAKE_SOURCE_DIR}/common/util/util.cpp
    )
set(mathcpp
    ${CMAKE_SOURCE_DIR}/common/math/aabox2d.cpp
    ${CMAKE_SOURCE_DIR}/common/math/angle.cpp
    ${CMAKE_SOURCE_DIR}/common/math/box2d.cpp
    ${CMAKE_SOURCE_DIR}/common/math/integral.cpp
    ${CMAKE_SOURCE_DIR}/common/math/istatistic.cpp
    ${CMAKE_SOURCE_DIR}/common/math/linear_interpolation.cpp
    ${CMAKE_SOURCE_DIR}/common/math/linear_quadratic_regulator.cpp
    ${CMAKE_SOURCE_DIR}/common/math/line_segment2d.cpp
    ${CMAKE_SOURCE_DIR}/common/math/math_utils.cpp
    ${CMAKE_SOURCE_DIR}/common/math/polygon2d.cpp
    ${CMAKE_SOURCE_DIR}/common/math/search.cpp
    ${CMAKE_SOURCE_DIR}/common/math/sin_table.cpp
    ${CMAKE_SOURCE_DIR}/common/math/vec2d.cpp
    )
catkin_package(
    CATKIN_DEPENDS roscpp
    DEPENDS roscpp genmsg gencpp genpy std_msgs 
    LIBRARIES vectormap geotool
    )

include(${CMAKE_SOURCE_DIR}/common/cmake/env_board.cmake)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    /usr/local/include/
    /usr/include/eigen3
    ${PROTOBUF_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/common/common_header
    ${CMAKE_SOURCE_DIR}/../devel/include/
    ${CMAKE_SOURCE_DIR}/../devel/include/proto
    )
link_directories(
    common/common_lib/glog/lib
    ${CMAKE_SOURCE_DIR}/common/common_lib
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/../devel/lib/
    )
aux_source_directory(${PROJECT_SOURCE_DIR}/src/hdmap/adapter/ adaptercpp)
aux_source_directory(${PROJECT_SOURCE_DIR}/src/hdmap/adapter/xml_parser/ xmlparsercpp)
# aux_source_directory(${CMAKE_SOURCE_DIR}/common/util utilcpp)
# aux_source_directory(${CMAKE_SOURCE_DIR}/common/math mathcpp)

add_library(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/hdmap/hdmap.cc
    ${PROJECT_SOURCE_DIR}/src/vectormap/vectormap.cc
    ${PROJECT_SOURCE_DIR}/src/vectormap/roadnet_impl.cc
    ${PROJECT_SOURCE_DIR}/src/vectormap/roadnet_xml_parser.cc
    ${PROJECT_SOURCE_DIR}/src/hdmap/hdmap_common.cc
    ${PROJECT_SOURCE_DIR}/src/hdmap/hdmap_impl.cc
    ${adaptercpp}
    ${xmlparsercpp}
    ${utilcpp}
    ${mathcpp}
    )

target_link_libraries(${PROJECT_NAME}
    tinyxml2
    ${catkin_LIBRARIES}
    gflags
    # adapter_manager
    glog
    ${PCL_LIBRARIES}
    proj boost_filesystem boost_system
    proto_proto
    vectormap_proto_proto
    ${PROTOBUF_LIBRARIES}
    )
add_dependencies(${PROJECT_NAME}
    ${${PROJECT_NAME}_EXPORTED_TARGETS}
    ${catkin_EXPORTED_TARGETS}
    )
add_library(geotool
    ${PROJECT_SOURCE_DIR}/src/geotool/geotool.cc
    )
target_link_libraries(geotool
    proj
    )

