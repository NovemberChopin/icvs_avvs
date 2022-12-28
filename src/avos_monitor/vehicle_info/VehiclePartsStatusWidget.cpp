#include "VehiclePartsStatusWidget.h"
#include "Backend.h"
#include "ChassisInformationGroupBox.h"
#include "CpuInformationGroupBox.h"
#include "SensorInformationGroupBox.h"
#include "SingleSensorInformationGroupBox.h"
#include <QGridLayout>
#include <QSpacerItem>
#include <QVBoxLayout>

VehiclePartsStatusWidget::VehiclePartsStatusWidget(QWidget *parent) : QGroupBox(parent) {
    using namespace Amass;
    m_lidar = new SingleSensorInformationGroupBox();
    m_lidar->setTitle("激光雷达");

    m_mill = new SensorInformationGroupBox();
    m_mill->setTitle("毫米波雷达");
    SensorInformationGroupBox::Sensors sensors;
    sensors.emplace(Sensor::Type::MillRadarFrontLeft, "前左");
    sensors.emplace(Sensor::Type::MillRadarFrontMiddle, "前中");
    sensors.emplace(Sensor::Type::MillRadarFrontRight, "前右");
    sensors.emplace(Sensor::Type::MillRadarBackLeft, "后左");
    sensors.emplace(Sensor::Type::MillRadarBackRight, "后右");
    m_mill->setSensors(sensors);

    m_camera = new SensorInformationGroupBox();
    m_camera->setTitle("相机");
    sensors.clear();
    sensors.emplace(Sensor::Type::CameraHD, "高清");
    sensors.emplace(Sensor::Type::CameraFront, "前");
    sensors.emplace(Sensor::Type::CameraBack, "后");
    sensors.emplace(Sensor::Type::CameraLeft, "左");
    sensors.emplace(Sensor::Type::CameraRight, "右");
    m_camera->setSensors(sensors);

    m_rtk = new SingleSensorInformationGroupBox();
    m_rtk->setTitle("组合惯导");

    auto cpu = new CpuInformationGroupBox();

    m_chassis = new ChassisInformationGroupBox();
    /*
    auto layout = new QVBoxLayout(this);
    layout->addWidget(lidar, 1);
    layout->addWidget(mill, 1);
    */

    auto layout = new QGridLayout(this);
    layout->addWidget(m_rtk, 0, 0);
    layout->addWidget(m_lidar, 0, 1);
    layout->addWidget(m_camera, 1, 0);
    layout->addWidget(m_mill, 1, 1);
    layout->addWidget(m_chassis, 2, 0);
    layout->addWidget(cpu, 2, 1);

    //    auto item1 = new QSpacerItem(10, 70);
    //    auto item2 = new QSpacerItem(10, 70);
    //    layout->addItem(item1, 3, 0);
    //    layout->addItem(item2, 3, 1);

    auto backend = Backend::instance();
    connect(backend, &Backend::cpuDataChanged, cpu, &CpuInformationGroupBox::setStatus);
    connect(m_mill, &SensorInformationGroupBox::currentSensorChanged, this,
            &VehiclePartsStatusWidget::onCurrentSensorChanged);
    connect(m_camera, &SensorInformationGroupBox::currentSensorChanged, this,
            &VehiclePartsStatusWidget::onCurrentSensorChanged);

    connect(backend, &Backend::chassisDataChanged, m_chassis, &ChassisInformationGroupBox::setChassisData);

    connect(backend, &Backend::sensorDataChanged, this, [this](const Amass::Sensor &sensor) {
        if (sensor.type == Amass::Sensor::Type::Lidar) {
            m_lidar->setSensorData(sensor);
        } else if (sensor.type == Amass::Sensor::Type::MillRadarFrontLeft ||
                   sensor.type == Amass::Sensor::Type::MillRadarFrontMiddle ||
                   sensor.type == Amass::Sensor::Type::MillRadarFrontRight ||
                   sensor.type == Amass::Sensor::Type::MillRadarBackLeft ||
                   sensor.type == Amass::Sensor::Type::MillRadarBackRight) {
            m_mill->setSensorData(sensor);
        } else if (sensor.type == Amass::Sensor::Type::CameraHD || sensor.type == Amass::Sensor::Type::CameraFront ||
                   sensor.type == Amass::Sensor::Type::CameraBack || sensor.type == Amass::Sensor::Type::CameraLeft ||
                   sensor.type == Amass::Sensor::Type::CameraRight) {
            m_camera->setSensorData(sensor);
        } else if (sensor.type == Amass::Sensor::Type::Gnss) {
            m_rtk->setSensorData(sensor);
        }
    });
}

void VehiclePartsStatusWidget::onCurrentSensorChanged(Amass::Sensor::Type type) {
    auto backend = Backend::instance();
    auto sensors = backend->sensors();
    auto iterator = sensors.find(type);
    if (iterator == sensors.cend()) return;

    if (dynamic_cast<SensorInformationGroupBox *>(sender()) == m_mill) {
        m_mill->setSensorData(iterator->second.data);
    } else if (dynamic_cast<SensorInformationGroupBox *>(sender()) == m_camera) {
        m_camera->setSensorData(iterator->second.data);
    }
}
