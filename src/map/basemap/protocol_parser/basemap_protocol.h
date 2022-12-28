/******************************************************************************
* Copyright (C) 2015-2020, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: basemap
* FileName:
*
* Description: protocol

*
* History:
* lbh         2018/05/22    1.0.0    build this module.
******************************************************************************/

#ifndef BASEMAP_PROTOCOL_H_
#define BASEMAP_PROTOCOL_H_
namespace avos {
namespace map {

typedef unsigned char uchar;
enum class eCellInfo : uchar {
  UNKNOWN = 0,                 // 205,205,205,127
  SOCIAL_UNDEFINE = 1,         // 100,250,255,127
  SOCIAL_SIDEWALK = 2,         // 100,200,255,127
  SOCIAL_BIKEWAY = 3,          // 100,150,255,127
  SOCIAL_MOTORWAY = 4,         // 100,100,255,127
  SOCIAL_CROSSWAY = 5,         // 100,50,255,127
  INSIDE_UNDEFINE = 6,         // 150,250,255,127
  INSIDE_SIDEWALK = 7,         // 150,200,255,127
  INSIDE_BIKEWAY = 8,          // 150,150,255,127
  INSIDE_MOTORWAY = 9,         // 150,100,255,127
  INSIDE_CROSSWAY = 10,        // 150,50,255,127
  BOUNDRY = 11,                // 0,0,255,127
  WAY_END = 12,                // 200,150,250,127
  PARKING_ZONE = 13,           // 200,200,245,127
  SPEED_BUMP = 14,             // 150,150,245,127
  TREE_WELL = 15,              // 255,0,0,127
  SHELTER_PILE = 16,           // 250,100,0,127
  BLINDWAY = 17,               // 245,200,0,127
  TRAFFICLIGHT = 18,           // 0,255,0,127
  CROSS_SIGNAL = 19,           // 150,250,50,127
  SPEED_LIMIT_SIGNAL = 20,     // 200,250,50,127
  D_SPEED_LIMIT_SIGNAL = 21,   // 200,250,100,127
  NO_PEDESTRIAN_SIGNAL = 22,   // 50,250,150,127
  NO_NONEVEHICLE_SIGNAL = 23,  // 200,250,0,127
  STRAIGHT_SIGNAL = 24,        // 200,250,150,127
  TURNLEFT_SIGNAL = 25,        // 200,250,200,127
  TURNRIGHT_SIGNAL = 26,       // 200,250,250,127
  NO_STRAIGHT_SIGNAL = 27,     // 100,250,0,127
  NO_TURNLEFT_SIGNAL = 28,     // 100,250,50,127
  NO_TURNRIGHT_SIGNAL = 29,    // 100,250,100,127
};
static const uchar JD[][5] = {
    {205, 205, 205, 127, 0},  {100, 250, 255, 127, 1},
    {100, 200, 255, 127, 2},  {100, 150, 255, 127, 3},
    {100, 100, 255, 127, 4},  {100, 50, 255, 127, 5},
    {150, 250, 255, 127, 6},  {150, 200, 255, 127, 7},
    {150, 150, 255, 127, 8},  {150, 100, 255, 127, 9},
    {150, 50, 255, 127, 10},  {0, 0, 255, 127, 11},
    {200, 150, 250, 127, 12}, {200, 200, 245, 127, 13},
    {150, 150, 245, 127, 14}, {255, 0, 0, 127, 15},
    {250, 100, 0, 127, 16},   {245, 200, 0, 127, 17},
    {0, 255, 0, 127, 18},     {150, 250, 50, 127, 19},
    {200, 250, 50, 127, 20},  {200, 250, 100, 127, 21},
    {50, 250, 150, 127, 22},  {200, 250, 0, 127, 23},
    {200, 250, 150, 127, 24}, {200, 250, 200, 127, 25},
    {200, 250, 250, 127, 26}, {100, 250, 0, 127, 27},
    {100, 250, 50, 127, 28},  {100, 250, 100, 127, 29}};
static const uchar IDP[][5] = {
    {205, 205, 205, 127, 0},  {100, 250, 255, 127, 1},
    {100, 200, 255, 127, 2},  {100, 150, 255, 127, 3},
    {100, 100, 255, 127, 4},  {100, 50, 255, 127, 5},
    {150, 250, 255, 127, 6},  {150, 200, 255, 127, 7},
    {150, 150, 255, 127, 8},  {150, 100, 255, 127, 9},
    {150, 50, 255, 127, 10},  {0, 0, 255, 127, 11},
    {200, 150, 250, 127, 12}, {200, 200, 245, 127, 13},
    {150, 150, 245, 127, 14}, {255, 0, 0, 127, 15},
    {250, 100, 0, 127, 16},   {245, 200, 0, 127, 17},
    {0, 255, 0, 127, 18},     {150, 250, 50, 127, 19},
    {200, 250, 50, 127, 20},  {200, 250, 100, 127, 21},
    {50, 250, 150, 127, 22},  {200, 250, 0, 127, 23},
    {200, 250, 150, 127, 24}, {200, 250, 200, 127, 25},
    {200, 250, 250, 127, 26}, {100, 250, 0, 127, 27},
    {100, 250, 50, 127, 28},  {100, 250, 100, 127, 29}};

}  // namespace map
}  // namespace avos
#endif