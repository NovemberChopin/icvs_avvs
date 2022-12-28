#include "Configuration.h"
#include <QDebug>
#include <sstream>

QString Configuration::sensorParameterFile = "";

void Configuration::initPath() {
    std::ostringstream oss;
    oss << getenv("HOME") << "/sensor_parameter.ini";
    sensorParameterFile = QString::fromStdString(oss.str());
    qDebug() << "parameter path: " << sensorParameterFile;
}
