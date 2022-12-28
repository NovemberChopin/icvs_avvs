#include "map/map_loader/include/map_loader.h"
#include "common/base/log/include/avos_node.h"

using namespace avos::map;

int main(int argc, char **argv) {
	bool Enable_GLOG_Screen = true;
	avos::common::AvosNode::Init("create_shm", Enable_GLOG_Screen);

  BaseMap *basemap_ = MapLoader::GetBasemapPtr();
  auto r = basemap_->CreatSharedMemory();
  return 0;
}