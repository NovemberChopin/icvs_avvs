/* /******************************************************************************
 * Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
 *
 * NodeName: map_loader
 * FileName: util.h
 *
 * Description: atom of map_loader node
 *
 *
 * History:
 * lbh         2018/04/22    1.0.0    build this module.
 ******************************************************************************/
#ifndef MAP_COMMON_UTIL_H_
#define MAP_COMMON_UTIL_H_
#pragma once
#include <proj_api.h>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <ros/ros.h>
namespace avos {
namespace map {
namespace common {
template <class T>
std::string ToString(T t) {
  std::stringstream name;
  name << t;
  return name.str();
}
std::string InToString();
template <class T, class... Args>
std::string intoString(T t, Args... args) {
  return ToString<T>(t) + IntoString(args...);
}

template <class T>
int VectorSubtract(std::vector<T> &abc, std::vector<T> &bcd, std::vector<T> &a,
                   std::vector<T> &bc) {
  a.clear();
  bc.clear();
  if (abc.size() == 0) return -1;
  for (auto ai : abc) {
    if (std::count(bcd.begin(), bcd.end(), ai) == 0)
      a.push_back(ai);
    else
      bc.push_back(ai);
  }
  return 0;
}

template <class T>
std::vector<T> VectorUnion(std::vector<T> &abc, std::vector<T> &bcd) {
  std::vector<T> bc;
  bc.clear();
  if (abc.size() == 0) return bc;
  for (auto bc_ : abc) {
    if (std::count(bcd.begin(), bcd.end(), bc_) != 0) bc.push_back(bc_);
  }
  return bc;
}

template <class T>
int VectorSubtract2(std::vector<T> &abc, std::vector<T> &bcd, std::vector<T> &a,
                    std::vector<T> &bc, std::vector<T> &d) {
  a.clear();
  bc.clear();
  d.clear();
  for (auto ai : abc) {
    if (std::count(bcd.begin(), bcd.end(), ai) == 0)
      a.push_back(ai);
    else
      bc.push_back(ai);
  }
  for (auto di : bcd) {
    if (std::count(bc.begin(), bc.end(), di) == 0) d.push_back(di);
  }
  return 0;
}
bool FindAllFiles(std::string directory_path, std::string suffix,
                  std::vector<std::string> &names);
std::vector<std::string> FindAllFiles(std::string directory_path,
                                      std::string suffix);
std::vector<std::string> ComposePCDNames(int size, int row, int col, int level,
                                         std::string suffix = ".pcd",
                                         std::string prefix = "Tile",
                                         std::string joint = "_");

inline int GetLongZone(double longitude) {
  double longZone = 0.0;
  if (longitude < 0.0) {
    longZone = ((180.0 + longitude) / 6.0) + 1;
  } else {
    longZone = (longitude / 6.0) + 31;
  }
  return static_cast<int>(longZone);
}
void Lonlat2UTM(double &lon, double &lat);
void UTM2Lonlat(double &x, double &y, int zone);
}  // namespace common
}  // namespace map
}  // namespace AVOS
#endif