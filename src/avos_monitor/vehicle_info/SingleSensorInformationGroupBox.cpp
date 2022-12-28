#include "SingleSensorInformationGroupBox.h"
#include "InstallParameterGroupBox.h"
#include "QStatusIndicator.h"
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>

SingleSensorInformationGroupBox::SingleSensorInformationGroupBox(QWidget *parent) : QGroupBox(parent) {

    m_indicator = new QStatusIndicator();
    m_timestamp = new QLineEdit();

    auto statusLayout = new QFormLayout();
    statusLayout->addRow("工作状态", m_indicator);
    statusLayout->addRow("最近一次时间戳", m_timestamp);

    m_installParameter = new InstallParameterGroupBox();

    auto layout = new QVBoxLayout(this);
    layout->addLayout(statusLayout);
    layout->addWidget(m_installParameter);
}

void SingleSensorInformationGroupBox::setSensorData(const Amass::Sensor &sensor) {
    m_indicator->setActive(sensor.status == Amass::Status::Good);
    m_timestamp->setText(QString::number(sensor.timestamp));
    m_installParameter->setPosition(sensor.position.x(), sensor.position.y(), sensor.position.z());
    m_installParameter->setCalibrationParameter(sensor.yaw, sensor.pitch, sensor.roll);
}
