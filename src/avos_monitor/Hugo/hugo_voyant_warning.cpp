#include <QTest>
#include "hugo_voyant_warning.h"

hugo_voyant_warning::hugo_voyant_warning()
{
    value = 0;
    cligno = 0;    
    cligno_ult = 0;
    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/warning.png");
    brake_icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/emergency_stop.png");
    QPixmap image0(icon_path_);
    image_warning = image0.scaled(20,20);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/offline.png");
    QPixmap image1(icon_path_);
    image_offline = image1.scaled(30,30);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/image/steering_wheel.png");
    image_wheel.load(icon_path_);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/image/car7.png");
    image_car.load(icon_path_);
}


QRectF hugo_voyant_warning::boundingRect() const
{
    return QRect(-800,-450,1600,900);
}

void hugo_voyant_warning::MAJ()
{
    if (this->cligno == 1)
    {
        this->cligno = 0;
    }
    else
    {
        this->cligno = 1;
    }
    QTest::qWait(500);
    update();
}

void hugo_voyant_warning::MajUlt()
{
    if (this->cligno_ult == 1)
    {
        this->cligno_ult = 0;
    }
    else
    {
        this->cligno_ult = 1;
    }
    QTest::qWait(500);
    update();
}

void hugo_voyant_warning::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    static int warning_level = 0;
    warning_level = getValue();
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));
    if (warning_level >= 2)
    {
        int x = 45;
        int y = 20;
        QRadialGradient radialGrad(QPointF(x+10, y+10), 15);
        // if (warning_level == 1)
        // {
        //     radialGrad.setColorAt(0, QColor(0,255,0,120));
        // }
        // else if (warning_level == 2)
        // {
        //     radialGrad.setColorAt(0, QColor(255,255,0,120));
        // }
        // else if (warning_level == 3)
        // {
        //     radialGrad.setColorAt(0, QColor(255,0,0,120));
        // }
        radialGrad.setColorAt(0, QColor(255,0,0,120));
        radialGrad.setColorAt(1, QColor(0,0,0,120));

        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->setOpacity(cligno);
        painter->drawPixmap(x,y,20,20,image_warning);
        painter->drawEllipse(x,y,20,20);
        MAJ();
    }
    // OnDrawUltrasonic(painter);
}

void hugo_voyant_warning::OnDrawUltrasonic(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));
    
    static int x = -30;
    static int y = 70;

    QRadialGradient radialGrad(QPointF(x + 30, y + 50), 50);
    radialGrad.setColorAt(0, QColor(255, 0, 0, 120));
    radialGrad.setColorAt(1, QColor(0, 0, 0, 120));

    QBrush brush(radialGrad);
    painter->setBrush(brush);
    painter->setOpacity(1);
    painter->drawPixmap(x, y, 60, 100, image_car);

    int radius = 6;
    painter->setBrush(Qt::red);
    painter->setOpacity(cligno_ult);

    if (getValue(ULTRASONIC_STATUS, 0) == 1)//左前
    {
        painter->drawEllipse(x+4, y+7, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 1) == 1)//前向
    {
        painter->drawEllipse(x+14, y, radius, radius);
        painter->drawEllipse(x+37, y, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 2) == 1)//右前
    {
        painter->drawEllipse(x+50, y+7, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 3) == 1)//右侧
    {
        painter->drawEllipse(x+50, y+7+25, radius, radius);
        painter->drawEllipse(x+50, y+7+50, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 4) == 1)//右后
    {
        painter->drawEllipse(x+50, y+90, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 5) == 1)//后向
    {
        painter->drawEllipse(x+14, y+97, radius, radius);
        painter->drawEllipse(x+37, y+97, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 6) == 1)//左后
    {
        painter->drawEllipse(x+4, y+90, radius, radius);
    }

    if (getValue(ULTRASONIC_STATUS, 7) == 1)//左侧
    {
        painter->drawEllipse(x+4, y+7+25, radius, radius);
        painter->drawEllipse(x+4, y+7+50, radius, radius);
    }
    painter->drawEllipse(x+4, y+7, radius, radius);
    painter->drawEllipse(x+14, y, radius, radius);
    painter->drawEllipse(x+37, y, radius, radius);
    painter->drawEllipse(x+50, y+7, radius, radius);

    painter->drawEllipse(x+4, y+7+25, radius, radius);
    painter->drawEllipse(x+4, y+7+50, radius, radius);

    painter->drawEllipse(x+50, y+7+25, radius, radius);
    painter->drawEllipse(x+50, y+7+50, radius, radius);

    painter->drawEllipse(x+4, y+90, radius, radius);
    painter->drawEllipse(x+14, y+97, radius, radius);
    painter->drawEllipse(x+37, y+97, radius, radius);
    painter->drawEllipse(x+50, y+90, radius, radius);
    MajUlt();
}
