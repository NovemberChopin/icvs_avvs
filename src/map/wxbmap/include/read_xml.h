/******************************************************************************
* Copyright (C) 2018-2023, idriverplus(BeiJing ZhiXingZhe, Inc.)
*
* NodeName: ivpathplan
* FileName: refpath.h
*
* Description: reference path
*
* History:
* Feng younan          2018/08/21          v1.0.0        Build this model.
******************************************************************************/
#ifndef __IVPLANNER_READ_XML_H__
#define __IVPLANNER_READ_XML_H__

// #include <ros/ros.h>
#include "map/common/macro.h"
#include "typedef.h"
#include <gtest/gtest.h>
#include "map/map_loader/include/map_loader.h"


using avos::vectormap::GeoTool;
using avos::vectormap::PointVCS;
using avos::vectormap::PointGCCS;
using avos::vectormap::PointGPS;

namespace avos     {
namespace wxbmap {

class ReadXml {
  DECLARE_SINGLETON(ReadXml);

public:
  MapInfo* GetMapInfo(std::string area_id);
  bool ReadXmlMap();
private:

private:

private:
  std::map<std::string, MapInfo*> xml_data_;

};

}
}
#endif  // __IVPLANNER_READ_XML_H__
