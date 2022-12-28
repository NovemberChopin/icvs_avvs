#include "freespace_check.h"

using avos::vectormap::GeoTool;
using avos::vectormap::PointVCS;
using avos::vectormap::PointGCCS;
using avos::vectormap::PointGPS;
using namespace std;

#define FREESPACE_COLLISION_INNER 1
namespace avos     {
namespace wxbmap {

FreespaceCheck::FreespaceCheck() {
  basemap_ = nullptr;
  is_in_border_clean_plus_ = false;
}

bool FreespaceCheck::Init(const double &x_offset, const double &y_offset, const double &front_over_hang,
                          const double &back_over_hang, const double &half_wheel) {
  instance()->map_x_offset_ = x_offset;
  instance()->map_y_offset_ = y_offset;
  instance()->free_drive_front_over_hang_ = front_over_hang;
  instance()->free_drive_back_over_hang_ = back_over_hang;
  instance()->free_drive_half_wheel_ = half_wheel;
}

bool FreespaceCheck::Check() {
  if (nullptr != instance()->basemap_)  return true;
  if (nullptr == instance()->basemap_) instance()->basemap_ = MapLoader::GetBasemapPtr();
  return (nullptr != instance()->basemap_);
}

bool FreespaceCheck::behavior_state(bool is_in_border_clean_plus) {
  is_in_border_clean_plus_ = is_in_border_clean_plus;
  return true;
}

bool FreespaceCheck::GCCSFSCheck (const Site &gccs, bool search_flag) {
  if (!Check()) return false;
  if (is_in_border_clean_plus_ && !search_flag) {
    PointGCCS car_pos;
    PointVCS  vcs;
    car_pos.xg     = gccs.xg + instance()->map_x_offset_;
    car_pos.yg     = gccs.yg + instance()->map_y_offset_;
    car_pos.angle  = gccs.globalangle;
    Site ego_pos;
    ego_pos.set_global(gccs.xg + instance()->map_x_offset_, gccs.yg + instance()->map_y_offset_, gccs.globalangle);
    CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
    Site vccs;
    cor_trans_ -> GCCS2VCS(ego_pos, gccs, vccs);
    bool freespace_plus = isInFreespacePlus(vccs);
    bool freespace_normal = instance()->basemap_->IsInFreespace(car_pos, vcs);
    return (freespace_plus || freespace_normal) && IsEgoInFreespace();
    // return isInFreespacePlus(vccs);
    // return true;
  } else {
    PointGCCS car_pos;
    PointVCS  relative_pos;
    car_pos.xg     = gccs.xg + instance()->map_x_offset_;
    car_pos.yg     = gccs.yg + instance()->map_y_offset_;
    car_pos.angle  = gccs.globalangle;
    relative_pos.x = 0;
    relative_pos.y = 0;
    return instance()->basemap_->IsInFreespace(car_pos, relative_pos);
  }
}

//reloaded by DZ for gccs-vcs form, check the vcs whther is in freespace.
bool FreespaceCheck::GCCSFSCheck (const PointGCCS &gccs, const PointVCS &vcs) {
  if (!Check()) return false;
  if (0 && is_in_border_clean_plus_) {
    PointGCCS car_pos;
    car_pos.xg     = gccs.xg + instance()->map_x_offset_;
    car_pos.yg     = gccs.yg + instance()->map_y_offset_;
    car_pos.angle  = gccs.angle;
    CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
    Site vccs;
    PointVCS temp_vcs;
    cor_trans_ -> GCCS2VCS (car_pos, gccs, temp_vcs); // zgh bug
    vccs.set_g(temp_vcs.x, temp_vcs.y, temp_vcs.angle);
    bool freespace_plus = isInFreespacePlus(vccs);
    bool freespace_normal = instance()->basemap_->IsInFreespace(car_pos, vcs);
    return (freespace_plus || freespace_normal) && IsEgoInFreespace();
    // return isInFreespacePlus(vccs);
    // return true;
  } else {
    PointGCCS car_pos;
    car_pos.xg     = gccs.xg + instance()->map_x_offset_;
    car_pos.yg     = gccs.yg + instance()->map_y_offset_;
    car_pos.angle  = gccs.angle;
    return instance()->basemap_->IsInFreespace(car_pos, vcs);
  }
}

bool FreespaceCheck::MODLFSCheck (const Site &gccs, double front_over_hang ,
                                  double back_over_hang , double half_wheel, bool search_flag) {
  if (!Check()) return false;
  if (is_in_border_clean_plus_ && !search_flag) {
    // std::cout<<"USE border check, returns true"<<std::endl;
    return true;
  }
  PointGCCS car_pos, toPrint, toCalc;
  PointVCS  fl, fr, rl, rr, center_pt;
  car_pos.xg     = gccs.xg + instance()->map_x_offset_;
  car_pos.yg     = gccs.yg + instance()->map_y_offset_;
  car_pos.angle  = gccs.globalangle;
  // car_pos.xg     = gccs.xg; //add by zgh
  // car_pos.yg     = gccs.yg ;
  // car_pos.angle  = gccs.globalangle;
  toCalc.xg     = gccs.xg;
  toCalc.yg     = gccs.yg;
  toCalc.angle  = gccs.globalangle;
  fl.x =  1 * front_over_hang;
  fl.y =  1 * half_wheel;
  fr.x =  1 * front_over_hang;
  fr.y = -1 * half_wheel;
  rl.x = -1 * back_over_hang;
  rl.y =  1 * half_wheel;
  rr.x = -1 * back_over_hang;
  rr.y = -1 * half_wheel;
  center_pt.x = 1.0 * front_over_hang;
  center_pt.y = 0.0;

  bool result_fl = instance()->basemap_->IsInFreespace(car_pos, fl);
  // bool result_fl = GCCSFSCheck (car_pos, fl); //add by zgh
  // if(!result_fl)
  // {
  //   CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
  //   cor_trans_->VCS2GCCS(toCalc, fl, toPrint);
  //   // AERROR<<" Out of FS: FL "<<fl.x<<" "<<fl.y<<"| "<<toPrint.xg<<" "<<toPrint.yg;
  // }

  bool result_fr = instance()->basemap_->IsInFreespace(car_pos, fr);
  // bool result_fr = GCCSFSCheck (car_pos, fr); //add by zgh
  // if(!result_fr)
  // {
  //   CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
  //   cor_trans_->VCS2GCCS(toCalc, fr, toPrint);
  //   // AERROR<<" Out of FS: FR "<<fr.x<<" "<<fr.y<<"| "<<toPrint.xg<<" "<<toPrint.yg;
  // }

  bool result_rl = instance()->basemap_->IsInFreespace(car_pos, rl);
  // bool result_rl = GCCSFSCheck (car_pos, rl); //add by zgh
  // if(!result_rl)
  // {
  //   CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
  //   cor_trans_->VCS2GCCS(toCalc, rl, toPrint);
  //   // AERROR<<" Out of FS: RL "<<rl.x<<" "<<rl.y<<"| "<<toPrint.xg<<" "<<toPrint.yg;
  // }

  bool result_rr = instance()->basemap_->IsInFreespace(car_pos, rr);
  // bool result_rr = GCCSFSCheck (car_pos, rr); //add by zgh
  // if(!result_rr)
  // {
  //   CoordinationTransform *cor_trans_ = CoordinationTransform::instance();
  //   cor_trans_->VCS2GCCS(toCalc, rr, toPrint);
  //   // AERROR<<" Out of FS: RR "<<rr.x<<" "<<rr.y<<"| "<<toPrint.xg<<" "<<toPrint.yg;
  // }
  bool result_center = instance()->basemap_->IsInFreespace(car_pos, center_pt);

  return (result_fl && result_fr && result_rl && result_rr && result_center);
}



bool FreespaceCheck::MODLPathFSCheck (std::list<Site> &gccs_list, Site &collision_site,  double front_over_hang ,
                                      double back_over_hang , double half_wheel,
                                      double valid_dis) {
  if (!Check()) return false;
  if (is_in_border_clean_plus_) {
    return true;
  }
  int count = 0;

  if (gccs_list.size() == 0) return true;

  if (gccs_list.size() == 1) return MODLFSCheck(gccs_list.front(), front_over_hang, back_over_hang, half_wheel);
  double dis = 0;
  auto it = gccs_list.begin();
  int counter = 0;
  for (; it != gccs_list.end() && std::next(it, 1) != gccs_list.end(); ++it) {
    double temp = std::hypot(it->x - std::next(it, 1)->x , it->y - std::next(it, 1)->y);
    dis += temp;
    count++;
    counter++;

    if (dis > valid_dis) break;
    if (counter % FREESPACE_COLLISION_INNER == 0) {
      if (MODLFSCheck(*it, front_over_hang, back_over_hang, half_wheel) == false) {
        collision_site = *it;
        collision_site.index = count;
        return false;
      }
    }

  }
  collision_site.index = 9999;
  return true;
}

bool FreespaceCheck::MODLPathFSAdapterCheck (std::vector<Site> &gccs_list,
    const std::vector<double> &offset,
    Site &collision_site,
    double front_over_hang ,
    double back_over_hang,
    double half_wheel,
    double valid_dis) {
  if (!Check()) return false;
  if (is_in_border_clean_plus_) {
    return true;
  }
  int count = 0;

  if (gccs_list.size() == 0) return true;

  if (gccs_list.size() == 1) return MODLFSCheck(gccs_list.front(), front_over_hang, back_over_hang, half_wheel);
  double dis = 0;
  auto it = gccs_list.begin();
  int counter = 0;
  for (; it != gccs_list.end() && std::next(it, 1) != gccs_list.end(); ++it) {
    double temp = std::hypot(it->x - std::next(it, 1)->x , it->y - std::next(it, 1)->y);
    dis += temp;
    count++;
    counter++;
    if (dis > valid_dis) break;
    double adapter_half_wheel = half_wheel + offset.at(count - 1);
    if (counter % FREESPACE_COLLISION_INNER == 0) {
      if (MODLFSCheck(*it, front_over_hang, back_over_hang, adapter_half_wheel) == false) {
        collision_site = *it;
        collision_site.index = count;
        return false;
      }
    }
  }
  collision_site.index = 9999;
  return true;
}

bool FreespaceCheck::MODLPathFSCheck_dis (std::list<Site> &gccs_list, Site &collision_site, double& collid_dis, double front_over_hang ,
    double back_over_hang , double half_wheel,
    double valid_dis) {
  collid_dis = numeric_limits<double>::max();
  if (!Check()) return false;
  // if (is_in_border_clean_plus_) {
  //   return true;
  // }
  int count = 0;

  int counter = 0;
  if (gccs_list.size() == 0) return true;

  bool ignore_border = true;
  if (gccs_list.size() == 1) return MODLFSCheck(gccs_list.front(), front_over_hang, back_over_hang, half_wheel, ignore_border);
  double dis = 0;
  auto it = gccs_list.begin();
  for (; it != gccs_list.end() && std::next(it, 1) != gccs_list.end(); ++it) {
    double temp = std::hypot(it->x - std::next(it, 1)->x , it->y - std::next(it, 1)->y);
    dis += temp;
    count++;
    counter++;
    if (dis > valid_dis) break;
    if (counter % FREESPACE_COLLISION_INNER == 0) {
      if (MODLFSCheck(*it, front_over_hang, back_over_hang, half_wheel, ignore_border) == false) {
        collid_dis = dis;
        collision_site = *it;
        collision_site.index = count;
        return false;
      }
    }
  }
  collision_site.index = 9999;
  return true;
}

bool FreespaceCheck::MODLPathFSAdapterCheck_dis (std::list<Site> &gccs_list,
    Site &collision_site,
    double& collid_dis,
    double front_over_hang ,
    double back_over_hang ,
    double half_wheel,
    double valid_dis) {
  collid_dis = numeric_limits<double>::max();
  if (!Check()) return false;
  // if (is_in_border_clean_plus_) {
  //   return true;
  // }
  int count = 0;
  int counter = 0;

  if (gccs_list.size() == 0) return true;

  bool ignore_border = true;
  if (gccs_list.size() == 1) return MODLFSCheck(gccs_list.front(), front_over_hang, back_over_hang, half_wheel, ignore_border);
  double dis = 0;
  auto it = gccs_list.begin();
  for (; it != gccs_list.end() && std::next(it, 1) != gccs_list.end(); ++it) {
    double temp = std::hypot(it->x - std::next(it, 1)->x , it->y - std::next(it, 1)->y);
    dis += temp;
    count++;
    counter++;
    if (dis > valid_dis) break;
    double half_wheel_off = half_wheel + it->velocity;
    if (counter % FREESPACE_COLLISION_INNER == 0) {
      if (MODLFSCheck(*it, front_over_hang, back_over_hang, half_wheel_off, ignore_border) == false) {
        collid_dis = dis;
        collision_site = *it;
        collision_site.index = count;
        return false;
      }
    }
  }
  collision_site.index = 9999;
  return true;
}

bool FreespaceCheck::isInFreespacePlus(const Site &vccs) {
  geometry_msgs::Polygon poly;
  geometry_msgs::Point32 point;
  // point = vccs;
  point.x = vccs.x;
  point.y = vccs.y;
  point.z = vccs.angle / 180.0 * M_PI;
  double front_over_hang = instance()->free_drive_front_over_hang_;
  double back_over_hang = instance()->free_drive_back_over_hang_;
  double half_wheel = instance()->free_drive_half_wheel_;
  geometry::carmodel(point, poly, front_over_hang,
                     back_over_hang,
                     half_wheel);
  // Site vcs, gccs, modelPt;
  for (const auto &a : poly.points) {
    if (fabs(a.y) < half_wheel + 0.1 && a.x > 0.0 && a.x < 2.0) {
      return true;
    } else {
      return false;
    }
  }
}

bool FreespaceCheck::IsEgoInFreespace() {
  PointGCCS car_pos;
  PointVCS  vcs;
  car_pos.xg     = loc_pos_.xg + instance()->map_x_offset_;
  car_pos.yg     = loc_pos_.yg + instance()->map_y_offset_;
  car_pos.angle  = loc_pos_.angle;
  vcs.x = 0.0;
  vcs.y = 0.0;
  return instance()->basemap_->IsInFreespace(car_pos, vcs);
}


MapProto FreespaceCheck::BaseMapType(const LocData &loc_data) {
  MapProto result;
  if (!Check()) return result;
  PointGCCS carPos;
  PointVCS  vs;
  carPos.xg    = loc_data.xg + instance()->map_x_offset_;
  carPos.yg    = loc_data.yg + instance()->map_y_offset_;
  carPos.angle = loc_data.globalangle;
  vs.x         = 0;
  vs.y         = 0;
  auto return_type = instance()->basemap_->GetFunctionType(carPos, vs);
  if (!return_type.IsValid) {
    // ROS_ERROR_STREAM("READ BASEMAP FAILD");
    return result;
  }
  int warning_area  = return_type.AttentionRegionType;
  int function_area = return_type.FunctionRegionType;
  switch (warning_area) {
  case 0:
    result.alam = Alarm::NONE;
    break;
  case 1:
    result.alam = Alarm::REVERSABLE;
    break;
  case 2:
    result.alam = Alarm::WAITREVERSE;
    break;
  case 3:
    result.alam = Alarm::NOAVOID;
    break;
  case 4:
    result.alam = Alarm::SLOW;
    break;
  default:
    result.alam = Alarm::NONE;
    break;
  }
  switch (function_area) {
  case 0:
    result.func = HDMap::UNREACHEABLE;
    break;
  case 1:
    result.func = HDMap::CONNECTION;
    break;
  case 2:
    result.func = HDMap::CLEANAREA;
    break;
  default:
    result.func = HDMap::UNREACHEABLE;
    break;
  }
  return result;
}


bool FreespaceCheck::SetLocPos(const Site& loc_pos){
  loc_pos_ = loc_pos;
  return true;
}

} // pathplan
} // avos
