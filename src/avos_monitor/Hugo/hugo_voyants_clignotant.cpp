#include "hugo_voyants_clignotant.h"
#include <QDebug>

hugo_voyants_clignotant::hugo_voyants_clignotant()
{
    value = 0;
    cligno = 0;
    icon_path_1 = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/turn_left.png");
    icon_path_2 = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/turn_right.png");

    QPixmap image1(icon_path_1);
    image_left = image1.scaled(30,30);

    QPixmap image2(icon_path_2);
    image_right = image2.scaled(30,30);
}

QRectF hugo_voyants_clignotant::boundingRect() const
{
    return QRect(-800,-450,1600,900);
}

void hugo_voyants_clignotant::MAJ()
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

void hugo_voyants_clignotant::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const int bias_dis = 25;
    painter->setRenderHint(QPainter::Antialiasing);///<Antialiasing//
    painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));

    if (getValue() !=0)
    {
        if (getValue()==-1)
        {
            QRadialGradient radialGrad(QPointF(-45+bias_dis, -90+15), 15);
            radialGrad.setColorAt(0, QColor(0,255,0));
            radialGrad.setColorAt(1, QColor(0,0,0,120));

            QBrush brush(radialGrad);
            painter->setBrush(brush);
            painter->setOpacity(cligno);
            painter->drawPixmap(-60+bias_dis,-90,30,30,image_left);
            painter->drawEllipse(-60+bias_dis,-90,30,30);
        }

        if (getValue()==1)
        {
            QRadialGradient radialGrad(QPointF(bias_dis, -90+15), 15);
            radialGrad.setColorAt(0, QColor(0,255,0));
            radialGrad.setColorAt(1, QColor(0,0,0,120));

            QBrush brush(radialGrad);
            painter->setBrush(brush);
            painter->setOpacity(cligno);
            painter->drawPixmap(bias_dis-15,-90,30,30,image_right);
            painter->drawEllipse(bias_dis-15,-90,30,30);
        }

        if (getValue()==2)
        {
            QRadialGradient radialGrad(QPointF(-45+bias_dis, -90+15), 15);
            radialGrad.setColorAt(0, QColor(0,255,0));
            radialGrad.setColorAt(1, QColor(0,0,0,120));

            QBrush brush(radialGrad);
            painter->setBrush(brush);
            painter->setOpacity(cligno);
            painter->drawPixmap(bias_dis-60,-90,30,30,image_left);
            painter->drawEllipse(bias_dis-60,-90,30,30);

            QRadialGradient radialGrad2(QPointF(bias_dis, -90+15), 15);
            radialGrad2.setColorAt(0, QColor(0,255,0));
            radialGrad2.setColorAt(1, QColor(0,0,0,120));

            QBrush brush2(radialGrad2);
            painter->setBrush(brush2);
            painter->setOpacity(cligno);
            painter->drawPixmap(bias_dis-15,-90,30,30,image_right);
            painter->drawEllipse(bias_dis-15,-90,30,30);
        }
        MAJ();
    }
}
