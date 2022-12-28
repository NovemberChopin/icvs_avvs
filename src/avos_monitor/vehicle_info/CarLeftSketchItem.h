#ifndef CARLEFTSKETCHITEM_H
#define CARLEFTSKETCHITEM_H

#include "SensorItem.h"
#include "SketchItem.h"

class CarLeftSketchItem : public SketchItem {
    Q_OBJECT
public:
    CarLeftSketchItem(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) final;

    void drawLabel(QPainter &painter);
    void drawSensors(const Sensors &sensors, QPainter &painter);
};

#endif // CARLEFTSKETCHITEM_H
