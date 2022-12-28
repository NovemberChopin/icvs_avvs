/******************************************************************************
 * Copyright (C) 2018-2023, idriverplus(BeiJing ZhiXingZhe, Inc.)
 *
 * NodeName: ivplanner
 * FileName: typedef.h
 *
 * Description: define custom types
 *
 * History:
 * Feng younan          2018/08/17    v1.5.1        build this model.
 ******************************************************************************/
#ifndef __IVPLANNER_TYPEDEF_H__
#define __IVPLANNER_TYPEDEF_H__

#include <map>
#include <memory>
#include <stack>
#include <vector>
#include <list>
#include "map/common/geometry/geoheader.h"

using geometry::Site;
using geometry::SiteVec;
using geometry::LineBase;
using geometry::LineVec;


enum class HDMap {
  FREEDRIVER,
  PARALLEL,
  LOADPOINT,
  CLEANAREA,
  DUMPSTER,
  CONNECTION,
  BASEROOM,
  ISOLATION,
  SPEEDBUMP,
  UNREACHEABLE,
};

enum class Alarm {
  NONE,
  REVERSABLE,
  WAITREVERSE,
  NOAVOID,
  SLOW,
};

struct BaseMap_Protocol {
  HDMap hdmap;
  Alarm alarm;
  bool freespace_valid;
  void reset() {
    hdmap = HDMap::FREEDRIVER;
    alarm = Alarm::NONE;
    freespace_valid = false;
  }

  const BaseMap_Protocol& operator=(const BaseMap_Protocol& rv) {
    hdmap           = rv.hdmap;
    alarm           = rv.alarm;
    freespace_valid = rv.freespace_valid;

    return *this;
  }
};

struct PathInfo {
  std::string             file_path;
  std::stack<std::string> seg_sequency;
  int                     current_seg;
  bool                    reachable_flag;
  bool                    seg_end;
  bool                    load_seg;
  void reset() {
    while (!seg_sequency.empty()) seg_sequency.pop();
    current_seg = 0;
    reachable_flag = true;
    seg_end = false;
    load_seg = false;
  }

  const PathInfo& operator=(const PathInfo& rv) {
    file_path      = rv.file_path;
    seg_sequency   = rv.seg_sequency;
    current_seg    = rv.current_seg;
    reachable_flag = rv.reachable_flag;
    seg_end        = rv.seg_end;
    load_seg       = rv.load_seg;

    return *this;
  }
};


struct CurbInfo {
  // std::string work_mode;
  // std::vector<std::tuple<Site, Site, double, Site>> sdc; //start_direction_confidence_DRpos;
  // int buff_size;
  // long time_stamp;
  // bool detect_enable;
  // std::pair<Site, Site> curb_line_points;
  // double offset;
  // bool is_border_clean;
  // bool is_smooth_trans;
  // bool will_be_border_clean;
  // bool control_enable;
  // bool speed_down;
  // double refpath_offset;
  // std::tuple<Site, Site, double> transmit_sdc; //start_direction_confidence,
  std::map<char, LineVec>   curb_sequency;// zhangzhuo [0614]
  LineBase   current_curb; // zhangzhuo [0614]
  int        current_curb_valid;
  void reset() {
    curb_sequency.clear();
    current_curb_valid = -1;
    // sdc.clear();
    // refpath_offset = 0.0;
    // transmit_sdc = std::make_tuple(Site(), Site(), 0);
    // detect_enable = false;
    // will_be_border_clean = false;
    // is_border_clean = false;
    // is_smooth_trans = false;
    // control_enable = false;
    // speed_down = false;
  }
  void reset_all() {
   curb_sequency.clear();
   current_curb_valid = -1;
    // work_mode = "lidar";
    // sdc.clear();
    // buff_size = 0;
    // time_stamp = 0;
    // detect_enable = false;
    // offset = 0.0;
    // will_be_border_clean = false;
    // is_border_clean = false;
    // control_enable = false;
    // speed_down = false;
    // transmit_sdc = std::make_tuple(Site(), Site(), 0);
  }

  const CurbInfo& operator=(const CurbInfo& rv) {
    curb_sequency           = rv.curb_sequency;
    current_curb            = rv.current_curb;
    current_curb_valid      = rv.current_curb_valid;
    // work_mode            = rv.work_mode;
    // sdc                  = rv.sdc;
    // buff_size            = rv.buff_size;
    // time_stamp           = rv.time_stamp;
    // detect_enable        = rv.detect_enable;
    // curb_line_points     = rv.curb_line_points;
    // offset               = rv.offset;
    // is_border_clean      = rv.is_border_clean;
    // is_smooth_trans      = rv.is_smooth_trans;
    // will_be_border_clean = rv.will_be_border_clean;
    // control_enable       = rv.control_enable;
    // speed_down           = rv.speed_down;
    // refpath_offset       = rv.refpath_offset;
    // transmit_sdc         = rv.transmit_sdc;

    return *this;
  }
};

struct BoundaryInfo {
  std::string boundary_type;
  std::vector<Site> boundary_points;
  void reset() {
    boundary_type = "";
    boundary_points.clear();
  }
};

using Topo_t      = std::pair<Site, std::vector<int>>;
using range_t     = std::pair<Site, Site>;
using area_range  = std::pair<SiteVec, range_t>;
using CleanArea_t = std::map<char, SiteVec>;

struct MapInfo {
  BaseMap_Protocol                 basemap;
  PathInfo                         path;
  SiteVec                          vir_cells;
  CurbInfo                         curb_info;
  std::map<std::string, Site>      point_table;
  std::map<std::string, SiteVec>   path_table;
  std::map<char, LineVec>          border_table; //for border clean
  std::map<char, area_range>       border_area_range; //for border clean
  CleanArea_t                      area_table;
  std::vector<Topo_t>              topo;
  std::vector<SiteVec>             freespace;
  std::vector<range_t>             freespace_range;
  std::map<std::string, BoundaryInfo>      boundary_table;  

  void reset() {
    path.reset();
    curb_info.reset();
    boundary_table.clear();
  }
  void reset_all() {
    basemap.reset();
    path.reset();
    curb_info.reset_all();
    vir_cells.clear();
    point_table.clear();
    path_table.clear();
    topo.clear();
    freespace.clear();
    freespace_range.clear();
    border_table.clear();
    border_area_range.clear();
    boundary_table.clear();
  }

  const MapInfo& operator=(const MapInfo& rv) {
    basemap           = rv.basemap;
    path              = rv.path;
    vir_cells         = rv.vir_cells;
    curb_info         = rv.curb_info;
    point_table       = rv.point_table;
    path_table        = rv.path_table;
    border_table      = rv.border_table;
    border_area_range = rv.border_area_range;
    topo              = rv.topo;
    freespace         = rv.freespace;
    freespace_range   = rv.freespace_range;
    boundary_table    = rv.boundary_table;
    return *this;
  }
};

struct LocData
{
  double xg;
  double yg;
  double globalangle;
  LocData() {
    xg = 0.0;
    yg = 0.0;
    globalangle = 0.0;
  }
  LocData(double _xg, double _yg, double _angle) {
    xg = _xg;
    yg = _yg;
    globalangle = _angle;
  }
  void Set(double _xg, double _yg, double _angle) {
    xg = _xg;
    yg = _yg;
    globalangle = _angle;
  }
};

struct LocDRData
{
  double dr_x;
  double dr_y;
  double dr_angle;
  LocDRData() {
    dr_x = 0.0;
    dr_y = 0.0;
    dr_angle = 0.0;
  }
  LocDRData(double _dr_x, double _dr_y, double _dr_angle) {
    dr_x = _dr_x;
    dr_y = _dr_y;
    dr_angle = _dr_angle;
  }
  void Set(double _dr_x, double _dr_y, double _dr_angle) {
    dr_x = _dr_x;
    dr_y = _dr_y;
    dr_angle = _dr_angle;
  }
};

#endif  // __IVPLANNER_TYPEDEF_H__
