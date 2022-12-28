#ifndef VEHICLEPARTSSTATUSWIDGET_H
#define VEHICLEPARTSSTATUSWIDGET_H

#include "SensorItem.h"
#include <QGroupBox>

class SingleSensorInformationGroupBox;
class SensorInformationGroupBox;
class ChassisInformationGroupBox;

class VehiclePartsStatusWidget : public QGroupBox {
    Q_OBJECT
public:
    VehiclePartsStatusWidget(QWidget *parent = nullptr);

protected:
    void onCurrentSensorChanged(Amass::Sensor::Type type);

private:
    SingleSensorInformationGroupBox *m_lidar{nullptr};
    SensorInformationGroupBox *m_mill{nullptr};
    SensorInformationGroupBox *m_camera{nullptr};
    SingleSensorInformationGroupBox *m_rtk{nullptr};
    ChassisInformationGroupBox *m_chassis{nullptr};
};

#endif // VEHICLEPARTSSTATUSWIDGET_H
