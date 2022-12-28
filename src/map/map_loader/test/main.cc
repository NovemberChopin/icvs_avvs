#include "ros/ros.h"
#include "common/base/log/include/avos_node.h"
#include <string>
#include <unistd.h>
#include "map/map_loader/include/map_loader.h"
#include "map/map_loader/include/show_vectormap.h"
#include <fstream>
#include <iostream>

using namespace avos::map;
using namespace avos::hdmap;
using namespace avos::vectormap;
using avos::common::PointENU;

int main(int argc, char *argv[])
{
	std::string node_name = "map_loader_test";
	ros::init(argc, argv, node_name);
	ros::NodeHandle nh;
	bool Enable_GLOG_Screen = true;
	avos::common::AvosNode::Init(node_name, Enable_GLOG_Screen);

	Mapinfo* mapinfo = MapLoader::GetMapinfoPtr();

	MapItem map_item;
	if (!mapinfo->GetCurrentMap(map_item)) {
		AERROR << "GetAvailableMaps failed!";
		return -1;
	}
	AINFO << "GetCurrentMap: " << map_item.name << " - " << map_item.date;

	std::vector<MapItem> maps;
	if (!mapinfo->GetAvailableMaps(maps)) {
		AERROR << "GetAvailableMaps failed!";
		return -1;
	}
	for (auto map : maps) {
		AINFO << "GetAvailableMaps: " << map.name << " - " << map.date;
		if ((map.name == map_item.name) && (map.date == map_item.date)) {
			AINFO << "Got it";
		}
	}
	// AINFO << mapinfo->GetWorkspace();
	// AINFO << mapinfo->GetMapPath();
	// AINFO << mapinfo->GetMapHeader().name;
	// AINFO << mapinfo->GetMapHeader().date;
	// AINFO << mapinfo->GetMapParamHeader().image_matrix_size;
	// AINFO << mapinfo->GetMapParamHeader().basemap_image_size;
	// AINFO << mapinfo->GetMapParamHeader().basemap_resolution_ratio;
	// AINFO << mapinfo->GetMapParamHeader().image_pixel_chars;
	// AINFO << mapinfo->GetMapParamHeader().basemap_filename_level;
	// AINFO << mapinfo->GetMapParamHeader().basemap_dir;
	// AINFO << mapinfo->GetMapParamHeader().vector_map_file;
	// AINFO << mapinfo->GetMapinfoHeader().name;
	// AINFO << mapinfo->GetMapinfoHeader().zone;
	// AINFO << mapinfo->GetMapinfoHeader().version;
	// AINFO << mapinfo->GetMapinfoHeader().type;
	// AINFO << mapinfo->GetMapinfoHeader().describe;
	// AINFO << mapinfo->GetMapinfoHeader().date;

	// PointGCCS car_gccs;
	// if (LoadVectorFile(car_gccs) == -1) {
	// 	return -1;
	// }

	// AINFO << "car_pgccs: (" << car_gccs.xg << ", " << car_gccs.yg << ") ";

	// PointENU loc_enupoint;
	// loc_enupoint.set_x(car_gccs.xg);
	// loc_enupoint.set_y(car_gccs.yg);
	// BaseMap* basemap = MapLoader::GetBasemapPtr();
	// PointVCS pvcs = {0, 0, 0};
	// // basemap->UpdateShm(pgccs.xg, pgccs.yg);
	// if (basemap->IsInFreespace(car_gccs, pvcs)) {
	// 	AINFO << "ego(" << car_gccs.xg << ", " << car_gccs.yg << ") is in basemap.";
	// } else {
	// 	AINFO << "ego(" << car_gccs.xg << ", " << car_gccs.yg << ") is out basemap.";
	// }

	std::cout<<"................start gps test................."<<std::endl;
	const avos::vectormap::VectorMap* vcmap_ = avos::map::MapLoader::GetVectorMapPtr(); //init once
  	if (vcmap_ == nullptr) {
  		std::cout<<"vcmap_ is nulltpr."<<std::endl;
  	  return 0;
  	}
  	LaneInfoConstPtr lane_ptr = vcmap_->GetLaneById("199_1_-1");
  	double s, l_width, r_width;
	if (lane_ptr == NULL) {
		std::cout<<"can't find lane."<<std::endl;
	}
	
	if (s > lane_ptr->total_length()) {
		std::cout<<"s is too big."<<std::endl;
	}
	lane_ptr->GetRoadWidth(5.0, &l_width, &r_width);
	std::cout<<"road width size "<<lane_ptr->sampled_left_road_width().size()
		<<" l_width = "<<l_width<<" r_width = "<<r_width<<std::endl;
	for (auto width : lane_ptr->sampled_left_road_width()) {
		AERROR << "left width "<<width;
	}
	vector<OverlapInfoConstPtr> signals_ptr = lane_ptr->my_signals();
	std::cout<<"trafficlight size is "<<signals_ptr.size()<<std::endl;;
  	if (signals_ptr.size()) {
  	  for (auto &object : signals_ptr[0]->overlap().object()) {
  	    if (object.has_signal_overlap_info()) { 
  	      std::cout<<"trafficlight id is "<<object.id().id()<<std::endl;;
  	      break;
  	    }
  	  }
  	}

 //  	Id temp_id;
 //  	temp_id.set_id("167157881");
 //  	auto junction_ptr = avos::map::MapLoader::GetHDMapImplPtr()->
	// 					GetJunctionById(temp_id);
	// // vector<OverlapInfoConstPtr> junction = junction_ptr->my_signals();
	
	// const std::vector<Id>& stop_signs = junction_ptr->OverlapStopSignIds();
	// std::cout<<"junction stop sign size is "<<stop_signs.size()<<std::endl;	
	// for (const auto &temp_id : stop_signs) {
	// 	std::cout<<"junction stop sign id "<<temp_id.id()<<std::endl;
	// }



  	// for (const auto& overlap_id : junction_ptr->junction().overlap_id()) {
  	//   if (avos::map::MapLoader::GetHDMapImplPtr()->GetOverlapById(overlap_id) != nullptr) {
  	//   	std::cout<<"object size "<<avos::map::MapLoader::GetHDMapImplPtr()->GetOverlapById(overlap_id)->overlap().object().size()<<std::endl;
  	//     for (const auto& object :
  	//          avos::map::MapLoader::GetHDMapImplPtr()->GetOverlapById(overlap_id)->overlap().object()) {
  	//       if (object.has_signal_overlap_info() ||
  	//           object.has_stop_sign_overlap_info()) {
  	//         std::cout<<"junction stop sign id "<<std::endl;
	
  	//       }
  	//     }
  	//   }
  	// }

	sleep(5);

	// std::vector<PointXYA> points;
	// points.clear();
	// if (vcmap_->GetLaneCenterLine(lane_id, points) != 0) {
	// 	std::cout<<"can't find lane id "<<lane_id.id()<<std::endl;
	// 	return 0;
	// }
	// ofstream FileWriteLog;
	// FileWriteLog.open("/home/idriver/work/gps.csv");
	// if (FileWriteLog) {
	// 	std::cout<<"open success! point size is "<<points.size()<<std::endl;
	// 	FileWriteLog<<"................start test................."<<std::endl;
	// 	for (auto point : points) {
	// 		FileWriteLog<<setprecision(12)<<point.x<< "\t"<<point.y<<std::endl;
	// 	}
	// }
	// FileWriteLog.close();

	return 0;
}

