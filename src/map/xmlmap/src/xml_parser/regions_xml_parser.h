/*
idriverplus
create 20181122 lbh
modify 20190613 pgq
=========================================================================*/

#ifndef AVOS1_XMLMAP_XML_PARSER_REGIONS_H_
#define AVOS1_XMLMAP_XML_PARSER_REGIONS_H_

#include "tinyxml2.h"
#include "util_xml_parser.h"

namespace avos {
namespace xmlmap {
namespace adapter {

class RegionsXmlParser {
 public:
  static int Parse(const tinyxml2::XMLElement& xml_node, Regions* regions);
};

}  // namespace adapter
}  // namespace xmlmap
}  // namespace avos

#endif
