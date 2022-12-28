#include "ChassisInformationGroupBox.h"
#include "SensorItem.h"
#include <QFormLayout>
#include <QGridLayout>
#include <QLineEdit>

static QString vcuStatusText(int status) {
    if (status == 0) {
        return "设备正常";
    } else {
        return "设备异常";
    }
}
ChassisInformationGroupBox::ChassisInformationGroupBox(QWidget *parent) : QGroupBox(parent) {
    setTitle("底盘状态信息");

    m_eps = new QLineEdit("EPS信息");
    m_eps->setReadOnly(true);

    m_vcu = new QLineEdit("VCU信息");
    m_vcu->setReadOnly(true);

    m_nboost = new QLineEdit("NBOOST信息");
    m_nboost->setReadOnly(true);

    m_timestamp = new QLineEdit();
    m_timestamp->setReadOnly(true);
    /*
    auto formLayout = new QFormLayout();
    formLayout->addRow("电量", battery);
    formLayout->addRow("车速", speed);
    formLayout->addRow("转角", steering);
    */

    auto layout = new QFormLayout(this);
    layout->addRow("EPS工作状态", m_eps);
    layout->addRow("VCU工作状态", m_vcu);
    layout->addRow("NBOOST工作状态", m_nboost);
    layout->addRow("最近一次时间戳", m_timestamp);
}

void ChassisInformationGroupBox::setChassisData(const Amass::Chassis &chassis) {
    if (chassis.type == Amass::Chassis::Type::EPS) {
        m_eps->setText(epsStateText(chassis.workState));
    } else if (chassis.type == Amass::Chassis::Type::VCU) {
        m_vcu->setText(vcuStatusText(chassis.workState));
    } else if (chassis.type == Amass::Chassis::Type::NBOOST) {
        m_nboost->setText(nboostStateText(chassis.workState));
        m_timestamp->setText(QString::number(chassis.timestamp));
    }
}
