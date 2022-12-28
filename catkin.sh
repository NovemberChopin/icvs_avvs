#!/bin/bash

export ROS_LANG_DISABLE=genjava
catkin_make -DCMAKE_BUILD_TYPE=Release --pkg common_msgs
catkin_make -DCMAKE_BUILD_TYPE=Release --pkg perception_msgs
catkin_make -DCMAKE_BUILD_TYPE=Release --pkg ivlocmsg
catkin_make -DCMAKE_BUILD_TYPE=Release --pkg mainstream_msgs
catkin_make -DCMAKE_BUILD_TYPE=Release
