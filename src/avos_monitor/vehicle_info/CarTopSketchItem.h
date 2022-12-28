#ifndef CARTOPSKETCHITEM_H
#define CARTOPSKETCHITEM_H

#include "SensorItem.h"
#include "SketchItem.h"

class CarTopSketchItem : public SketchItem {
    Q_OBJECT
public:
    CarTopSketchItem(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *event) final;

    void drawLabel(QPainter &painter);
    void drawSensors(const Sensors &sensors, QPainter &painter);
};

#endif // CARTOPSKETCHITEM_H
