#include "CarTopSketchItem.h"

#include <QPainter>

CarTopSketchItem::CarTopSketchItem(QWidget *parent) : SketchItem(parent) {}

void CarTopSketchItem::paintEvent(QPaintEvent * /*event*/) {
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

    drawXAxis(painter);
    drawYAxis(painter);

    drawLabel(painter);

    // painter.drawRect(-2000, -2000, 4000, 4000);
}

void CarTopSketchItem::drawLabel(QPainter &painter) {
    painter.save();
    QFont font("arial", 60);
    painter.setFont(font);

    painter.scale(1, -1);
    painter.drawText(1800, -100, "X");

    painter.drawText(100, -1800, "Y");
    painter.restore();
}

void CarTopSketchItem::drawSensors(const Sensors &sensors, QPainter &painter) {
    painter.save();
    //    painter.scale(1, -1);
    QFont font("arial", 50);
    painter.setFont(font);
    for (auto &item : sensors) {
        if (item.second.data.status != Amass::Status::Good && !m_showFatalItem) continue;
        auto &pixmap = item.second.data.status == Amass::Status::Good ? item.second.okPixmap : item.second.fatalPixmap;
        painter.drawPixmap(item.second.data.position.x() - pixmap.width() / 2 + m_offset.x(),
                           item.second.data.position.y() - pixmap.height() / 2 + m_offset.y(), pixmap);
        painter.save();
        painter.scale(1, -1);
        painter.drawText(item.second.data.position.x() - 150 + m_offset.x(),
                         -item.second.data.position.y() + 180 - m_offset.y(), sensorText(item.second.data.type));
        painter.restore();
    }
    painter.restore();
}
