#include "map/map_loader/include/map_loader.h"
#include "common/base/log/include/avos_node.h"

using namespace avos::map;

int main(int argc, char **argv) {
	bool Enable_GLOG_Screen = true;
	avos::common::AvosNode::Init("free_shm", Enable_GLOG_Screen);

  BaseMap *base = MapLoader::GetBasemapPtr();
  base->CreatSharedMemory();
  base->FreeSharedMemory();
  return 0;
}