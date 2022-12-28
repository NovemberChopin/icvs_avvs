#ifndef SENSORITEM_H
#define SENSORITEM_H

#include <QPixmap>
#include <QVector3D>

namespace Amass {

enum class Status : int {
    NotWorked,
    Good,
    Fatal,
};

struct Sensor {

    enum class Type : int {
        Lidar,
        MillRadarFrontLeft,
        MillRadarFrontMiddle,
        MillRadarFrontRight,
        MillRadarBackLeft,
        MillRadarBackRight,
        CameraHD,
        CameraFront,
        CameraBack,
        CameraLeft,
        CameraRight,
        Gnss
    };
    Status status{Status::NotWorked};
    int64_t timestamp{0};
    QVector3D position;
    float yaw{0}, pitch{0}, roll{0};
    Type type;
};

struct Chassis {
    enum class Type : int {
        EPS,
        VCU,
        NBOOST,
    };
    Type type;
    int workState; //拿到的状态码
    Status dataState;
    size_t timestamp;
};
} // namespace Amass

class SensorItem {
public:
    // - 激光雷达x1
    // - 毫米波雷达x5(前左，前中，前右，后左，后右)
    // - 相机x5(前，后，左，右，高清)
    // - 组合惯导x1
    // - 超声波雷达x12
    Amass::Sensor data;
    QPixmap okPixmap;
    QPixmap fatalPixmap;
};

enum class TopicType {
    Lidar,
    MillRadar,
    Camera,
    Gnss,
};

class CpuStatus {
public:
    Amass::Status perception{Amass::Status::NotWorked};
    Amass::Status localization{Amass::Status::NotWorked};
    Amass::Status decision{Amass::Status::NotWorked};
    Amass::Status control{Amass::Status::NotWorked};
    Amass::Status navigation{Amass::Status::NotWorked};
    size_t timestamp{0};
};

QString sensorText(Amass::Sensor::Type type);
QString statusText(Amass::Status status);
QString nboostStateText(int state);
QString epsStateText(int state);
QString vcuStateText(int state);

#endif // SENSORITEM_H
