#include "QStatusIndicator.h"
#include <QDebug>
#include <QPainter>

QStatusIndicator::QStatusIndicator(QWidget *parent) : QWidget(parent) {
    setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    setMinimumSize(25, 25);
}

void QStatusIndicator::setColor(const QColor &active, const QColor &inactive) {
    if (active.isValid()) {
        m_activeColor = active;
    }
    if (inactive.isValid()) {
        m_inactiveColor = inactive;
    }
}

void QStatusIndicator::setActive(bool active) {
    if (m_active == active) return;
    m_active = active;
    update();
}

void QStatusIndicator::setAlignment(Qt::Alignment alignment) {
    if (m_alignment == alignment) return;
    m_alignment = alignment;
}

void QStatusIndicator::paintEvent(QPaintEvent * /*event*/) {
    auto width = this->width();
    auto height = this->height();
    float diameter = width < height ? width : height;
    int radius = diameter / 2;

    QPoint center;
    if (m_alignment & Qt::AlignHCenter) {
        center.setX(width / 2);
        center.setY(height / 2);
    } else if (m_alignment & Qt::AlignLeft) {
        center.setX(radius);
        center.setY(height / 2);
    }

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing); //反锯齿
    painter.translate(center);                     //以widget中心为原点
    painter.scale(diameter / 200, diameter / 200); //比例缩放

    painter.setPen(Qt::NoPen);

    radius = 100;

    /* 外边框 */
    QLinearGradient lg1(0, -radius, 0, radius);
    lg1.setColorAt(0, QColor(255, 255, 255));
    lg1.setColorAt(1, QColor(166, 166, 166));
    painter.setBrush(lg1);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    /* 内边框 */
    radius -= 13;
    QLinearGradient lg2(0, -radius, 0, radius);
    lg2.setColorAt(0, QColor(166, 166, 166));
    lg2.setColorAt(1, QColor(220, 220, 220));
    painter.setBrush(lg2);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);

    /* 内部指示颜色 */
    radius -= 4;
    QRadialGradient rg(0, 0, radius);
    const auto &color = m_active ? m_activeColor : m_inactiveColor;

    rg.setColorAt(0, color.lighter(110));
    rg.setColorAt(0.6, color);
    rg.setColorAt(1, color.darker(130));

    painter.setBrush(rg);
    painter.drawEllipse(-radius, -radius, radius << 1, radius << 1);
}
