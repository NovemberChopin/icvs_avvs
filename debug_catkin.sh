#!/bin/bash
if [ $# -ge 1 ]; then
	tn="$1"
else
	tn="-j8"
fi 
echo "编译使用参数：$tn"

val=$1
build_type=${val:-Debug}
export ROS_LANG_DISABLE=genjava
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg common_msgs
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg perception_msgs
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg ivlocmsg
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg planning_msgs
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg control_msgs
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg business_platform
catkin_make -DCMAKE_BUILD_TYPE=$build_type --pkg canbus_msgs
catkin_make -DCMAKE_BUILD_TYPE=$build_type
