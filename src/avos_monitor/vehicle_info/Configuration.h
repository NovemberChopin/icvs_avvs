#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QString>

class Configuration {
public:
    static void initPath();
    static QString sensorParameterFile; // = "/home/optimus/Projects/sensor_parameter.ini";
};
#endif // CONFIGURATION_H
