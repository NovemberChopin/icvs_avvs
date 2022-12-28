#include "read_xml.h"
#include "map/vectormap/include/geotool.h"
#include "map/common/coordination_transform/coordination_transform.h"

using avos::vectormap::GeoTool;
using avos::vectormap::PointVCS;
using avos::vectormap::PointGCCS;
using avos::vectormap::PointGPS;
using namespace std;

using geometry::Path;
using geometry::Site;
using geometry::SiteSet;
using geometry::SiteVec;
using geometry::LineBase;

namespace avos     {
namespace wxbmap {

ReadXml::ReadXml() {
  xml_data_.clear();

  if (!ReadXmlMap()) {
    AERROR << "WxbMap ReadXmlMap failed.";
    return;
  }
}

MapInfo* ReadXml::GetMapInfo(std::string area_id) {
  auto it = xml_data_.find(area_id);
  return it != xml_data_.end() ? it->second : nullptr;
}

bool ReadXml::ReadXmlMap() {
  xml_data_.clear();

  auto link_map = avos::map::MapLoader::GetLinkmapPtr();
  if (link_map == nullptr) {
    AERROR << "GetLinkmapPtr failed";
    return false;
  }

  for (auto area : link_map->GetLinkAreas()) {
    MapInfo* xml_info = new MapInfo;

    // 1. clear point_table and path_table and topo
    xml_info->point_table.clear();
    xml_info->path_table.clear();
    xml_info->topo.clear();

    // 2. get xmlvectormap
    // 2.1 get map name
    // ros::NodeHandle nh;
    // std::string map_name = "";
    // nh.param("/localization/map_name", map_name, map_name);
    // 2.2 get the vector map and check it
    const avos::xmlmap::XmlVectormap* xmlmap = area.map;
    RETURN_VAL_IF((nullptr == xmlmap) , false);
    avos::xmlmap::Vectormap* xml_vectormap = xmlmap->GetVectormapPtr();
    if (nullptr == xml_vectormap) {
      ROS_ERROR("call GetVectormapPtr in ReadXmlMap failed!");
      return false;
    }

    // 3. get point by traversing
    // 3.1 get point in order
    SiteSet point_set;
    // 3.2 sort function points by position
    for (const auto &p : xml_vectormap->points) {
      Site pos(p.position.center_point.x, p.position.center_point.y, p.position.yaw);
      pos.set_g(p.position.center_point.x, p.position.center_point.y, p.position.yaw);
      point_set.insert(pos);
    }
    // 3.3 put site into point_table with name "a-z" (name, pos)
    int ascii_start = 97; // letter 'a'
    for (const auto &p : point_set) {
      std::string name(1, (char)ascii_start);
      xml_info->point_table.emplace(name, p);
      ++ascii_start;
    }

    // 4. get path by jobs
    for (const auto &j : xml_vectormap->jobs) {
      // 4.1 if from_id and to_id are the same, this is a area clean path
      if (0 == j.to.compare(j.from)) {
        // AINFO<<"ID: "<<j.from;
        // std::string id;
        // auto rt = xmlmap->GetFunctionRegionIdByName("A", id);
        // AINFO<<"rt: "<<rt<<" id: "<<id;
        auto area = xmlmap->GetFunctionRegionById(j.to);
        RETURN_VAL_IF((nullptr == area), false);
        RETURN_VAL_IF(!(int)area->name().size(), false);
        char area_name_letter = (char)((int)area->name()[0] + 32);
        std::string area_name("a");
        std::string area_border_name("b");
        area_name += area_name_letter;
        area_border_name += area_name_letter;
        SiteVec seg;
        seg.reserve(6000);
        // 4.2.1 push this path to path_table
        avos::xmlmap::ConnectSegments segments_head;
        xmlmap->GetSegmentIdByJob(j.from, j.to, segments_head);
        RETURN_VAL_IF(segments_head.empty(), false);
        std::map<int, avos::xmlmap::ConnectSegment> sort_segments_head;
        for(int i=0; i<segments_head.size(); i++){
          auto segment_head = segments_head.at(i);
          int seg_order = -1;
          xmlmap->GetSegmentOrderBySegmentId(segment_head.id, j.id, seg_order);        
          // cout<<"segment_head.id: "<<segment_head.id<<endl;
          sort_segments_head.emplace(seg_order, segment_head);
        }
        for(auto it=sort_segments_head.begin(); it!=sort_segments_head.end(); it++){
          auto seg_id = it->second.id;
          // cout<<"seg_id: "<<seg_id<<endl;
          auto segment = xmlmap->GetSegmentById(seg_id);
          auto seg_type = xmlmap->GetSegmentTypeBySegmentId(seg_id);
          // cout<<"seg_type: "<<seg_type<<endl;
          auto bound_id = xmlmap->GetBoundaryLineIdBySegmentId(seg_id);
          // cout<<"bound_id: "<<bound_id<<endl;
          RETURN_VAL_IF((nullptr == segment), false);
          if (segment->segment().geometry.pointset.size() != segment->segment().geometry.angleset.size()) {
              AERROR << "pointset is not equal with angleset";
              return false;
          }
          if(!segment->segment().geometry.pointset.empty() &&
            !segment->segment().geometry.angleset.empty()){
            // cout<<"segment pointset.size: "<<segment->segment().geometry.pointset.size()<<endl;
            // cout<<"segment angleset.size: "<<segment->segment().geometry.angleset.size()<<endl;
            for (std::size_t i = 0; i < segment->segment().geometry.pointset.size(); i++) {
              Site seg_point(segment->segment().geometry.pointset[i].x, segment->segment().geometry.pointset[i].y, 0);//why? chang x y using xg yg
              seg_point.set_g(segment->segment().geometry.pointset[i].x, segment->segment().geometry.pointset[i].y, 0);
              seg_point.angle = segment->segment().geometry.angleset[i];
              seg_point.type = seg_type;
              seg_point.boundary_id = bound_id;
              seg.push_back(seg_point);
            }
            // for (const auto &p : segment->segment().geometry.pointset) {
            //   Site seg_point(p.x, p.y, 0);//why? chang x y using xg yg
            //   seg_point.set_g(p.x, p.y, 0);
            //   seg_point.type = seg_type;
            //   seg_point.boundary_id = bound_id;         
            //   seg.push_back(seg_point);
            // }
            // for (size_t i = 0; i < segment->segment().geometry.angleset.size(); ++i) {
            //   seg[i].angle = segment->segment().geometry.angleset[i];
            // }
          }
          // 4.2.2 save boundarys
          BoundaryInfo bound_info;
          std::string boundary_type;
          xmlmap->GetBoundaryLineTypeByBoundaryLineId(bound_id, boundary_type);
          bound_info.boundary_type = boundary_type;
          auto boundary = xmlmap->GetBoundaryLineByBoundaryLineId(bound_id);
          if(nullptr!=boundary){
            // cout<<"boundary_type: "<<boundary_type<<endl;
            // cout<<"boundary pointset.size: "<<boundary->boundaryline().geometry.pointset.size()<<endl;
            // cout<<"boundary angleset.size: "<<boundary->boundaryline().geometry.angleset.size()<<endl;          
            if(!boundary->boundaryline().geometry.pointset.empty() &&
              !boundary->boundaryline().geometry.angleset.empty()){
              for (const auto &p : boundary->boundaryline().geometry.pointset) {
                Site bound_point(p.x, p.y, 0);
                bound_point.set_g(p.x, p.y, 0);
                bound_point.type = bound_info.boundary_type;
                bound_info.boundary_points.push_back(bound_point);
              }
              for (size_t i = 0; i < boundary->boundaryline().geometry.angleset.size(); ++i) {
                bound_info.boundary_points.at(i).angle = boundary->boundaryline().geometry.angleset[i];
              }      
            }
            if (!bound_info.boundary_points.empty()) {
              xml_info->boundary_table.emplace(bound_id, bound_info);
            }
          }
        }
        xml_info->path_table.emplace(area_name, seg);
        // 4.2.2 save this area's border path
        auto fr = area->functionRegion(); //zgh : no understand
        LineBase border_l;
        LineVec l_vec;
        l_vec.clear();
        for (const auto &b : fr.curbs) {
          auto p_point = b.geometry.pointset;
          auto p_angle = b.geometry.angleset;
          border_l.p1.x     = p_point.front().x;
          border_l.p1.y     = p_point.front().y;
          border_l.p1.angle = p_angle.front();
          border_l.p2.x     = p_point.back().x;
          border_l.p2.y     = p_point.back().y;
          border_l.p2.angle = p_angle.back();
          border_l.p1.set_global(p_point.front().x, p_point.front().y, p_angle.front());
          border_l.p2.set_global(p_point.back().x, p_point.back().y, p_angle.back());
          l_vec.push_back(border_l);
        }
        if (!l_vec.empty()) {
          xml_info->border_table.emplace(area_name_letter, l_vec);
        }
      } else {
        // 4.3 if from_id is not same with to_id, this is a connect path
        std::string name_h, name_l;
        // 4.4 get name of from_id
        if (avos::xmlmap::FREGION == xmlmap->GetFunctionType(j.from)) {
          name_h = "a";
          auto area = xmlmap->GetFunctionRegionById(j.from);
          RETURN_VAL_IF((nullptr == area), false);
          RETURN_VAL_IF(!(int)area->name().size(), false);
          char area_name_letter = (char)((int)area->name()[0] + 32);
          name_h += area_name_letter;
        } else {
          name_h = "p";
          auto funp = xmlmap->GetFunctionPointById(j.from);
          RETURN_VAL_IF((nullptr == funp), false);
          Site target_pos;
          target_pos.xg    = funp->functionPoint().position.center_point.x;
          target_pos.yg    = funp->functionPoint().position.center_point.y;
          target_pos.angle = funp->functionPoint().position.yaw;
          char funp_name_letter;
          double min_dis = std::numeric_limits<double>::max();
          for (const auto &p : xml_info->point_table) {
            double tmp_dis = std::hypot((target_pos.yg - p.second.yg), (target_pos.xg - p.second.xg));
            if (tmp_dis < min_dis) {
              min_dis = tmp_dis;
              if (p.first.size()) { //add by zgh
                funp_name_letter = p.first[0];
              }
            }
          }
          name_h += funp_name_letter;
        }
        // 4.5 get name of to_id
        if (avos::xmlmap::FREGION == xmlmap->GetFunctionType(j.to)) {
          name_l = "a";
          auto area = xmlmap->GetFunctionRegionById(j.to);
          RETURN_VAL_IF((nullptr == area), false);
          RETURN_VAL_IF(!(int)area->name().size(), false);
          char area_name_letter = (char)((int)area->name()[0] + 32);
          name_l += area_name_letter;
        } else {
          name_l = "p";
          auto funp = xmlmap->GetFunctionPointById(j.to);
          RETURN_VAL_IF((nullptr == funp), false);
          Site target_pos;
          target_pos.xg    = funp->functionPoint().position.center_point.x;
          target_pos.yg    = funp->functionPoint().position.center_point.y;
          target_pos.angle = funp->functionPoint().position.yaw;
          char funp_name_letter;
          double min_dis = std::numeric_limits<double>::max();
          for (const auto &p : xml_info->point_table) {
            double tmp_dis = std::hypot((target_pos.yg - p.second.yg), (target_pos.xg - p.second.xg));
            if (tmp_dis < min_dis) {
              min_dis = tmp_dis;
              if (p.first.size()) { //add by zgh
                funp_name_letter = p.first[0];
              }
            }
          }
          name_l += funp_name_letter;
        }
        // 4.6 get total path name
        std::string path_name(name_h);
        path_name += name_l;
        // 4.7 get segment by connect seg id
        SiteVec seg;
        seg.reserve(2000);
        avos::xmlmap::ConnectSegments segments_head;
        xmlmap->GetSegmentIdByJob(j.from, j.to, segments_head);
        RETURN_VAL_IF(segments_head.empty() , false);
        auto segment = xmlmap->GetSegmentById(segments_head.front().id);
        RETURN_VAL_IF((nullptr == segment) , false);
        // cout<<"connect: "<<endl;
        // cout<<"segment_id: "<<segments_head.front().id<<endl;
        // cout<<"pointset.size = "<<segment->segment().geometry.pointset.size()<<endl;
        // cout<<"angleset.size = "<<segment->segment().geometry.angleset.size()<<endl;
        for (const auto &p : segment->segment().geometry.pointset) {
          Site seg_point(p.x, p.y, 0);
          seg_point.set_g(p.x, p.y, 0);
          seg.push_back(seg_point);
        }
        for (size_t i = 0; i < segment->segment().geometry.angleset.size(); ++i) {
          seg[i].angle = segment->segment().geometry.angleset[i];
        }
        // 4.8 push this path into path_table with name_h + name_l
        // optimizer::MeanFilt mft;
        // mft.Interpolation(seg, 0.08);
        xml_info->path_table.emplace(path_name, seg);
      }
    }
    // std::cout<< "xml_info->path_table.size()" << xml_info->path_table.size() << std::endl;
    // for (const auto &p : xml_info->path_table) {
    //   std::cout<< "p.first" << p.first << std::endl;
    // }
    // 5. get topo relations
    xml_info->topo.clear();
    xml_info->topo.reserve(200);
    std::map<std::string, size_t> id_map;
    size_t index = 0;
    // 5.1 get the map from id to index
    for (const auto &loc : xml_vectormap->pathpoint) {
      id_map.emplace(loc.id, index++);
      Topo_t this_topo;
      Site from(loc.center_point.x, loc.center_point.y, 0);
      from.set_g(loc.center_point.x, loc.center_point.y, 0);
      this_topo.first = from;
      xml_info->topo.push_back(this_topo);
    }
    // 5.2 traversing topology and complete topo info in context
    for (const auto &edge : xml_vectormap->topology) {
      size_t from_index = id_map[edge.id];
      for (const auto &connected_id : edge.connect_ids) {
        xml_info->topo[from_index].second.push_back(id_map[connected_id]);
      }
    }

    // 6. get free space

    xml_info->freespace.clear();
    xml_info->freespace_range.clear();
    xml_info->area_table.clear();
    // 6.1 read clear area optimizer::MeanFilt::Interpolation
    for (auto &region : xml_vectormap->regions) {
      for (const auto &p : region.outlines) {
        SiteVec list;
        if (!region.name.size()) return false; //add by zgh maybe is a bug
    RETURN_VAL_IF((nullptr == xmlmap) , false);
        char border_area_name = char((static_cast<int>(region.name.c_str()[0]) + 32));
        double min_x = std::numeric_limits<double>::max();
        double min_y = std::numeric_limits<double>::max();
        double max_x = std::numeric_limits<double>::min();
        double max_y = std::numeric_limits<double>::min();
        for (const auto &s : p.pointset) {
          list.emplace_back(s.x, s.y);
          if (s.x > max_x) max_x = s.x;
          if (s.x < min_x) min_x = s.x;
          if (s.y > max_y) max_y = s.y;
          if (s.y < min_y) min_y = s.y;
        }
        auto range_m = std::make_pair(Site(min_x, min_y), Site(max_x, max_y));
        auto area_range = std::make_pair(list, range_m);
        xml_info->freespace.push_back(list);
        xml_info->freespace_range.emplace_back(Site(min_x, min_y), Site(max_x, max_y));
        xml_info->area_table.insert(std::make_pair(border_area_name, list));
        xml_info->border_area_range.emplace(border_area_name, area_range);
      }
      // for (const auto &s : region.outline.pointset) {
      //   list.emplace_back(s.x, s.y);
      //   if (s.x > max_x) max_x = s.x;
      //   if (s.x < min_x) min_x = s.x;
      //   if (s.y > max_y) max_y = s.y;
      //   if (s.y < min_y) min_y = s.y;
      // }
    }
    // 6.2 read road area
    for (const auto &surface : xml_vectormap->surfaces) {
      SiteVec list;
      double min_x = std::numeric_limits<double>::max();
      double min_y = std::numeric_limits<double>::max();
      double max_x = std::numeric_limits<double>::min();
      double max_y = std::numeric_limits<double>::min();
      for (const auto &s : surface.outline.pointset) {
        list.emplace_back(s.x, s.y);
        if (s.x > max_x) max_x = s.x;
        if (s.x < min_x) min_x = s.x;
        if (s.y > max_y) max_y = s.y;
        if (s.y < min_y) min_y = s.y;
      }
      xml_info->freespace.push_back(list);
      xml_info->freespace_range.emplace_back(Site(min_x, min_y), Site(max_x, max_y));
    }
    if (xml_info->freespace.size() < 1 ||
        xml_info->freespace_range.size() < 1) {
      AERROR << "freespace.size=" << xml_info->freespace.size() << ", freespace_range.size=" << xml_info->freespace_range.size();
      return false;
    }
    
    std::string area_id = area.area_id;
    xml_data_.insert(std::pair<std::string, MapInfo*>(area_id, xml_info));
  }
  return true;
}

} // wxbmap
} // avos
