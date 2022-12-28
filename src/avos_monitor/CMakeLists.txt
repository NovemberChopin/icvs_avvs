##############################################################################
# CMake
##############################################################################

cmake_minimum_required(VERSION 2.8.3)
project(avos_monitor)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#添加c++11的支持
set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")
##############################################################################
# Catkin
##############################################################################

# qt_build provides the qt cmake glue, roscpp the comms for a default talker
find_package(catkin REQUIRED COMPONENTS rviz roscpp sensor_msgs
    cv_bridge
    std_msgs
    image_transport
    common_base
    pcl_ros
 	pcl_conversions
    )
include_directories(${PCL_INCLUDE_DIRS})
link_directories(${PCL_LIBRARY_DIRS}) 
add_definitions(${PCL_DEFINITIONS})

find_package(OpenCV REQUIRED)

find_package(Qt5Core)
find_package(Qt5Test)
get_target_property(QtCore_location Qt5::Core LOCATION )

find_package(Qt5 REQUIRED Core Widgets Concurrent Multimedia Xml)
set(QT_LIBRARIES Qt5::Widgets Qt5::Multimedia)

include_directories(
	include
    ${catkin_INCLUDE_DIRS}
    ${CMAKE_SOURCE_DIR}/
    ${CMAKE_SOURCE_DIR}/avos_monitor/Hugo/
    ${CMAKE_SOURCE_DIR}/avos_monitor/vehicle_info/
    ${CMAKE_SOURCE_DIR}/common/
    ${CMAKE_SOURCE_DIR}/common/common_header/
    ${CMAKE_SOURCE_DIR}/common/base/communication/include/
    ${CMAKE_SOURCE_DIR}/common/base/
    ${CMAKE_SOURCE_DIR}/../devel/include/
	${PCL_INCLUDE_DIRS}
    )

link_directories(
  ${CMAKE_SOURCE_DIR}/../devel/lib/
  ${CMAKE_SOURCE_DIR}/common/common_lib
  common/common_lib/glog/lib
)
# Use this to define what the package will export (e.g. libs, headers).
# Since the default here is to produce only a binary, we don't worry about
# exporting anything. 
catkin_package()

##############################################################################
# Qt Environment
##############################################################################

# this comes from qt_build's qt-ros.cmake which is automatically 
# included via the dependency call in package.xml
#rosbuild_prepare_qt4(QtCore QtGui) # Add the appropriate components to the component list here

##############################################################################
# Sections
##############################################################################

file(GLOB QT_FORMS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB QT_RESOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resources/*.qrc vehicle_info/resource.qrc)
file(GLOB_RECURSE QT_MOC RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h Hugo/*.hpp *.h vehicle_info/*.h)

QT5_ADD_RESOURCES(QT_RESOURCES_CPP ${QT_RESOURCES})
QT5_WRAP_UI(QT_FORMS_HPP ${QT_FORMS})
QT5_WRAP_CPP(QT_MOC_HPP ${QT_MOC})

##############################################################################
# Sources
##############################################################################

file(GLOB_RECURSE QT_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp Hugo/*.cpp vehicle_info/*.cpp)

##############################################################################
# Binaries
##############################################################################


add_executable(${PROJECT_NAME} ${QT_SOURCES} ${QT_RESOURCES_CPP} ${QT_FORMS_HPP} ${QT_MOC_HPP})

target_link_libraries(${PROJECT_NAME} 
	${QT_LIBRARIES} 
	${catkin_LIBRARIES} 
	${Qt5Widgets_LIBRARIES} 
	${Qt5Core_LIBRARIES} 
	Qt5::Xml 
	Qt5::Concurrent
	Qt5::Test
)

target_link_libraries(${PROJECT_NAME}
	${common_base_LIBRARIES}
	${OpenCV_LIBRARIES} 
	glog
	common_base_time
	tinyxml2
	geotool
	map_loader
	yaml-cpp
)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})

