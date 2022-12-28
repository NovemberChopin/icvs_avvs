#include "CpuInformationGroupBox.h"
#include <QFormLayout>
#include <QLineEdit>

CpuInformationGroupBox::CpuInformationGroupBox(QWidget *parent) : QGroupBox(parent) {
    setTitle("计算平台状态信息");

    auto layout = new QFormLayout(this);

    m_perception = new QLineEdit();
    m_perception->setReadOnly(true);

    m_localization = new QLineEdit();
    m_localization->setReadOnly(true);

    m_decision = new QLineEdit();
    m_decision->setReadOnly(true);

    m_control = new QLineEdit();
    m_control->setReadOnly(true);

    m_navigation = new QLineEdit();
    m_navigation->setReadOnly(true);

    m_timestamp = new QLineEdit();
    m_timestamp->setReadOnly(true);

    layout->addRow("感知模块", m_perception);
    layout->addRow("定位模块", m_localization);
    layout->addRow("决策模块", m_decision);
    layout->addRow("控制模块", m_control);
    layout->addRow("导航模块", m_navigation);
    layout->addRow("最近一次时间戳", m_timestamp);
}

void CpuInformationGroupBox::setStatus(const CpuStatus &status) {
    m_perception->setText(statusText(status.perception));
    m_localization->setText(statusText(status.localization));
    m_decision->setText(statusText(status.decision));
    m_control->setText(statusText(status.control));
    m_navigation->setText(statusText(status.navigation));
    m_timestamp->setText(QString::number(status.timestamp));
}
