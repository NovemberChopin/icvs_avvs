/* Copyright 2017 The Apollo Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
=========================================================================*/
#include "map/vectormap/src/hdmap/adapter/opendrive_adapter.h"

#include <vector>

#include <ros/ros.h>
#include "map/vectormap/src/hdmap/adapter/proto_organizer.h"
#include "map/vectormap/src/hdmap/adapter/xml_parser/status.h"
#include "map/vectormap/src/vectormap/alog.h"

namespace avos {
namespace hdmap {
namespace adapter {

bool OpendriveAdapter::LoadData(const std::string &filename,
                                avos::hdmap::Map *pb_map,
                                avos::hdmap::Roadnet *pb_roadnet) {
  ROS_INFO_STREAM("BEGIN TO LOAD MAP DATA ... ...");
  if (!LoadData(filename, pb_map)) return false;
  ROS_INFO_STREAM("BEGIN TO LOAD ROADNET DATA ... ...");
  if (!LoadData(filename, pb_roadnet)) return false;
  return true;
}
bool OpendriveAdapter::LoadData(const std::string &filename,
                                avos::hdmap::Map *pb_map) {
  AVOS_RETURN_IF_NULL(pb_map);

  tinyxml2::XMLDocument document;
  if (document.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    ROS_ERROR_STREAM("fail to load file " << filename);
    return false;
  }

  // root node
  const tinyxml2::XMLElement *root_node = document.RootElement();
  AVOS_CHECK(root_node != nullptr);
  // header
  PbHeader *map_header = pb_map->mutable_header();
  Status status = HeaderXmlParser::Parse(*root_node, map_header);
  if (!status.ok()) {
    ROS_ERROR_STREAM("fail to parse opendrive header, "
                     << status.error_message());
    return false;
  }

  // roads
  std::vector<RoadInternal> roads;
  status = RoadsXmlParser::Parse(*root_node, &roads);
  if (!status.ok()) {
    ROS_ERROR_STREAM("fail to parse opendrive road, "
                     << status.error_message());
    return false;
  }

  // junction
  std::vector<JunctionInternal> junctions;
  status = JunctionsXmlParser::Parse(*root_node, &junctions);
  if (!status.ok()) {
    ROS_ERROR_STREAM("fail to parse opendrive junction, "
                     << status.error_message());
    return false;
  }

  ProtoOrganizer proto_organizer;
  proto_organizer.GetRoadElements(&roads);
  proto_organizer.GetJunctionElements(junctions);
  proto_organizer.GetOverlapElements(roads, junctions);
  proto_organizer.OutputData(pb_map);
  return true;
  /*
  if (pb_roadnet == nullptr) return false;
  ROS_INFO_STREAM("BEGIN TO LOAD ROADNET DATA ... ...");
  RoadnetXmlParser roadnet_par;
  int ret = roadnet_par.GetRoadnetProto(*root_node, pb_roadnet);
  if (ret == -1) {
    ROS_WARN_STREAM("failed to get roadnet proto");
    return false;
  }
  return true;
  */
}

bool OpendriveAdapter::LoadData(const std::string &filename,
                                avos::hdmap::Roadnet *pb_roadnet) {
  AVOS_RETURN_IF_NULL(pb_roadnet);

  ROS_INFO_STREAM (" OpendriveAdapter::LoadData filename " << filename);
  tinyxml2::XMLDocument document;
  if (document.LoadFile(filename.c_str()) != tinyxml2::XML_SUCCESS) {
    ROS_ERROR_STREAM("fail to load file " << filename);
    return false;
  }

  // root node
  const tinyxml2::XMLElement *root_node = document.RootElement();
  AVOS_CHECK(root_node != nullptr);

  if (pb_roadnet == nullptr) return false;

  RoadnetXmlParser roadnet_par;
  int ret = roadnet_par.GetRoadnetProto(*root_node, pb_roadnet);
  if (ret == -1) {
    ROS_WARN_STREAM("failed to get roadnet proto");
    return false;
  }
  return true;
}

}  // namespace adapter
}  // namespace hdmap
}  // namespace avos
