#ifndef __SHOW_VECTOR_MAP_H__
#define __SHOW_VECTOR_MAP_H__

#include <vector>
#include <string>
#include <map>
typedef struct ShowMapPoint
{
	int id;
	double lon;
	double lat;
	double x;
	double y;

} ShowMapPoint;


class ShowVectorMapBase
{
public:
	ShowVectorMapBase() {
		vector_map_points_.clear();
	}
	~ShowVectorMapBase() {

	}


	virtual bool LoadVectorMap(const std::string &path) = 0;
	virtual bool SetLocPos(double xg, double yg, double angle) = 0;


public:
	std::map<std::string, std::vector<ShowMapPoint>> vector_map_points_;

};




















#endif