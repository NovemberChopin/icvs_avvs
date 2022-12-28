#include "CarFrontSketchItem.h"

#include <QPainter>

// draw sensor yz
CarFrontSketchItem::CarFrontSketchItem(QWidget *parent) : SketchItem(parent) {}

void CarFrontSketchItem::paintEvent(QPaintEvent * /*event*/) {
    QPainter painter(this);
    painter.setBrush(Qt::red);
    // painter.drawRect(0, 0, width(), height());

    float width = this->width();
    float height = this->height();
    float diameter = width < height ? width : height;

    painter.setRenderHint(QPainter::Antialiasing);
    painter.translate(width / 2, height / 2);
    painter.scale(diameter / m_diameter, -diameter / m_diameter);

    drawCar(painter);
    drawSensors(m_sensors, painter);

    // painter.drawRect(-2000, -2000, 4000, 4000);
    drawXAxis(painter);
    drawYAxis(painter);

    drawLabel(painter);
}

void CarFrontSketchItem::drawLabel(QPainter &painter) {
    painter.save();
    QFont font("arial", 50);
    painter.setFont(font);

    painter.scale(1, -1);
    painter.drawText(1800, -100, "Y");

    painter.drawText(100, -1800, "Z");
    painter.restore();
}

void CarFrontSketchItem::drawSensors(const Sensors &sensors, QPainter &painter) {
    painter.save();
    QFont font("arial", 60);
    painter.setFont(font);

    for (auto &item : sensors) {
        auto &sensor = item.second.data;
        if (sensor.status != Amass::Status::Good && !m_showFatalItem) continue;
        auto &pixmap = item.second.data.status == Amass::Status::Good ? item.second.okPixmap : item.second.fatalPixmap;
        painter.drawPixmap(sensor.position.y() - pixmap.width() / 2 + m_offset.x(),
                           sensor.position.z() - pixmap.height() / 2 + m_offset.y(), pixmap);

        painter.save();
        painter.scale(1, -1);
        painter.drawText(sensor.position.y() - 150 + m_offset.x(), -sensor.position.z() + 180 - m_offset.y(),
                         sensorText(sensor.type));
        painter.restore();
    }
    painter.restore();
}
