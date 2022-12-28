#include "SensorItem.h"

QString sensorText(Amass::Sensor::Type type) {
    using namespace Amass;
    switch (type) {
    case Sensor::Type::Lidar:
        return "激光雷达";
    case Sensor::Type::MillRadarFrontLeft:
        return "前左毫米波雷达";
    case Sensor::Type::MillRadarFrontMiddle:
        return "前中毫米波雷达";
    case Sensor::Type::MillRadarFrontRight:
        return "前右毫米波雷达";
    case Sensor::Type::MillRadarBackLeft:
        return "后左毫米波雷达";
    case Sensor::Type::MillRadarBackRight:
        return "后右毫米波雷达";
    case Sensor::Type::CameraFront:
        return "前摄像头";
    case Sensor::Type::CameraBack:
        return "后摄像头";
    case Sensor::Type::CameraLeft:
        return "左摄像头";
    case Sensor::Type::CameraRight:
        return "右摄像头";
    case Sensor::Type::CameraHD:
        return "高清摄像头";
    case Sensor::Type::Gnss:
        return "组合惯导";
    default:
        break;
    }
    return "未知";
    /*

    enum class Type {
        Lidar,
        MillRadarFrontLeft,
        MillRadarFrontMiddle,
        MillRadarFrontRight,
        MillRadarBackLeft,
        MillRadarBackRight,
        CameraFront,
        CameraBack,
        CameraLeft,
        CameraRight,
        CameraHD,
        Gnss
    };
*/
}

QString statusText(Amass::Status status) {
    switch (status) {
    case Amass::Status::Fatal:
        return "故障";
    case Amass::Status::Good:
        return "正常";
    case Amass::Status::NotWorked:
        return "未知";
    default:
        return "未知";
    }
    return "未知";
}

QString nboostStateText(int state) {
    // 0:No fault 0x1:Minor fault 0x2:General fault 0x3:Major fault 0x4:Serious fault 0x5:Especially serious fault
    QString txt;
    switch (state) {
    case 0x0: {
        txt = "工作正常";
        break;
    }
    case 0x1: {
        txt = "轻微故障";
        break;
    }
    case 0x2: {
        txt = "常见故障";
        break;
    }
    case 0x3: {
        txt = "主要故障";
        break;
    }
    case 0x4: {
        txt = "严重故障";
        break;
    }
    case 0x5: {
        txt = "特级严重故障";
        break;
    }
    default: {
        txt = "未知";
        break;
    }
    }

    return txt;
}

QString epsStateText(int state) {
    // 0：停止工作状态Stop;0x10：手动控制状态Manual control;0x20: 自动控制状态Auto control;0x55：错误状态Error
    QString txt;
    switch (state) {
    case 0: {
        txt = "工作正常";
        break;
    }
    case 1: {
        txt = "EPS故障";
        break;
    }
    default: {
        txt = "未知";
        break;
    }
    }
    return txt;
}

QString vcuStateText(int state) {
    static QString txt;
    switch (state) {
    case 0: {
        txt = "工作正常";
        break;
    }
    case 0x10: {
        txt = "手动控制";
        break;
    }
    case 0x20: {
        txt = "自动控制";
        break;
    }
    case 0x55: {
        txt = "错误状态";
        break;
    }
    default: {
        txt = "未知";
        break;
    }
    }

    return txt;
}
