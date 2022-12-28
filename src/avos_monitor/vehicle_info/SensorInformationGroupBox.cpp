#include "SensorInformationGroupBox.h"
#include "InstallParameterGroupBox.h"
#include "QStatusIndicator.h"
#include <QComboBox>
#include <QDebug>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>

SensorInformationGroupBox::SensorInformationGroupBox(QWidget *parent) : QGroupBox(parent) {

    m_comboBox = new QComboBox();
    connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onSensorChanged(int)));

    auto positionLayout = new QFormLayout();
    positionLayout->addRow("位置", m_comboBox);

    m_indicator = new QStatusIndicator();
    m_timestamp = new QLineEdit();
    auto statusLayout = new QFormLayout();
    statusLayout->addRow("工作状态", m_indicator);
    statusLayout->addRow("最近一次时间戳", m_timestamp);

    m_installParameter = new InstallParameterGroupBox();

    auto layout = new QVBoxLayout(this);
    layout->addLayout(positionLayout);
    layout->addLayout(statusLayout);
    layout->addWidget(m_installParameter);
}

void SensorInformationGroupBox::setSensors(const Sensors &sensors) {
    m_sensors = sensors;
    for (auto &sensor : sensors) {
        m_comboBox->addItem(QString::fromStdString(sensor.second), static_cast<int>(sensor.first));
    }
}

void SensorInformationGroupBox::setSensorData(const Amass::Sensor &sensor) {
    if (sensor.type != static_cast<Amass::Sensor::Type>(m_comboBox->currentData().toInt())) return;
    m_indicator->setActive(sensor.status == Amass::Status::Good);
    m_timestamp->setText(QString::number(sensor.timestamp));
    m_installParameter->setPosition(sensor.position.x(), sensor.position.y(), sensor.position.z());
    m_installParameter->setCalibrationParameter(sensor.yaw, sensor.pitch, sensor.roll);
}

void SensorInformationGroupBox::onSensorChanged(int index) {
    // qDebug() << "onSensorChanged" << index;
    auto type = static_cast<Amass::Sensor::Type>(m_comboBox->currentData().toInt());
    emit currentSensorChanged(type);
}
// InstallParameterGroupBox
