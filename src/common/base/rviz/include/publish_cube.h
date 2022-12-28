#ifndef __PUBLISH_CUBE_H__
#define __PUBLISH_CUBE_H__

#include <ros/ros.h>

#include <iostream>
#include <string>

#include "publish_base_header.h"
#include "visualization_msgs/MarkerArray.h"
// For heading cube:
#include <tf2/LinearMath/Quaternion.h>

#include "macros.h"
DEFINE_has_member(heading);

template <typename T>
struct PublishCube {
  visualization_msgs::MarkerArray publish_marker_array;
  T inner_cubes;
  bool has_heading_;
  const float degree_to_arc_ = 3.14159 / 180.0;
  pColorFunction pget_g_func;
  pColorFunction pget_r_func;
  pColorFunction pget_b_func;
  float DefaultGetGFunc(void *) { return 1.0; }

  float DefaultGetRFunc(void *) { return 1.0; }

  float DefaultGetBFunc(void *) { return 0.0; }
  PublishCube() {
    publish_marker_array.markers.clear();
    pget_g_func =
        std::bind(&PublishCube::DefaultGetGFunc, this, std::placeholders::_1);
    pget_r_func =
        std::bind(&PublishCube::DefaultGetRFunc, this, std::placeholders::_1);
    pget_b_func =
        std::bind(&PublishCube::DefaultGetBFunc, this, std::placeholders::_1);
    has_heading_ = temp_has_member(typename T::value_type, heading);
  }
  void Reset() { publish_marker_array.markers.clear(); }
  void SetGFunc(pColorFunction usr_get_g_func) {
    pget_g_func = std::move(usr_get_g_func);
  }
  void SetRFunc(pColorFunction usr_get_r_func) {
    pget_r_func = std::move(usr_get_r_func);
  }
  void SetBFunc(pColorFunction usr_get_b_func) {
    pget_b_func = std::move(usr_get_b_func);
  }

  void publish(ros::Publisher &pub, const char &color = 'y',
               const std::string &_namespace = "base") {
    if (pub.getNumSubscribers() == 0) return;
    if (publish_marker_array.markers.size() > 400) {
      publish_marker_array.markers.clear();
    }

    if (inner_cubes.size() > publish_marker_array.markers.size()) {
      publish_marker_array.markers.resize(inner_cubes.size());
    }
    int counter = 0;
    for (auto &cube : inner_cubes) {
      publish_marker_array.markers[counter].header.frame_id = "/base_link";
      publish_marker_array.markers[counter].header.stamp = ros::Time::now();
      publish_marker_array.markers[counter].ns = _namespace;
      publish_marker_array.markers[counter].id = counter;
      publish_marker_array.markers[counter].lifetime = ros::Duration(0.1);
      publish_marker_array.markers[counter].action =
          visualization_msgs::Marker::ADD;
      publish_marker_array.markers[counter].scale.x = cube.length;
      publish_marker_array.markers[counter].scale.y = cube.width;
      publish_marker_array.markers[counter].scale.z = cube.height;

      publish_marker_array.markers[counter].pose.position.x = cube.x;
      publish_marker_array.markers[counter].pose.position.y = cube.y;
      publish_marker_array.markers[counter].pose.position.z = 0;  // cube.z;
      if (has_heading_) {
        tf2::Quaternion myQuaternion;
        myQuaternion.setRPY(0, 0, cube.heading * degree_to_arc_);  //
        publish_marker_array.markers[counter].pose.orientation.x =
            myQuaternion[0];
        publish_marker_array.markers[counter].pose.orientation.y =
            myQuaternion[1];
        publish_marker_array.markers[counter].pose.orientation.z =
            myQuaternion[2];
        publish_marker_array.markers[counter].pose.orientation.w =
            myQuaternion[3];
      }
      publish_marker_array.markers[counter].type =
          visualization_msgs::Marker::CUBE;
      switch (color) {
        case 'y':
          publish_marker_array.markers[counter].color.b = 0.0;
          publish_marker_array.markers[counter].color.g = 1.0;
          publish_marker_array.markers[counter].color.r = 1.0;
          break;
        case 'r':
          publish_marker_array.markers[counter].color.b = 0.0;
          publish_marker_array.markers[counter].color.g = 0.0;
          publish_marker_array.markers[counter].color.r = 1.0;
          break;
        case 'b':
          publish_marker_array.markers[counter].color.b = 1.0;
          publish_marker_array.markers[counter].color.g = 0.0;
          publish_marker_array.markers[counter].color.r = 0.0;
          break;
        case 'g':
          publish_marker_array.markers[counter].color.b = 0.0;
          publish_marker_array.markers[counter].color.g = 1.0;
          publish_marker_array.markers[counter].color.r = 0.0;
          break;
        case 'p':
          publish_marker_array.markers[counter].color.b = 0.5;
          publish_marker_array.markers[counter].color.g = 0.0;
          publish_marker_array.markers[counter].color.r = 0.5;
          break;
        case 'w':
          publish_marker_array.markers[counter].color.b = 1.0;
          publish_marker_array.markers[counter].color.g = 1.0;
          publish_marker_array.markers[counter].color.r = 1.0;
          break;
        default:
          publish_marker_array.markers[counter].color.b =
              pget_b_func((void *)&cube);
          publish_marker_array.markers[counter].color.g =
              pget_g_func((void *)&cube);
          publish_marker_array.markers[counter].color.r =
              pget_r_func((void *)&cube);
          // publish_marker_array.markers[counter].color.b = 0.0;
          // publish_marker_array.markers[counter].color.g = 1.0;
          // publish_marker_array.markers[counter].color.r = 1.0;
      }

      publish_marker_array.markers[counter].color.a = 0.7;
      counter++;
    }

    for (int i = counter; i < publish_marker_array.markers.size(); i++) {
      publish_marker_array.markers[i].id = i;
      publish_marker_array.markers[i].action =
          visualization_msgs::Marker::DELETE;
      publish_marker_array.markers[i].pose.position.x = -9900;
    }
    pub.publish(publish_marker_array);
  }
};

#endif
