#include "Backend.h"
#include "AsyncEvent.h"
#include "Configuration.h"
#include "include/data_pool.h"
#include <QDebug>
#include <QPainter>
#include <QSettings>
#include <QTimer>
#include <random>

inline static int64_t currentTime() {
    using namespace std::chrono;
    std::default_random_engine randomEngine;
    randomEngine.seed(system_clock::now().time_since_epoch().count());
    return time_point_cast<milliseconds>(system_clock::now()).time_since_epoch().count() - randomEngine() % 1000;
}

static Amass::Status translate(int status) {
    if (status == -1) {
        return Amass::Status::NotWorked;
    } else if (status == 0) {
        return Amass::Status::Fatal;
    } else if (status == 1) {
        return Amass::Status::Good;
    }
    return Amass::Status::NotWorked;
}

QSharedPointer<Backend> Backend::m_instance{nullptr};

Backend *Backend::instance() {
    if (!m_instance) m_instance = QSharedPointer<Backend>(new Backend());
    return m_instance.data();
}

Backend::Sensors Backend::sensors() const { return m_sensors; }

void Backend::updateSensor(const Amass::Sensor &sensor) {
    executeAtObjectThread<ForceAsync>(this, [this, sensor]() {
        auto item = m_sensors.find(sensor.type);
        if (item == m_sensors.cend()) return;
        item->second.data = sensor;
        emit sensorDataChanged(sensor);
    });
}

void Backend::updateChassis(const Amass::Chassis &chassis) {
    executeAtObjectThread<ForceAsync>(this, [this, chassis]() {
        auto iterator = m_chassis.find(chassis.type);
        if (iterator == m_chassis.cend()) return;
        iterator->second = chassis;
        emit chassisDataChanged(chassis);
    });
}

void Backend::updateTopic(TopicType type, const QString &topic) {
    executeAtObjectThread<ForceAsync>(this, [this, type, topic]() { emit incommingTopic(type, topic); });
}

Backend::Backend(QObject *parent) : QObject(parent) {
    QTimer::singleShot(1, this, [this]() {
        initializeChassis();
        initializeSensorItem();
        emit initialized();
        startTimer(1000);
    });
    Configuration::initPath();
}

void Backend::initializeSensorItem() {
    using namespace Amass;
    SensorItem item;

    {
        QPixmap fatalpixmap(120, 120);
        fatalpixmap.fill(Qt::transparent);
        QPainter painter(&fatalpixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(Qt::red);
        painter.drawEllipse(0, 0, 120, 120);
        item.fatalPixmap = fatalpixmap;
    }
    {
        QPixmap okpixmap(120, 120);
        okpixmap.fill(Qt::transparent);
        QPainter painter(&okpixmap);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBrush(Qt::green);
        painter.drawEllipse(0, 0, 120, 120);

        item.okPixmap = okpixmap;
    }

    item.data.type = Sensor::Type::CameraBack; // 后摄像头
    item.data.position = QVector3D(-410, 0, 1175);
    item.data.status = Status::Good;
    m_sensors.emplace(Sensor::Type::CameraBack, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::CameraFront; //前摄像头
    item.data.position = QVector3D(1680, 0, 1720);
    item.data.status = Status::Fatal;
    m_sensors.emplace(Sensor::Type::CameraFront, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::CameraHD; //高清摄像头
    item.data.position = QVector3D(3113, 0, 1175);
    item.data.status = Status::Good;
    m_sensors.emplace(Sensor::Type::CameraHD, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::CameraLeft; //左摄像头
    item.data.position = QVector3D(1020, 840, 1720);
    item.data.status = Status::Fatal;
    m_sensors.emplace(Sensor::Type::CameraLeft, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::CameraRight; //右摄像头
    item.data.position = QVector3D(1020, -840, 1720);
    item.data.status = Status::Good;
    m_sensors.emplace(Sensor::Type::CameraRight, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::Gnss; //组合惯导
    item.data.position = QVector3D(-9.348, 0, 1130);
    m_sensors.emplace(Sensor::Type::Gnss, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::Lidar; //激光雷达
    item.data.position = QVector3D(1010, 0, 1995);
    m_sensors.emplace(Sensor::Type::Lidar, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::MillRadarBackLeft; //后左毫米波雷达
    item.data.position = QVector3D(-452.27, -821.38, 720);
    m_sensors.emplace(Sensor::Type::MillRadarBackLeft, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::MillRadarBackRight; // 后右毫米波雷达
    item.data.position = QVector3D(-452.27, 821.38, 720);
    m_sensors.emplace(Sensor::Type::MillRadarBackRight, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::MillRadarFrontLeft; //前左毫米波雷达
    item.data.position = QVector3D(3400, -821.38, 720);
    m_sensors.emplace(Sensor::Type::MillRadarFrontLeft, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::MillRadarFrontMiddle; //前中毫米波雷达
    item.data.position = QVector3D(3400, 0, 720);
    m_sensors.emplace(Sensor::Type::MillRadarFrontMiddle, item);
    emit sensorDataChanged(item.data);

    item.data.type = Sensor::Type::MillRadarFrontRight; //前右毫米波雷达
    item.data.position = QVector3D(3400, 821.38, 720);
    m_sensors.emplace(Sensor::Type::MillRadarFrontRight, item);
    emit sensorDataChanged(item.data);
}

void Backend::initializeChassis() {
    // std::vector<Amass::Chassis> m_chassis;
    Amass::Chassis chassis;

    chassis.dataState = Amass::Status::NotWorked;
    chassis.timestamp = 3434353545;
    chassis.type = Amass::Chassis::Type::EPS;
    chassis.workState = 0;
    m_chassis.emplace(Amass::Chassis::Type::EPS, chassis);
    emit chassisDataChanged(chassis);

    chassis.dataState = Amass::Status::NotWorked;
    chassis.timestamp = 3434353545;
    chassis.type = Amass::Chassis::Type::VCU;
    chassis.workState = 0;
    m_chassis.emplace(Amass::Chassis::Type::VCU, chassis);
    emit chassisDataChanged(chassis);

    chassis.dataState = Amass::Status::NotWorked;
    chassis.timestamp = 3434353545;
    chassis.type = Amass::Chassis::Type::NBOOST;
    chassis.workState = 0;
    m_chassis.emplace(Amass::Chassis::Type::NBOOST, chassis);
    emit chassisDataChanged(chassis);
}

void Backend::timerEvent(QTimerEvent * /*event*/) {
    std::default_random_engine random;
    // -1: 未知
    // 0: 异常
    // 1:正常

    /*
    todo:
        1. 读取sensor_parameter.ini 配置文件，里面保存着 标定工具 设置的传感器的位置参数
        2. 读取 g_common_info_ 的 传感器状态信息。
    */
    QSettings settings(Configuration::sensorParameterFile, QSettings::IniFormat);
    {
        settings.beginGroup("lidar");

        auto x = settings.value("x", 0).toDouble();
        auto y = settings.value("y", 0).toDouble();
        auto z = settings.value("z", 0).toDouble();

        auto yaw = settings.value("yaw", 0).toDouble();
        auto roll = settings.value("roll", 0).toDouble();
        auto pitch = settings.value("pitch", 0).toDouble();

        settings.endGroup();

        auto &lidar = m_sensors[Amass::Sensor::Type::Lidar];
        lidar.data.status = translate(g_common_info_.sensor_status.lidar_status);

        lidar.data.position.setX(x);
        lidar.data.position.setY(y);
        lidar.data.position.setZ(z);
        lidar.data.yaw = yaw;
        lidar.data.pitch = pitch;
        lidar.data.roll = roll;
        lidar.data.timestamp = currentTime();
        emit sensorDataChanged(lidar.data);
    }
    { // MillRadarFrontLeft
        auto &millRadar = m_sensors[Amass::Sensor::Type::MillRadarFrontLeft];
        millRadar.data.status = translate(g_common_info_.sensor_status.mill_radar[0]);
        millRadar.data.timestamp = currentTime();
        emit sensorDataChanged(millRadar.data);
    }
    { // MillRadarFrontMiddle
        settings.beginGroup("mill_radar");

        auto x = settings.value("x", 0).toDouble();
        auto y = settings.value("y", 0).toDouble();
        auto z = settings.value("z", 0).toDouble();

        auto yaw = settings.value("yaw", 0).toDouble();
        auto roll = settings.value("roll", 0).toDouble();
        auto pitch = settings.value("pitch", 0).toDouble();

        settings.endGroup();

        auto &millRadar = m_sensors[Amass::Sensor::Type::MillRadarFrontMiddle];
        millRadar.data.status = translate(g_common_info_.sensor_status.mill_radar[1]);

        millRadar.data.position.setX(x);
        millRadar.data.position.setY(y);
        millRadar.data.position.setZ(z);
        millRadar.data.yaw = yaw;
        millRadar.data.pitch = pitch;
        millRadar.data.roll = roll;
        millRadar.data.timestamp = currentTime();
        emit sensorDataChanged(millRadar.data);
    }
    { // MillRadarFrontRight
        auto &millRadar = m_sensors[Amass::Sensor::Type::MillRadarFrontRight];
        millRadar.data.status = translate(g_common_info_.sensor_status.mill_radar[2]);
        millRadar.data.timestamp = currentTime();
        emit sensorDataChanged(millRadar.data);
    }
    { // MillRadarBackLeft
        auto &millRadar = m_sensors[Amass::Sensor::Type::MillRadarBackLeft];
        millRadar.data.status = translate(g_common_info_.sensor_status.mill_radar[3]);
        millRadar.data.timestamp = currentTime();
        emit sensorDataChanged(millRadar.data);
    }
    { // MillRadarBackRight
        auto &millRadar = m_sensors[Amass::Sensor::Type::MillRadarBackRight];
        millRadar.data.status = translate(g_common_info_.sensor_status.mill_radar[4]);
        millRadar.data.timestamp = currentTime();
        emit sensorDataChanged(millRadar.data);
    }
    { // CameraHD
        settings.beginGroup("camera");

        auto x = settings.value("x", 0).toDouble();
        auto y = settings.value("y", 0).toDouble();
        auto z = settings.value("z", 0).toDouble();

        auto yaw = settings.value("yaw", 0).toDouble();
        auto roll = settings.value("roll", 0).toDouble();
        auto pitch = settings.value("pitch", 0).toDouble();

        settings.endGroup();

        auto &camera = m_sensors[Amass::Sensor::Type::CameraHD];
        camera.data.status = translate(g_common_info_.sensor_status.camera_status[0]);

        camera.data.position.setX(x);
        camera.data.position.setY(y);
        camera.data.position.setZ(z);
        camera.data.yaw = yaw;
        camera.data.pitch = pitch;
        camera.data.roll = roll;
        camera.data.timestamp = currentTime();
        emit sensorDataChanged(camera.data);
    }
    { // CameraFront
        auto &camera = m_sensors[Amass::Sensor::Type::CameraFront];
        camera.data.status = translate(g_common_info_.sensor_status.camera_status[1]);
        camera.data.timestamp = currentTime();
        emit sensorDataChanged(camera.data);
    }
    { // CameraLeft
        auto &camera = m_sensors[Amass::Sensor::Type::CameraLeft];
        camera.data.status = translate(g_common_info_.sensor_status.camera_status[2]);
        camera.data.timestamp = currentTime();
        emit sensorDataChanged(camera.data);
    }
    { // CameraBack
        auto &camera = m_sensors[Amass::Sensor::Type::CameraBack];
        camera.data.status = translate(g_common_info_.sensor_status.camera_status[3]);
        camera.data.timestamp = currentTime();
        emit sensorDataChanged(camera.data);
    }
    { // CameraRight
        auto &camera = m_sensors[Amass::Sensor::Type::CameraRight];
        camera.data.status = translate(g_common_info_.sensor_status.camera_status[4]);
        camera.data.timestamp = currentTime();
        emit sensorDataChanged(camera.data);
    }
    { // Gnss
        settings.beginGroup("gnss");
        auto x = settings.value("x", 0).toDouble();
        auto y = settings.value("y", 0).toDouble();
        auto z = settings.value("z", 0).toDouble();
        auto pitch = settings.value("atc_pitch", 0).toDouble();
        auto roll = settings.value("atc_roll", 0).toDouble();
        auto yaw = settings.value("atc_yaw", 0).toDouble();
        settings.endGroup();

        auto &gnss = m_sensors[Amass::Sensor::Type::Gnss];
        gnss.data.status = translate(g_common_info_.sensor_status.nav_status);
        gnss.data.timestamp = currentTime();

        gnss.data.position.setX(x);
        gnss.data.position.setY(y);
        gnss.data.position.setZ(z);
        gnss.data.roll = roll;
        gnss.data.yaw = yaw;
        gnss.data.pitch = pitch;
        emit sensorDataChanged(gnss.data);
    }
    { // VCU

        auto &vcu = m_chassis[Amass::Chassis::Type::VCU];
        vcu.workState = g_common_info_.sensor_status.vcu_status;
        vcu.timestamp = currentTime();
        emit chassisDataChanged(vcu);
    }
    { // EPS

        auto &eps = m_chassis[Amass::Chassis::Type::EPS];
        eps.workState = g_common_info_.sensor_status.eps_status;
        eps.timestamp = currentTime();
        emit chassisDataChanged(eps);
    }
    { // NBOOST
        auto &nboost = m_chassis[Amass::Chassis::Type::NBOOST];
        nboost.workState = g_common_info_.sensor_status.nboost_status;
        nboost.timestamp = currentTime();
        emit chassisDataChanged(nboost);
    }
    { //计算单元信息0:未启动；１：已启动
        CpuStatus status;
        status.control = translate(g_common_info_.autodriver_status.control);
        status.decision = translate(g_common_info_.autodriver_status.planning);
        status.localization = translate(g_common_info_.autodriver_status.localization);
        status.navigation = translate(g_common_info_.autodriver_status.navigation);
        status.perception = translate(g_common_info_.autodriver_status.perception);
        status.timestamp = currentTime();
        emit cpuDataChanged(status);
    }
    //    auto topic = "\n---\n"
    //                 "x: 4.72991943359375\n"
    //                 "y: 3.3476438522338867\n"
    //                 "theta: -2.4111852645874023\n"
    //                 "linear_velocity: 0.0\n"
    //                 "angular_velocity: 0.0\n"
    //                 "\n---\n"
    //                 "x: 4.72991943359375\n"
    //                 "y: 3.3476438522338867\n"
    //                 "theta: -2.4111852645874023\n"
    //                 "linear_velocity: 0.0\n"
    //                 "angular_velocity: 0.0\n"
    //                 "\n---\n"
    //                 "x: 4.72991943359375\n"
    //                 "y: 3.3476438522338867\n"
    //                 "theta: -2.4111852645874023\n"
    //                 "linear_velocity: 0.0\n"
    //                 "angular_velocity: 0.0\n"
    //                 "\n---\n";
    //    updateTopic(TopicType::Camera, topic);
    //    updateTopic(TopicType::Lidar, topic);
    //    updateTopic(TopicType::Gnss, topic);
    //    updateTopic(TopicType::MillRadar, topic);
}
