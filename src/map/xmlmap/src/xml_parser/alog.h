/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: vectormap
* FileName: map/vectormap/src/vectormap/alog.h
*
* Description: AVOS use roslog system, apollo use glog, so ...... creat this
file

*
* History:
* lbh         2018/05/11    1.0.0    build this module.
******************************************************************************/

#ifndef AVOS_CHECK_H_
#define AVOS_CHECK_H_

#include "ros/ros.h"

#define AVOS_RETURN_IF_ERROR(con, str)                                              \
  if (con) {                                                                   \
    ROS_ERROR_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                              << #str);                                        \
    return -1;                                                                 \
  }

#define AVOS_RETURN_IF_NULL(ptr)                                              \
  if (ptr == nullptr) {                                                       \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #ptr << " is nullptr,"                        \
                             << "ros::shutdown()");                           \
    return -1;                                                          \
  }

#define AVOS_RETURN_VAL_IF_NULL(ptr, val)                                     \
  if (ptr == nullptr) {                                                       \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #ptr << " is nullptr.");                      \
    return val;                                                               \
  }

#define AVOS_RETURN_IF(condition)                                             \
  if (condition) {                                                            \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #condition << " is not met,"                  \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }

#define AVOS_RETURN_VAL_IF(condition, val)                                    \
  if (condition) {                                                            \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #condition << " is not met.");                \
    return val;                                                               \
  }

#define AVOS_CHECK(val)                                                       \
  if (!(val)) {                                                               \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val << " is false,"                          \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_GT(val1, val2)                                             \
  if ((val1) <= (val2)) {                                                     \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " <= " << #val2                      \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_GE(val1, val2)                                             \
  if ((val1) < (val2)) {                                                      \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " < " << #val2                       \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_LT(val1, val2)                                             \
  if ((val1) >= (val2)) {                                                     \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " >= " << #val2                      \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_LE(val1, val2)                                             \
  if ((val1) > (val2)) {                                                      \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " > " << #val2                       \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_NE(val1, val2)                                             \
  if ((val1) == (val2)) {                                                     \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " == " << #val2                      \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#define AVOS_CHECK_EQ(val1, val2)                                             \
  if ((val1) != (val2)) {                                                     \
    ROS_WARN_STREAM(__FILE__ << "/" << __FUNCTION__ << ":" << __LINE__ << ":" \
                             << #val1 << " != " << #val2                      \
                             << " ros::shutdown()");                          \
    return -1;                                                          \
  }
#endif
