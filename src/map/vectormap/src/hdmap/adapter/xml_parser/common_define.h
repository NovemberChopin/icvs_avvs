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
#ifndef MODULES_MAP_HDMAP_ADAPTER_XML_PARSER_COMMON_DEFINE_H_
#define MODULES_MAP_HDMAP_ADAPTER_XML_PARSER_COMMON_DEFINE_H_

#include <string>
#include <unordered_set>
#include <vector>
#include <ros/ros.h>
#include "map/vectormap/src/vectormap/alog.h"
#include "map.pb.h"

namespace avos {
namespace hdmap {
namespace adapter {

using PbHeader = avos::hdmap::Header;
using PbRoad = avos::hdmap::Road;
using PbRoadSection = avos::hdmap::RoadSection;
using PbLane = avos::hdmap::Lane;
using PbJunction = avos::hdmap::Junction;
using PbSignal = avos::hdmap::Signal;
using PbSubSignal = avos::hdmap::Subsignal;
using PbCrosswalk = avos::hdmap::Crosswalk;
using PbSpeedBump = avos::hdmap::SpeedBump;
using PbStopSign = avos::hdmap::StopSign;
using PbYieldSign = avos::hdmap::YieldSign;
using PbObjectOverlapInfo = avos::hdmap::ObjectOverlapInfo;
using PbOverlap = avos::hdmap::Overlap;
using PbClearArea = avos::hdmap::ClearArea;
using PbLineSegment = avos::hdmap::LineSegment;
using PbCurveSegment = avos::hdmap::CurveSegment;
using PbCurve = avos::hdmap::Curve;
using PbPoint3D = avos::common::PointENU;
using PbLaneType = avos::hdmap::Lane_LaneType;
using PbTurnType = avos::hdmap::Lane_LaneTurn;
using PbID = avos::hdmap::Id;
using PbLaneBoundary = avos::hdmap::LaneBoundary;
using PbLaneBoundaryTypeType = avos::hdmap::LaneBoundaryType_Type;
using PbPolygon = avos::hdmap::Polygon;
using PbBoundaryPolygon = avos::hdmap::BoundaryPolygon;
using PbBoundaryEdge = avos::hdmap::BoundaryEdge;

using PbLaneDirection = avos::hdmap::Lane_LaneDirection;
using PbSignalType = avos::hdmap::Signal_Type;
using PbSubSignalType = avos::hdmap::Subsignal_Type;
using PbBoundaryEdgeType = avos::hdmap::BoundaryEdge_Type;

struct StopLineInternal {
  std::string id;
  PbCurve curve;
};

struct StopSignInternal {
  std::string id;
  PbStopSign stop_sign;
  std::unordered_set<std::string> stop_line_ids;
};

struct YieldSignInternal {
  std::string id;
  PbYieldSign yield_sign;
  std::unordered_set<std::string> stop_line_ids;
};

struct TrafficLightInternal {
  std::string id;
  PbSignal traffic_light;
  std::unordered_set<std::string> stop_line_ids;
};

struct OverlapWithLane {
  std::string object_id;
  double start_s;
  double end_s;
  bool is_merge;

  OverlapWithLane() : is_merge(false) {}
};

struct OverlapWithJunction {
  std::string object_id;
};

struct LaneInternal {
  PbLane lane;
  std::vector<OverlapWithLane> overlap_signals;
  std::vector<OverlapWithLane> overlap_objects;
  std::vector<OverlapWithLane> overlap_junctions;
  std::vector<OverlapWithLane> overlap_lanes;
};

struct JunctionInternal {
  PbJunction junction;
  std::unordered_set<std::string> road_ids;
  std::vector<OverlapWithJunction> overlap_with_junctions;
};

struct RoadSectionInternal {
  std::string id;
  PbRoadSection section;
  std::vector<LaneInternal> lanes;
};

struct RoadInternal {
  std::string id;
  PbRoad road;

  bool in_junction;
  std::string junction_id;

  std::vector<RoadSectionInternal> sections;

  std::vector<TrafficLightInternal> traffic_lights;
  std::vector<StopSignInternal> stop_signs;
  std::vector<YieldSignInternal> yield_signs;
  std::vector<PbCrosswalk> crosswalks;
  std::vector<PbClearArea> clear_areas;
  std::vector<PbSpeedBump> speed_bumps;
  std::vector<StopLineInternal> stop_lines;

  RoadInternal() : in_junction(false) { junction_id = ""; }
};

}  // namespace adapter
}  // namespace hdmap
}  // namespace avos

#endif  // MODULES_MAP_HDMAP_ADAPTER_XML_PARSER_COMMON_DEFINE_H_
