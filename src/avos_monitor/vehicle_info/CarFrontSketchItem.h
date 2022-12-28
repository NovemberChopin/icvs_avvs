#ifndef CARSKETCHITEM_H
#define CARSKETCHITEM_H

#include "SketchItem.h"
#include <vector>

class CarFrontSketchItem : public SketchItem {
    Q_OBJECT
public:
    CarFrontSketchItem(QWidget *parent = nullptr);

    void setCoordinateWidth();

protected:
    void paintEvent(QPaintEvent *event) final;

    void drawLabel(QPainter &painter);
    void drawSensors(const Sensors &sensors, QPainter &painter);
};

#endif // CARSKETCHITEM_H
