#ifndef __SHOW_WXB_VECTOR_MAP_H__
#define __SHOW_WXB_VECTOR_MAP_H__


#include <iostream>
#include "show_vector_map.h"
#include "tinyxml2.h"


using namespace tinyxml2;


class ShowWXBVectorMap: public ShowVectorMapBase
{
public:
	ShowWXBVectorMap() {
		vector_map_points_.clear();
		points_belong = " ";
	}
	~ShowWXBVectorMap() {

	}

public:
	bool LoadVectorMap(const std::string &path) override {
		XMLDocument doc;
		if (doc.LoadFile(path.c_str()) != 0)
		{
			std::cout << "load xml file failed " << path.c_str() << std::endl;
			return false;
		}
		XMLElement* root = doc.RootElement();
		ParseSubNode(root);
		std::cout << "size: " << vector_map_points_.size() << std::endl;
		return true;
	}


	bool SetLocPos(double xg, double yg, double angle) override {

	}



private:
	void ParseSubNode(tinyxml2::XMLElement *xml_node) {
		auto node = xml_node->FirstChildElement();

		if (node) {
			std::string node_value =  node->Value();
			if (node_value == "boundaries") {
				points_belong = "boundaries";
			}
			if (node_value == "roads") {
				points_belong = "roads";
			}
			if (node_value == "surfaces") {
				points_belong = "surfaces";
			}
			if (node_value == "regions") {
				points_belong = "regions";
			}

			if (node_value == "point" ) {
				ShowMapPoint point;
				point.x = strtod(node->Attribute("x"), nullptr);
				point.y = strtod(node->Attribute("y"), nullptr);
				vector_map_points_[points_belong].push_back(point);
			}

			ParseSubNode(node);
		}

		node = xml_node->NextSiblingElement();
		if (node) {
			std::string node_value =  node->Value();


			if (node_value == "boundaries") {
				points_belong = "boundaries";
			}
			if (node_value == "roads") {
				points_belong = "roads";
			}
			if (node_value == "surfaces") {
				points_belong = "surfaces";
			}
			if (node_value == "regions") {
				points_belong = "regions";
			}

			if (node_value == "point" ) {
				ShowMapPoint point;
				point.x = strtod(node->Attribute("x"), nullptr);
				point.y = strtod(node->Attribute("y"), nullptr);
				vector_map_points_[points_belong].push_back(point);
			}
			ParseSubNode(node);
		}
	}


private:
	std::string points_belong;


};














#endif