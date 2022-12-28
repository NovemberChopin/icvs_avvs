/*
 * Copyright (c) 2011, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */


#include <ros/ros.h>

#include "common/rviz/include/base_interactive_marker.h"
#include "common/rviz/include/publish_pointcloud.h"




using namespace visualization_msgs;

double gcenter_x = 0.0, gcenter_y = 0.0, gwidth = 3.0, glength = 3.0;
using Point32Vec = std::vector<geometry_msgs::Point32>;

void MakePointCloud(ObjectPointCloud<Point32Vec>  &object_pointcloud ) {
  ros::NodeHandle nh;
  nh.param("width", gwidth, gwidth);
  nh.param("length", glength, glength);

  object_pointcloud.Reset();

  // ROS_INFO_STREAM(  "-------init1 " << object_pointcloud.inner_points.size() );
  float dl_w = gcenter_x - gwidth / 2;
  float dr_h = gcenter_y - glength / 2;
  float ul_w = gcenter_x + gwidth / 2;
  float ur_h = gcenter_y + glength / 2;
  float d = 0.0;

  // ROS_INFO_STREAM(  "init1 " << dl_w << "," << dr_h << "," << ul_w << "," << ur_h);
  for (d = dl_w; d < ul_w; d += 0.1) {
    geometry_msgs::Point32 tmp;
    tmp.x = d;
    tmp.y = dr_h;
    object_pointcloud.inner_points.push_back(tmp);
  }

  // ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );


  for (d = dl_w; d < ul_w; d += 0.1) {
    geometry_msgs::Point32 tmp;
    tmp.x = d;
    tmp.y = ur_h;
    object_pointcloud.inner_points.push_back(tmp);
  }
  // ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

  for (d = dr_h; d < ur_h; d += 0.1) {
    geometry_msgs::Point32 tmp;
    tmp.x = dl_w;
    tmp.y = d;
    object_pointcloud.inner_points.push_back(tmp);
  }
  // ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

  for (d = dr_h; d < ur_h; d += 0.1) {
    geometry_msgs::Point32 tmp;
    tmp.x = ul_w;
    tmp.y = d;
    object_pointcloud.inner_points.push_back(tmp);
  }
  // ROS_INFO_STREAM(  "init1 " << object_pointcloud.inner_points.size() );

}


// %Tag(main)%
int main(int argc, char** argv)
{
  ros::init(argc, argv, "basic_controls");
  ros::NodeHandle n;



  BaseInteractiveMarker base_interactive_marker;


  ros::NodeHandle nh;
  ros::Publisher pub_pointcloud_ = nh.advertise<PathplanPointCloud> ( "obs_cell", 1, true );

  ros::Rate rate(10);
  ObjectPointCloud<Point32Vec> object_pointcloud;
  // for (int i = 0; i < 100; i++) {
  //   geometry_msgs::Point32 temp;
  //   temp.x = 0;
  //   temp.y = i;
  //   object_pointcloud.inner_points.push_back(temp);
  // }
  // ROS_INFO_STREAM(  "init" );
  while (ros::ok()) {
    MakePointCloud(object_pointcloud);
    object_pointcloud.publish(pub_pointcloud_);
    ros::spinOnce();
    rate.sleep();
  }

  // ros::spin();

}
// %EndTag(main)%
