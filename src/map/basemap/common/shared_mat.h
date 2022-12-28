/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: basemap
* FileName: 
*
* Description: shared memory util. 
WARNING : dont include this file more than twice, that will give you error, mutidefiniton. 
        you can divide the file to h and cc.

*
* History:
* lbh         2018/07/07    1.0.0    build this module.
******************************************************************************/
#ifndef SHARED_MAT_H_
#define SHARED_MAT_H_

#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fstream>
#include <opencv2/opencv.hpp>

#define KEYT 11111

int CreatMemory(std::string filename, uint size_char) {
  int shmid;
  void* shared;
  shmid = shmget((key_t)KEYT, size_char, 0666 | IPC_CREAT);
  if (shmid == -1) return -1;
  shared = shmat(shmid, 0, 0);
  if (shared == (void*)-1) return -2;
  std::ifstream infile;
  infile.open(filename.c_str(), std::ios::binary);
  if (!infile.is_open()) return -3;
  infile.read((char*)shared, size_char);
  if (infile.bad()) return -4;
  infile.close();
  if (shmdt(shared) == -1) return -5;
  return shmid;
}

int NewMemoryPtr(void** shared, int shmid) {
  void* sh;
  sh = shmat(shmid, 0, 0);
  if (sh == (void*)-1) return -1;
  *shared = sh;
  return 0;
}

int NewMemoryPtr(void** shared, uint size_char){
  int shmid;
  void* sh;
  shmid = shmget((key_t)KEYT, 0, 0);
  if (shmid == -1) return -1;
  sh = shmat(shmid, 0, 0);
  if (sh == (void*)-1) return -2;
  *shared = sh;
  return shmid;
}

int DeleteMemoryPtr(void** shared) {
  if (shmdt(*shared) == -1) return -1;
  return 0;
}

int FreeMemory(int shmid) {
  if (shmctl(shmid, IPC_RMID, 0) == -1) return -1;
  return 0;
}

bool BinaryFileToMat(std::string filename, cv::Mat& img) {
  std::ifstream infile;
  infile.open(filename.c_str(), std::ios::binary);
  if (!infile.is_open()) return -1;
  for (int r = 0; r < img.rows; r++) {
    infile.read(reinterpret_cast<char*>(img.ptr(r)), img.cols * img.elemSize());
    if (infile.bad()) return -1;
  }
  infile.close();
  return 0;
}
bool MatToBinaryFile(cv::Mat& image, std::string filename) {
  std::ofstream outfile;
  outfile.open(filename.c_str(), std::ios::binary);
  if (!outfile.is_open()) return -1;
  for (int r = 0; r < image.rows; r++) {
    outfile.write(reinterpret_cast<const char*>(image.ptr(r)),
                  image.cols * image.elemSize());
    if (outfile.bad()) return -1;
  }
  return 0;
}

#endif