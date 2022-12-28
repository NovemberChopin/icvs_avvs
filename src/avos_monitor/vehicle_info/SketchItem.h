#ifndef SKETCHITEM_H
#define SKETCHITEM_H

#include "SensorItem.h"
#include <QWidget>

class SketchItem : public QWidget {
    Q_OBJECT
public:
    using Sensors = std::map<Amass::Sensor::Type, SensorItem>;
    SketchItem(QWidget *parent = nullptr);
    void updateSensor(const Amass::Sensor &sensor);
    void setSensors(const Sensors &sensors);
    void setCarPixmap(const QString &pixmap, size_t width, size_t height);
    void setOffset(float x, float y);
    void setRange(int xMin, int xMax, int yMin, int yMax, size_t interval);

protected:
    void drawXAxis(QPainter &painter);
    void drawYAxis(QPainter &painter);

    void drawXInch(QPainter &painter);
    void drawYInch(QPainter &painter);

    void drawCar(QPainter &painter);

    void timerEvent(QTimerEvent *event) final;
    void mouseMoveEvent(QMouseEvent *event) final;

    Sensors m_sensors;

    QPointF m_offset;
    int m_diameter{0};
    bool m_showFatalItem{false};

    int m_pixmapWidth{0};
    int m_pixmapHeight{0};
    QPixmap m_pixmap;

    int m_xMin{0};
    int m_xMax{0};
    int m_yMin{0};
    int m_yMax{0};
    size_t m_interval{0};
};

#endif // SKETCHITEM_H
