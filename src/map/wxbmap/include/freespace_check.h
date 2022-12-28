#ifndef __IVPLANNER_FREESPACE_CHECK_H__
#define __IVPLANNER_FREESPACE_CHECK_H__

// #include <ros/ros.h>
#include "map/common/macro.h"
#include "typedef.h"
#include "map/vectormap/include/geotool.h"
#include "map/common/geometry/path.h"
#include "map/common/geometry/geoheader.h"
#include "map/map_loader/include/map_loader.h"
#include <locale>
#include <limits>
#include "map/common/coordination_transform/coordination_transform.h"
#include <gtest/gtest.h>

using namespace std;
using namespace avos::map;
using namespace avos::xmlmap;

namespace avos     {
namespace wxbmap {

struct MapProto {
  HDMap func;
  Alarm alam;
};

class FreespaceCheck {
  DECLARE_SINGLETON(FreespaceCheck);
public:
  bool Init(const double &x_offset, const double &y_offset, const double &front_over_hang,
            const double &back_over_hang, const double &half_wheel);
  bool behavior_state(bool is_in_border_clean_plus);
  bool VCSFSCheck (const Site &vccs); //todo  xy   not impl
  bool GCCSFSCheck (const Site &gccs, bool search_flag = false);
  bool GCCSFSCheck (const avos::vectormap::PointGCCS &gccs, const avos::vectormap::PointVCS &vcs);
  bool MODLFSCheck (const Site &gccs, double front_over_hang = 1.3,
                    double back_over_hang = 0.3, double half_wheel = 0.45, bool search_flag = false);

  bool MODLPathFSCheck (std::list<Site> &gccs_list, Site &collision_site, double front_over_hang = 1.3,
                        double back_over_hang = 0.3, double half_wheel = 0.45,
                        double valid_dis = numeric_limits<double>::max());
  bool MODLPathFSAdapterCheck (std::vector<Site> &gccs_list,
                               const std::vector<double> &offset,
                               Site &collision_site,
                               double front_over_hang = 1.3,
                               double back_over_hang = 0.3,
                               double half_wheel = 0.45,
                               double valid_dis = numeric_limits<double>::max());
  bool MODLPathFSCheck_dis (std::list<Site> &gccs_list, Site &collision_site, double& collid_dis, double front_over_hang = 1.3,
                        double back_over_hang = 0.3, double half_wheel = 0.45,
                        double valid_dis = numeric_limits<double>::max());
  bool MODLPathFSAdapterCheck_dis (std::list<Site> &gccs_list,
                                   Site &collision_site,
                                   double& collid_dis,
                                   double front_over_hang ,
                                   double back_over_hang ,
                                   double half_wheel,
                                   double valid_dis);

  MapProto BaseMapType(const LocData &loc_data);

  template<class T> 
  bool IsPathTypeBorder(const T& path, double scale){
    double total_dis = 0.0;
    for(auto it=path.begin(); std::next(it,1)!=path.end(); it++){
      auto next_it = std::next(it,1);
      double temp_dis = hypot(it->xg-next_it->xg, it->yg-next_it->yg);
      total_dis += temp_dis;
      if(total_dis>scale+1e-3){
        return false;
      }else{
        if(3==it->property) return true;
      }
    }
    return false;
  }

  template<class T> 
  bool IsInBorderPath(const T& paths, double front_dis=2.0, double back_dis=2.0){
    const auto& front_path = paths.front_local_path.path;
    const auto& back_path = paths.back_local_path.path;
    if(IsPathTypeBorder(front_path, front_dis) || IsPathTypeBorder(back_path, back_dis)) 
      return true;
    else
      return false;
  }

  bool SetLocPos(const Site& loc_pos);

private:
  bool Check();
  bool isInFreespacePlus(const Site &vccs);
  bool IsEgoInFreespace();
  
private:
  BaseMap *basemap_;
  double map_x_offset_;
  double map_y_offset_;
  double free_drive_front_over_hang_;
  double free_drive_back_over_hang_;
  double free_drive_half_wheel_;
  bool is_in_border_clean_plus_;
  Site loc_pos_;

};


}
}
#endif  // __IVPLANNER_FREESPACE_CHECK_H__
