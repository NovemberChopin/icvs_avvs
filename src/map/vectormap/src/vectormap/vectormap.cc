/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: vectormap
* FileName: api
*
* Description: fool AVOS vectormap API like USB

*
* History:
* lbh         2018/05/15    1.0.0    build this module.
******************************************************************************/
#include "map/vectormap/include/vectormap.h"
#include <iostream>
#include "map/vectormap/src/vectormap/alog.h"
#include "common/util/util.h"

namespace avos {
namespace vectormap {

constexpr double kSampleDistance = 0.25;
constexpr double kRadToDeg = 57.29577950560105;
/*
std::shared_ptr<VectorMap> VectorMap::vectormap_ptr_(new VectorMap());
std::shared_ptr<VectorMap> VectorMap::Initance() { return vectormap_ptr_; }
*/
int VectorMap::LoadMapFromFile(const std::string &file) {
  return impl_.LoadMapFromFile(file);
}

int VectorMap::LoadMapFromProto(const Map &map_proto) {
  ROS_DEBUG_STREAM(
    "Loading VectorMap with header: " << map_proto.header().ShortDebugString());
  return impl_.LoadMapFromProto(map_proto);
}

int VectorMap::LoadRoadnetFromFile(const std::string &file) {
  return roadnet_impl_.LoadRoadnetFromFile(file);
}

int VectorMap::LoadRoadnetFromProto(const Roadnet &roadnet_proto) {
  ROS_DEBUG_STREAM("Loading Roadnet proto ... ");
  return roadnet_impl_.LoadRoadnetFromProto(roadnet_proto);
}

const RoadNodeTable *VectorMap::GetRoadNet() const {
  return roadnet_impl_.GetRoadNet();
}

LaneInfoConstPtr VectorMap::GetLaneById(const Id &id) const {
  return impl_.GetLaneById(id);
}

LaneInfoConstPtr VectorMap::GetLaneById(const std::string &id) const {
  Id lane_id;
  lane_id.set_id(id);
  return impl_.GetLaneById(lane_id);
}

RoadInfoConstPtr VectorMap::GetRoadById(const Id &id) const {
  return impl_.GetRoadById(id);
}

RoadNodeInfoConstPtr VectorMap::GetRoadNodeById(const Id &id) const {
  return roadnet_impl_.GetRoadNodeById(id);
}

SignalInfoConstPtr VectorMap::GetSignalById(const Id& id) const {
  return impl_.GetSignalById(id);
}

int VectorMap::GetRoads(const avos::common::PointENU &point, double distance,
                        std::vector<RoadInfoConstPtr> &roads) const {
  return impl_.GetRoads(point, distance, &roads);
}

int VectorMap::GetRoads(const avos::common::PointENU &point, double distance,
                        std::vector<Id> &ids) const {
  std::vector<RoadInfoConstPtr> roads;
  if (GetRoads(point, distance, roads) != 0) return -1;
  for (auto &road : roads) {
    ids.push_back(road->id());
  }
  return 0;
}

int VectorMap::GetNearestRoad(const common::PointENU &point,
                              Id &nearest_road_id) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1) {
    ROS_ERROR_STREAM("Can't find nearest lane!");
    return -1;
  }

  nearest_road_id = nearest_lane->road_id();
  return 0;
}


int VectorMap::GetNearestLane(const common::PointENU &point,
                              LaneInfoConstPtr &nearest_lane, double &nearest_s,
                              double &nearest_l) const {
  return impl_.GetNearestLane(point, &nearest_lane, &nearest_s, &nearest_l);
}

int VectorMap::GetNearestLaneWithHeading(const common::PointENU& point, const double distance, 
                                         const double central_heading,
                                         const double max_heading_difference, 
                                         LaneInfoConstPtr& nearest_lane,
                                         double& nearest_s, double& nearest_l) const {
  return impl_.GetNearestLaneWithHeading(point, distance, central_heading,
                                         max_heading_difference, &nearest_lane, 
                                         &nearest_s, &nearest_l);
}

int VectorMap::GetRoadLanes(const Id &id, roadLanes &lanes) const {
  RoadInfoConstPtr road = GetRoadById(id);
  if (road == nullptr) return -1;
  Id key;
  std::vector<Id> value;
  for (auto &section : road->road().section()) {
    key = section.id();
    for (auto &id : section.lane_id()) {
      value.push_back(id);
    }
    lanes.push_back(std::make_pair(key, value));
  }
  return 0;
}

int VectorMap::GetRoadSuccessorIDs(const Id &id,
                                   std::vector<Id> &ids_succ) const {
  RoadNodeInfoConstPtr roadnode = GetRoadNodeById(id);
  if (roadnode == nullptr) return -1;
  ids_succ = roadnode->GetSuccessorIds();
  return 0;
}

int VectorMap::GetRoadPredecessorIDs(const Id &id,
                                     std::vector<Id> &ids_pred) const {
  RoadNodeInfoConstPtr roadnode = GetRoadNodeById(id);
  if (roadnode == nullptr) return -1;
  ids_pred = roadnode->GetPredecessorIds();
  return 0;
}

int VectorMap::GetRoad(const Id &lane_id, Id &road_id) const {
  LaneInfoConstPtr lane = GetLaneById(lane_id);
  if (lane == nullptr) return -1;
  road_id = lane->road_id();
  return 0;
}

int VectorMap::GetLanes(const avos::common::PointENU &point, double distance,
                        std::vector<LaneInfoConstPtr> &lanes) const {
  return impl_.GetLanes(point, distance, &lanes);
}

int VectorMap::GetSignals(const avos::common::PointENU &point, double distance,
                          std::vector<SignalInfoConstPtr> &my_signals) const {
  return impl_.GetSignals(point, distance, &my_signals);
}

int VectorMap::GetLaneCenterLine(const Id &id,
                                 std::vector<PointXYA> &points) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  points.clear();
  if (lane->points().size() != lane->headings().size()) return -1;
  int i = 0;
  for (auto &point : lane->points()) {
    points.emplace_back(point.x(), point.y(), lane->headings()[i] * kRadToDeg);
    i++;
  }
  return 0;
}

LaneBoundaryType::Type VectorMap::LeftBoundaryType(
  const LaneWaypoint &waypoint) const {
  if (!waypoint.lane) {
    return LaneBoundaryType::UNKNOWN;
  }
  for (const auto &type :
       waypoint.lane->lane().left_boundary().boundary_type()) {
    if (type.s() <= waypoint.s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneBoundaryType::Type VectorMap::RightBoundaryType(
  const LaneWaypoint &waypoint) const {
  if (!waypoint.lane) {
    return LaneBoundaryType::UNKNOWN;
  }
  for (const auto &type :
       waypoint.lane->lane().right_boundary().boundary_type()) {
    if (type.s() <= waypoint.s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneBoundaryType::Type VectorMap::LeftBoundaryType(
  const common::PointENU &point) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return LaneBoundaryType::UNKNOWN;
  for (const auto &type :
       nearest_lane->lane().left_boundary().boundary_type()) {
    if (type.s() <= nearest_s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneBoundaryType::Type VectorMap::RightBoundaryType(
  const common::PointENU &point) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return LaneBoundaryType::UNKNOWN;
  for (const auto &type :
       nearest_lane->lane().right_boundary().boundary_type()) {
    if (type.s() <= nearest_s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneBoundaryType::Type VectorMap::LeftBoundaryType(
  const LaneInfoConstPtr lane) const {
  double nearest_s;
  double nearest_l;
  for (const auto &type :
       lane->lane().left_boundary().boundary_type()) {
    if (type.s() <= nearest_s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneBoundaryType::Type VectorMap::RightBoundaryType(
  const LaneInfoConstPtr lane) const {
  double nearest_s;
  double nearest_l;
  for (const auto &type :
       lane->lane().right_boundary().boundary_type()) {
    if (type.s() <= nearest_s) {
      if (type.types_size() > 0) {
        return type.types(0);
      } else {
        return LaneBoundaryType::UNKNOWN;
      }
    }
  }
  return LaneBoundaryType::UNKNOWN;
}

LaneWaypoint VectorMap::LeftNeighborWaypoint(
  const LaneWaypoint &waypoint) const {
  LaneWaypoint neighbor;
  if (!waypoint.lane) {
    return neighbor;
  }
  auto point = waypoint.lane->GetSmoothPoint(waypoint.s);
  for (const auto &lane_id :
       waypoint.lane->lane().left_neighbor_forward_lane_id()) {
    auto lane = GetLaneById(lane_id);
    if (!lane) {
      return neighbor;
    }
    double s = 0.0;
    double l = 0.0;
    if (!lane->GetProjection({point.x(), point.y()}, &s, &l)) {
      continue;
    }

    if (s < -kSampleDistance || s > lane->total_length() + kSampleDistance) {
      continue;
    } else {
      return LaneWaypoint(lane, s, l);
    }
  }
  return neighbor;
}

LaneWaypoint VectorMap::RightNeighborWaypoint(
  const LaneWaypoint &waypoint) const {
  LaneWaypoint neighbor;
  if (!waypoint.lane) {
    return neighbor;
  }
  auto point = waypoint.lane->GetSmoothPoint(waypoint.s);
  for (const auto &lane_id :
       waypoint.lane->lane().right_neighbor_forward_lane_id()) {
    auto lane = GetLaneById(lane_id);
    if (!lane) {
      return neighbor;
    }
    double s = 0.0;
    double l = 0.0;
    if (!lane->GetProjection({point.x(), point.y()}, &s, &l)) {
      continue;
    }
    if (s < -kSampleDistance || s > lane->total_length() + kSampleDistance) {
      continue;
    } else {
      return LaneWaypoint(lane, s, l);
    }
  }
  return neighbor;
}

int VectorMap::DistanceToLaneEnd(const common::PointENU &point,
                                 double &distance) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return -1;
  distance = nearest_lane->total_length() - nearest_s;
  return 0;
}

int VectorMap::GetLeftRightLaneIDs(const Id &id, Id &id_left,
                                   Id &id_right) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  int ret = 0;
  for (const auto &lane_id : lane->lane().left_neighbor_forward_lane_id()) {
    id_left = lane_id;
    ret += 1;
  }
  for (const auto &lane_id : lane->lane().right_neighbor_forward_lane_id()) {
    id_right = lane_id;
    ret += 2;
  }
  return ret;
}

int VectorMap::GetLaneSuccessorIDs(const Id &id,
                                   std::vector<Id> &ids_succ) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  int ret = 0;
  ids_succ.clear();
  for (const auto &lane_id : lane->lane().successor_id()) {
    ids_succ.push_back(lane_id);
    ret++;
  }
  return ret;
}

int VectorMap::GetLaneSpeedLimit(const Id &id, double &speed) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  if (lane->lane().has_speed_limit()) {
    speed = lane->lane().speed_limit();
    return 0;
  } else {
    speed = 0.;
    return 1;
  }
}

int VectorMap::GetLaneTurn(const Id &id,
                           avos::hdmap::Lane::LaneTurn &type) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  if (lane->lane().has_turn()) {
    type = lane->lane().turn();
    return 0;
  } else {
    return -1;
  }
}

int VectorMap::GetLaneType(const Id &id,
                           avos::hdmap::Lane::LaneType &type) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  if (lane->lane().has_type()) {
    type = lane->lane().type();
    return 0;
  } else {
    return -1;
  }
}

int VectorMap::GetLanePredecessorIDs(const Id &id,
                                     std::vector<Id> &ids_prede) const {
  LaneInfoConstPtr lane = GetLaneById(id);
  if (lane == nullptr) return -1;
  int ret = 0;
  ids_prede.clear();
  for (const auto &lane_id : lane->lane().predecessor_id()) {
    ids_prede.push_back(lane_id);
    ret++;
  }
  return ret;
}

int VectorMap::GetWidthToBoundary(const common::PointENU &point,
                                  double &left_width,
                                  double &right_width) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return -1;
  nearest_lane->GetWidth(nearest_s, &left_width, &right_width);
  left_width -= nearest_l;
  right_width += nearest_l;
  return 0;
}

int VectorMap::GetLaneWidth(const common::PointENU &point, double &width) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return -1;
  width = nearest_lane->GetWidth(nearest_s);
  return 0;
}

int VectorMap::GetWidthToNeighbor(const common::PointENU &point,
                                  double &left_width,
                                  double &right_width) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return -1;
  int ret = 0;
  LaneWaypoint waypoint(nearest_lane, nearest_s, nearest_l);
  LaneWaypoint lwp = LeftNeighborWaypoint(waypoint);
  if (lwp.lane != nullptr) {
    ret += 1;
    left_width = fabs(lwp.l);
  }
  LaneWaypoint rwp = RightNeighborWaypoint(waypoint);
  if (rwp.lane != nullptr) {
    ret += 2;
    right_width = fabs(lwp.l);
  }
  return ret;
}

bool VectorMap::IsOnLane(const common::PointENU &point, Id &lane_id) const {
  LaneInfoConstPtr nearest_lane;
  double nearest_s;
  double nearest_l;
  if (GetNearestLane(point, nearest_lane, nearest_s, nearest_l) == -1)
    return false;
  lane_id.set_id(nearest_lane->id().id());
  avos::common::math::Vec2d p(point.x(), point.y());
  return nearest_lane->IsOnLane(p);
}

int VectorMap::GetCrosswalks(const avos::common::PointENU& point, double distance,
                         std::vector<CrosswalkInfoConstPtr>* crosswalks) const {
  return impl_.GetCrosswalks(point, distance, crosswalks);
}



HDMapImpl* VectorMap::GetHDMapImplPtr() {
  return &impl_;
}

}  // namespace vectormap
}  // namespace avos
