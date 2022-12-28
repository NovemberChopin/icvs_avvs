#include <gtest/gtest.h>
#include "map/map_loader/include/map_loader.h"

using namespace avos::xmlmap;
using namespace avos::map;

using namespace basemap_shm_util;
PointVCS GetVCS(int row, int col) {
  PointVCS temp;
  temp.x = col * 0.05;
  temp.y = (1023 - row) * 0.05;
  temp.angle = 0;
  return temp;
}
/*
TEST(shm_update_util, GetFileName) {
  ull gics;
  *((uint *)&gics) = 120;
  *((uint *)&gics + 1) = 110;
  auto name = GetFileName(&gics, 50);
  EXPECT_EQ(name, "120_110_50.bin");
}

TEST(shm_update_util, GetGICS) {
  ull gics;
  *((uint *)&gics) = 120;
  *((uint *)&gics + 1) = 110;
  auto g = GetGICS(110, 120, 1);
  EXPECT_EQ(g, gics);

  char p[32];
  auto r = GetGICS(110, 120, 20, 0.05, p, sizeof(p));
  EXPECT_EQ(r, 0);
  EXPECT_EQ(*((ull *)p), g);
  EXPECT_EQ(*((ull *)p + 1), 19 * 20);
  char te[32];
}

TEST(shm_update_util, GetUpdatePairs) {
  ull gics;
  *((uint *)&gics) = 20;
  *((uint *)&gics + 1) = 30;
  ull gics1;
  *((uint *)&gics1) = 20 - 1;
  *((uint *)&gics1 + 1) = 30 + 2;
  int num = 25 * 16;
  char p[num];
  auto re = GetUpdatePairs(gics, gics1, 5, p, 25 * 16);
  EXPECT_GE(re, 0);
  std::cout << re << std::endl;
  cv::Mat img(40, 40, CV_8UC3, cv::Scalar(255, 255, 255));
  for (int i = 0; i < re; i++) {
    int r = *((uint *)p + 4 * i + 0), c = *((uint *)p + 4 * i + 1);
    unsigned char co = i * 10;
    img.at<cv::Vec3b>(r, c) = {co, 0, 0};
    r = *((uint *)p + 4 * i + 2);
    c = *((uint *)p + 4 * i + 3);
    img.at<cv::Vec3b>(r, c) = {0, 0, co};
  }
  cv::namedWindow("img", CV_WINDOW_NORMAL);
  cv::imshow("img", img);
  cv::waitKey(1);
}
*/
/*
int shmid;
void *shared;
#define SIZE (13380 * 13380)

TEST(basemap_shm_util, CreatMemory) {
  shmid = CreatMemory(1234, SIZE * 4);
  std::cout << "creatmemory shmid: " << shmid << std::endl;
  EXPECT_GE(shmid, 0);
}
TEST(basemap_shm_util, NewMemory) {
  int r = NewMemoryPtr(&shared, shmid);
  EXPECT_EQ(r, 0);
  std::cout << "newmemory shmid" << std::endl;
}
TEST(basemap_shm_util, WriteMemoryByData) {
  // char data[1000] = {1};//heap has max size < 13380*13380
  char *data = new (char[SIZE]);
  data[5] = 111;
  int r = WriteMemoryByData(shared, data, 0, SIZE);
  delete data;
  std::cout << "555555555: "<<(int)(((char*)shared)[5]) << std::endl;
  EXPECT_EQ(r, 0);
}
TEST(basemap_shm_util, WriteMemoryByFile) {
  std::string name =
      "/home/idriver/work/AVOS20/src/map/basemap/map/tile_map/"
      "Tile_6631_663_19.bin";
  int r = WriteMemoryByFile(shared, name, 0, SIZE * 4);
  cv::Mat map(13380, 13380, CV_8UC4, (char *)shared);
  cv::Vec4b vec4b;
  vec4b = map.at<cv::Vec4b>(3709, 587);
  unsigned char rgba[4];
  rgba[0] = vec4b[2];
  rgba[1] = vec4b[1];
  rgba[2] = vec4b[0];
  rgba[3] = 127;
  for (int i = 0; i < 4; i++) {
    std::cout << (int)(*((unsigned char *)shared + 3709 * 13380 * 4 + 587 * 4 +
                         i))
              << std::endl;
    std::cout << (int)(rgba[i]) << std::endl;
  }
  EXPECT_EQ(r, 0);
}

TEST(basemap_shm_util, DeleteMemory) {
  int r = DeleteMemoryPtr(&shared);
  EXPECT_EQ(r, 0);
  std::cout << "deletememory shmid" << std::endl;
}
TEST(basemap_shm_util, FreeMemory) {
  int r = FreeMemory(shmid);
  EXPECT_EQ(r, 0);
  std::cout << "freememory shmid: " << shmid << std::endl;
}*/
/*
TEST(ShmParser, all) {
  ShmParser sp(5, 1024, 0.05,
               "/home/idriver/work/AVOS20/src/map/basemap/map/tile_map/Tile_");
  int r = sp.CreatSharedMemory();
  EXPECT_GE(r, 0);
  r = sp.NewSharedMemoryPtr();
  EXPECT_EQ(r, 0);
  r = sp.UpdateShm(440780.80000000005, 4424448.0);
  std::cout<<"update:"<<r<<std::endl;

  //PointGCCS car = {443576.3666, 4436622.53, 0.0};
  PointGCCS car = {440780.80000000005, 4424448.0, 0.0};
  PointVCS temp;
  temp.x = 0;
  temp.y = 0;
  temp.angle = 0;
  //uint ss = sp.GetCellBGRA(car, temp);
  uint ss = sp.GetCellBGRA(car, GetVCS(200, 200));
  for (int i = 0; i < 4; i++) {
    std::cout << (int)(*((unsigned char*)&ss+i)) << std::endl;
  }
  std::cout<<"read:"<<r<<std::endl;
  avos::map::PNGParser png_parser_;
  avos::map::eCellInfo cell;
  for (int i = 0; i < 1; i++) {
      cell = png_parser_.GetObject(ss);

  }
  EXPECT_EQ(static_cast<unsigned int>(cell), 13);
  //EXPECT_EQ(static_cast<unsigned int>(cell), 4);
  r = sp.DeleteSharedMemoryPtr();
  EXPECT_EQ(r, 0);
  r = sp.FreeSharedMemory();
  EXPECT_EQ(r, 0);
}
*/
TEST(BaseMap, CreatSharedMemory) {
  BaseMap *base_ = MapLoader::GetBasemapPtr();
  auto r = 0;
  r = base_->CreatSharedMemory();
  EXPECT_GE(r, 0);
  r = base_->NewSharedMemoryPtr();
  // base_->UpdateShm(440780.80000000005, 4424448.0);
  EXPECT_EQ(r, 0);
}
TEST(BaseMap, GetBasemapPtr) {
  PointGCCS car = {440780.80000000005, 4424448.0, 0.0};
  BaseMap *basemap_ = MapLoader::GetBasemapPtr();
  eCellInfo cell;
  int8_t au = 1;
  int num = 0;
  for (int j = 0; j < 1000; j++) {
    for (int i = 0; i < 100000; i++) {
      cell = basemap_->GetCellInfo(car, GetVCS(200, 200));
      if (static_cast<unsigned int>(cell) != 13) {
        num++;
      }
    }
    std::cout << "not read:" << num << std::endl;
    num = 0;
    //usleep(50);
    // EXPECT_EQ(static_cast<unsigned int>(cell), 13);
  }
}
TEST(BaseMap, FreeSharedMemory) {
  BaseMap *base = MapLoader::GetBasemapPtr();
  // auto r = base->DeleteSharedMemoryPtr();
  // EXPECT_EQ(r, 0);
  // r = base->FreeSharedMemory();
  // EXPECT_EQ(r, 0);
}
int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}