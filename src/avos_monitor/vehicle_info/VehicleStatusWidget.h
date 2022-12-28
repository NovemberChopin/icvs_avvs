#ifndef VEHICLESTATUSWIDGET_H
#define VEHICLESTATUSWIDGET_H

#include "SensorItem.h"
#include <QGroupBox>

class CarFrontSketchItem;
class CarLeftSketchItem;
class CarTopSketchItem;

class VehicleStatusWidget : public QGroupBox {
    Q_OBJECT
public:
    VehicleStatusWidget(QWidget *parent = nullptr);

private:
    CarFrontSketchItem *m_frontView{nullptr};
    CarLeftSketchItem *m_leftView{nullptr};
    CarTopSketchItem *m_topView{nullptr};
};

#endif // VEHICLESTATUSWIDGET_H
