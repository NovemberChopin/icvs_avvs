#include "SketchItem.h"
#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QToolTip>

SketchItem::SketchItem(QWidget *parent) : QWidget(parent) {
    startTimer(1000);
    setMouseTracking(true);
}

void SketchItem::updateSensor(const Amass::Sensor &sensor) {
    auto iterator = m_sensors.find(sensor.type);
    if (iterator == m_sensors.cend()) return;
    auto pos = iterator->second.data.position;
    iterator->second.data = sensor;
    iterator->second.data.position = pos;
}

void SketchItem::setSensors(const Sensors &sensors) { m_sensors = sensors; }

void SketchItem::setCarPixmap(const QString &pixmap, size_t width, size_t height) {
    m_pixmapWidth = width;
    m_pixmapHeight = height;
    m_pixmap = QPixmap(pixmap);
}

void SketchItem::setOffset(float x, float y) { m_offset = QPointF(x, y); }

void SketchItem::drawCar(QPainter &painter) {
    painter.save();
    painter.scale(1, -1);
    painter.drawPixmap(-m_pixmapWidth / 2, -m_pixmapHeight / 2, m_pixmapWidth, m_pixmapHeight, m_pixmap);
    painter.restore();
}

void SketchItem::drawXAxis(QPainter &painter) {
    painter.save();
    painter.translate(m_offset);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawLine(-m_diameter, 0, m_diameter, 0);
    drawXInch(painter);
    painter.restore();
}

void SketchItem::drawXInch(QPainter &painter) {
    painter.save();
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(10);
    painter.setPen(pen);

    QFont font("arial", 50);
    painter.setFont(font);
    for (int i = -m_diameter; i <= m_diameter; i += m_interval) {
        painter.drawLine(i, 0, i, 30);
        painter.save();
        painter.scale(1, -1);
        painter.drawText(i - 50, 100, QString::number(i));
        painter.restore();
    }
    painter.restore();
}

void SketchItem::drawYInch(QPainter &painter) {
    painter.save();
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(10);
    painter.setPen(pen);

    QFont font("arial", 50);
    painter.setFont(font);
    for (int i = -m_diameter; i <= m_diameter; i += m_interval) {
        painter.drawLine(0, i, 30, i);
        painter.save();
        painter.scale(1, -1);
        painter.drawText(-180, -i, QString::number(i));
        painter.restore();
    }
    painter.restore();
}

void SketchItem::drawYAxis(QPainter &painter) {
    painter.save();
    painter.translate(m_offset);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(10);
    painter.setPen(pen);
    painter.drawLine(0, m_diameter, 0, -m_diameter);
    drawYInch(painter);
    painter.restore();
}

void SketchItem::timerEvent(QTimerEvent * /*event*/) {
    m_showFatalItem = !m_showFatalItem;
    update();
}

void SketchItem::mouseMoveEvent(QMouseEvent *event) {
    float width = this->width();
    float height = this->height();
    QPoint center(width / 2, height / 2);
    float diameter = width < height ? width : height;
    auto scale = m_diameter / diameter;
    // qDebug() << (event->pos() - center) * scale;
}

void SketchItem::setRange(int xMin, int xMax, int yMin, int yMax, size_t interval) {
    m_xMin = xMin;
    m_xMax = xMax;
    m_yMin = yMin;
    m_yMax = yMax;

    m_interval = interval;
    m_diameter = m_xMax - m_xMin;
}
