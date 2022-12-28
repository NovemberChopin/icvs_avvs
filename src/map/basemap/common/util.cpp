#include "util.h"
#include <dirent.h>
#include <string.h>
namespace avos {
namespace map {
namespace common {
std::string IntoString() { return ""; }
bool FindAllFiles(std::string directory_path, std::string suffix,
                  std::vector<std::string> &names) {
  names.clear();
  DIR *directory = opendir(directory_path.c_str());
  if (directory == nullptr) {
    std::cout << "Cannot open directory " << directory_path;
    return false;
  }
  struct dirent *file;
  while ((file = readdir(directory)) != nullptr) {
    if (strstr(file->d_name, suffix.c_str()) != NULL) {
      names.push_back(file->d_name);
    }
  }
  closedir(directory);
  return true;
}
std::vector<std::string> FindAllFiles(std::string directory_path,
                                      std::string suffix) {
  std::vector<std::string> re;
  FindAllFiles(directory_path, suffix, re);
  return re;
}
std::vector<std::string> ComposePCDNames(int size, int row, int col, int level,
                                         std::string suffix, std::string prefix,
                                         std::string joint) {
  std::vector<std::string> re;
  std::stringstream name;
  int n = size / 2;
  for (int i = -n; i < n + 1; ++i)
    for (int j = -n; j < n + 1; ++j) {
      name.str("");
      name << prefix << joint << row + i << joint << col + j << joint << level
           << suffix;
      re.push_back(name.str());
    }
  return re;
}

void Lonlat2UTM(double &lon, double &lat) {
  double height = 1.;
  std::string utmInit = "+proj=utm +zone=" + std::to_string(GetLongZone(lon)) +
                        "+ellps=WGS84 +datum=WGS84 +units=m +no_defs";
  projPJ lonlat = pj_init_plus(" +proj=longlat +datum=WGS84 +no_defs");
  projPJ utm = pj_init_plus(utmInit.c_str());
  lon *= DEG_TO_RAD;
  lat *= DEG_TO_RAD;
  pj_transform(lonlat, utm, 1, 1, &lon, &lat, &height);
  ;
}
void UTM2Lonlat(double &x, double &y, int zone) {
  double z = 1.;
  std::string utmInit = "+proj=utm +zone=" + std::to_string(zone) +
                        "+ellps=WGS84 +datum=WGS84 +units=m +no_defs";
  projPJ lonlat = pj_init_plus(" +proj=longlat +datum=WGS84 +no_defs");
  projPJ UTM = pj_init_plus(utmInit.c_str());
  int ret = pj_transform(UTM, lonlat, 1, 1, &x, &y, &z);
  x *= RAD_TO_DEG;
  y *= RAD_TO_DEG;
}
}  // namespace common
}  // namespace map
}  // namespace AVOS