#include "hugo_mygraphicsitem.h"
#include <QtMath>
#include <QDebug>
#define pi 3.14159265
#include <QString>
#include <QTime>

hugo_MyGraphicsitem::hugo_MyGraphicsitem()
{
    value=0;
    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/car7.png");
}



QRectF hugo_MyGraphicsitem::boundingRect() const
{
    qreal penWidth = 5;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void hugo_MyGraphicsitem::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const int bias_dis = 125;
    painter->setRenderHints(QPainter::Antialiasing);///<Antialiasing//

    QLinearGradient linearGrad(QPointF(-450, -250), QPointF(-325, -200));
    linearGrad.setColorAt(0, QColor(0,26,77));
    linearGrad.setColorAt(1, Qt::black);
    QBrush black_brush(QColor(Qt::black));
    painter->setBrush(linearGrad);
    QRect rect_fond(-450,-350,900,600);
    painter->drawRect(rect_fond);

    painter->setPen(QPen( QColor(150,150,150,100) ,6, Qt::SolidLine,Qt::FlatCap));
    painter->drawArc(QRect(-450,-150,900,300),0,180*16);
    painter->drawArc(QRect(-450,120,900,300),0,180*16);
    painter->setPen(QPen( QColor(5,5,5) ,6, Qt::SolidLine,Qt::FlatCap));
    for (int i = -153; i < 121; i++)
    {
        painter->drawArc(QRect(-450,i,900,300),0,180*16);
        painter->drawArc(QRect(-450,i,900,300),0,180*16);
    }

    for (int i = 1; i < 20; i++)
    {
        painter->setPen(QPen( QColor(65,65,65,5*i) ,6, Qt::SolidLine,Qt::FlatCap));
        painter->drawArc(QRect(-450,-130-i,900,300),0,180*16);
    }

    for (int i = 1; i < 20; i++)
    {
        painter->setPen(QPen( QColor(40,40,40,5*i) ,6, Qt::SolidLine,Qt::FlatCap));
        painter->drawArc(QRect(-450,100+i,900,300),0,180*16);
    }

    // painter->setPen(QPen( QColor(0,255,0) ,2, Qt::SolidLine,Qt::FlatCap));
    // painter->drawArc(QRect(-450,123,900,300),0,180*16);

    QRect carre(25,-100,200,200);
    painter->setPen(QPen( QColor(51,102,153) ,2, Qt::SolidLine,Qt::FlatCap));
    painter->drawChord(carre,-60*16,300*16);

    painter->setPen(QPen(QColor(51,102,153,40) ,5, Qt::SolidLine,Qt::FlatCap));
    for (int i = 1; i < 5; i++)
    {
        QRect carre_grad(25+i,-100+i,2*(100-i),2*(100-i));
        painter->drawChord(carre_grad,-60*16,300*16);
    }


    QRect carre1(-225,-100,200,200);
    painter->setPen(QPen( QColor(51,102,153) ,2, Qt::SolidLine,Qt::FlatCap));
    painter->drawChord(carre1,-60*16,300*16);

    painter->setPen(QPen(QColor(51,102,153,40) ,5, Qt::SolidLine,Qt::FlatCap));
    for (int i = 1; i < 5; i++)
    {
        QRect carre_grad1(225+i,-100+i,2*(100-i),2*(100-i));
        painter->drawChord(carre_grad1,-60*16,300*16);
    }

    painter->setPen(QPen(QColor(Qt::green) , 1, Qt::SolidLine,Qt::FlatCap));
    painter->setFont(QFont("Arial", 9, -1,false));
    painter->drawText(113,-20,"km/h");
    painter->drawText(-150,-20,"x100r/min");

    QRect carre_rpm(-100,15,50,50);
    painter->setBrush(black_brush);
    painter->setPen(QPen(QColor(0,125,200) ,1, Qt::SolidLine,Qt::FlatCap));
    painter->drawEllipse(carre_rpm);
    // QRect carre_rpm2(120 + bias_dis,-10,80,80);
    // painter->drawEllipse(carre_rpm2);

    painter->setFont(QFont("Arial", 8, -1,false));
    painter->setPen(QPen(QColor(Qt::darkGray), 1, Qt::SolidLine,Qt::FlatCap));
    QStringList Regimes;
    Regimes<<"P"<<"R"<<"N"<<"D"<<"3"<<"2"<<"L";
    float teta=-30*3.14159/180;
    int i=0;
    while (i<7)
    {
        teta += 10*3.14159/180;
        if (i == getValue())
        {
            QRadialGradient radialGrad(QPointF(110*(qCos(teta))-bias_dis,110*(qSin(teta))-3), 12);
            radialGrad.setColorAt(0, QColor(0,255,0,80));
            radialGrad.setColorAt(1, QColor(0,0,0,0));
            painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));
            painter->setBrush(radialGrad);
            painter->drawEllipse(QPointF(110*(qCos(teta))-bias_dis,110*(qSin(teta))-3),20,20);
            painter->setPen(QPen(QColor(Qt::green), 1, Qt::SolidLine,Qt::FlatCap));
        }
        else
        {
            painter->setPen(QPen(QColor(Qt::darkGray), 1, Qt::SolidLine,Qt::FlatCap));
        }
        painter->drawText(110*(qCos(teta))-bias_dis - 4 + i ,110*(qSin(teta)),Regimes[i]);
        i++;
    }

    ///
    /// \brief Affichage d'une icone jauge d'essence
    ///    
    // QPixmap image(":/Images/voyant_carburant.jpg");
    // QPixmap image(icon_path_);
    // QPixmap image2 = image.scaled(20,20);
    // painter->drawPixmap(145 + bias_dis,-90,20,20,image2);
    // painter->drawEllipse(144 + bias_dis,-92,16,16);
}


