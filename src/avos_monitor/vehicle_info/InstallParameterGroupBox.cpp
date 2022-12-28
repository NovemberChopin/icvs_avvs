#include "InstallParameterGroupBox.h"
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>

InstallParameterGroupBox::InstallParameterGroupBox(QWidget *parent) : QWidget(parent) {
    auto installGroupBox = new QGroupBox("安装参数");
    m_x = new QLineEdit();
    m_x->setReadOnly(true);

    m_y = new QLineEdit();
    m_y->setReadOnly(true);

    m_z = new QLineEdit();
    m_z->setReadOnly(true);

    auto installForm = new QFormLayout(installGroupBox);
    installForm->addRow("x", m_x);
    installForm->addRow("y", m_y);
    installForm->addRow("z", m_z);

    auto calibrationGroupBox = new QGroupBox("标定参数");

    m_roll = new QLineEdit();
    m_roll->setReadOnly(true);

    m_yaw = new QLineEdit();
    m_yaw->setReadOnly(true);

    m_pitch = new QLineEdit();
    m_pitch->setReadOnly(true);

    auto calibrationForm = new QFormLayout(calibrationGroupBox);
    calibrationForm->addRow("roll", m_roll);
    calibrationForm->addRow("yaw", m_yaw);
    calibrationForm->addRow("pitch", m_pitch);

    auto layout = new QHBoxLayout(this);
    layout->addWidget(installGroupBox);
    layout->addWidget(calibrationGroupBox);
}

void InstallParameterGroupBox::setPosition(float x, float y, float z) {
    m_x->setText(QString::number(x));
    m_y->setText(QString::number(y));
    m_z->setText(QString::number(z));
}

void InstallParameterGroupBox::setCalibrationParameter(float yaw, float pitch, float roll) {
    m_yaw->setText(QString::number(yaw));
    m_pitch->setText(QString::number(pitch));
    m_roll->setText(QString::number(roll));
}
