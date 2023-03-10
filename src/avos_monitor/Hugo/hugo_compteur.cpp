#include "hugo_compteur.h"
#include <QPainter>
#include <QtMath>
#include <QDebug>
#define pi 3.14159265

hugo_Compteur::hugo_Compteur(int param_x, int param_y, int param_r, int param_start_angle, 
    int param_end_angle, QStringList param_graduations, int param_value, int param_r_verre, 
    int param_direction_grad, int red, int green, int blue, int param_critique, int red2, int green2, int blue2)
{
    x = param_x;
    y = param_y;
    r = param_r;
    start_angle = param_start_angle;
    end_angle = param_end_angle;
    nbre_graduations = param_graduations.length()-1;
    graduations = param_graduations;
    couleur = QColor(red,green,blue);
    couleurgrad = QColor(red,green,blue,20),
    critique = param_critique;
    couleur2 = QColor(red2,green2,blue2);
    couleurgrad2 = QColor(255,green2,blue2,20);
    value2 = param_value;
    valueMax = value2;
    direction_grad = param_direction_grad;
    r_verre = param_r_verre;
    per_angle = (start_angle - end_angle)/nbre_graduations;
}


QRectF hugo_Compteur::boundingRect() const
{
    qreal penWidth = 5;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void hugo_Compteur::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    float value_temp = getValue();
    painter->setRenderHints(QPainter::Antialiasing);

    QRect carre_rpm(x-r,y-r,2*r,2*r);
    //Support du cadran
    painter->setPen(QPen( couleur ,2, Qt::SolidLine,Qt::FlatCap));
    painter->drawArc(carre_rpm,start_angle*16,(end_angle-start_angle)*16);
    painter->setPen(QPen(couleurgrad ,5, Qt::SolidLine,Qt::FlatCap));
    //Boucle pour l'effet de gradient
    for (int i = 1; i < 5; i++)
    {
        QRect carre_grad(x-(r-i),y-(r-i),2*(r-i),2*(r-i));
        painter->drawArc(carre_grad,start_angle*16,(end_angle-start_angle)*16);
    }
    //On rentre dans la boucle uniquement si l'on veut donner 2 couleurs au compteur. Se définit dans le constructeur, non par défaut.
    // if (critique != 100)
    // {
    //     painter->setPen(QPen( QColor(0,0,0) ,5, Qt::SolidLine,Qt::FlatCap));
    //     for (int i = 1; i < 5; i++)
    //     {
    //         QRect carre_grad(x-(r-i),y-(r-i),2*(r-i),2*(r-i));
    //         painter->drawArc(carre_grad,98*16,-39*16);
    //     }
    //     painter->setPen(QPen( couleur2 ,2, Qt::SolidLine,Qt::FlatCap));
    //     painter->drawArc(carre_rpm,98*16,-39*16);
    //     painter->setPen(QPen( couleurgrad2 ,5, Qt::SolidLine,Qt::FlatCap));
    //     for (int i = 1; i < 5; i++)
    //     {
    //         QRect carre_grad(x-(r-i),y-(r-i),2*(r-i),2*(r-i));
    //         painter->drawArc(carre_grad,98*16,-39*16);
    //     }
    // }
    if (critique != 100)
    {
        painter->setPen(QPen( QColor(0,0,0) ,5, Qt::SolidLine,Qt::FlatCap));
        for (int i = 0; i < 5; i++)
        {
            QRect carre_grad(x-(r-i),y-(r-i),2*(r-i),2*(r-i));
            painter->drawArc(carre_grad,(start_angle-critique*per_angle)*16,-(nbre_graduations-critique)*per_angle*16);
        }
        painter->setPen(QPen( couleur2 ,2, Qt::SolidLine,Qt::FlatCap));
        painter->drawArc(carre_rpm,(start_angle-critique*per_angle)*16,-(nbre_graduations-critique)*per_angle*16);

        painter->setPen(QPen( couleurgrad2 ,5, Qt::SolidLine,Qt::FlatCap));
        for (int i = 0; i < 5; i++)
        {
            QRect carre_grad(x-(r-i),y-(r-i),2*(r-i),2*(r-i));
            painter->drawArc(carre_grad,(start_angle-critique*per_angle)*16,-(nbre_graduations-critique)*per_angle*16);
        }
    }

    painter->setPen(QPen( couleur ,1, Qt::SolidLine,Qt::FlatCap));
    //Boucle pour les grandes graduations
    int compteur=0;
    while (compteur <= nbre_graduations)
    {
        float i = start_angle*1.0+compteur*(end_angle*1.0-start_angle*1.0)/(nbre_graduations*1.0);
        if (compteur > critique)
        {
            painter->setPen(QPen( couleur2 ,1, Qt::SolidLine,Qt::FlatCap));
        }
        painter->drawLine((r-10)*qCos(i*(pi/180))+x,-(r-10)*qSin(i*(pi/180))+y,(r)*qCos(i*(pi/180))+x,-(r)*qSin(i*(pi/180))+y);
        compteur++;
    }
    //Boucle pour les petites graduations
    compteur = 0;
    painter->setPen(QPen( couleur ,1, Qt::SolidLine,Qt::FlatCap));
    while (compteur <= nbre_graduations-1)
    {
        float i = start_angle*1.0+(end_angle*1.0-start_angle*1.0)/(nbre_graduations*2.0)+compteur*(end_angle*1.0-start_angle*1.0)/(nbre_graduations*1.0);
        if (compteur > critique)
        {
            painter->setPen(QPen( couleur2 ,1, Qt::SolidLine,Qt::FlatCap));
        }
        painter->drawLine((r-5)*qCos(i*(pi/180))+x,-(r-5)*qSin(i*(pi/180))+y,(r)*qCos(i*(pi/180))+x,-(r)*qSin(i*(pi/180))+y);
        compteur++;
    }

    int j=0;
    painter->setPen(QPen(QColor(Qt::green) , 1, Qt::SolidLine,Qt::FlatCap));
    if (r > 60)
    {        
        painter->setFont(QFont("Arial", 9, -1,false));
    }
    else
    {        
        painter->setFont(QFont("Arial", 6, -1,false));
    }
    compteur=0;
    while (compteur <= nbre_graduations)
    {
        float i = start_angle*1.0+compteur*(end_angle*1.0-start_angle*1.0)/(nbre_graduations*1.0);
        painter->drawText(qCos(i*pi/180)*(r-20)+x-11,-qSin(i*pi/180)*(r-20)+y+3,graduations[j]);
        j++;
        compteur++;
    }

    painter->setPen(QPen(QColor(Qt::darkGray) , 1, Qt::SolidLine,Qt::FlatCap));
    QRect carre_aiguille(x-10,y-10,20,20);
    painter->drawEllipse(carre_aiguille);
    QRect carre_aiguille2(x-8,y-8,16,16);
    QLinearGradient linearGrad(QPointF((x+10*qCos(135*pi/180)), (y-10*qSin(135*pi/180))), QPointF(x, y));
    linearGrad.setColorAt(0, QColor(200,200,200,80));
    linearGrad.setColorAt(1, QColor(0,0,0,0));
    painter->setBrush(linearGrad);
    painter->setPen(QPen(QColor(30,30,30) , 1, Qt::SolidLine,Qt::FlatCap));
    painter->drawEllipse(carre_aiguille2);

    angle = ((1.0*(start_angle-end_angle))/(value2*1.0))*value_temp*1.0+(360-start_angle);

    QPointF points[4]={
        QPointF(x,y),
        QPointF(x+(r/4)*qCos((angle+8)*pi/180),y+(r/4)*qSin((angle+8)*pi/180)),
        QPointF(x+r*qCos(angle*pi/180),y+r*qSin(angle*pi/180)),
        QPointF(x+(r/4)*qCos((angle-8)*pi/180),y+(r/4)*qSin((angle-8)*pi/180)),
    };
    QLinearGradient linearGrad_aiguille( QPointF(x+r*qCos(angle*pi/180),y+r*qSin(angle*pi/180)), QPointF(x, y));
    linearGrad_aiguille.setColorAt(0, QColor(Qt::darkRed));
    linearGrad_aiguille.setColorAt(0.5, QColor(230, 46, 0));
    linearGrad_aiguille.setColorAt(1, QColor(Qt::darkRed));

    QBrush brushgrad(linearGrad_aiguille);
    painter->setBrush(brushgrad);
    painter->setPen(QPen(QColor(Qt::darkRed), 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPolygon(points,4);

    QRect carre(x-r_verre,y-r_verre,2*r_verre,2*r_verre);
    painter->setPen(QPen( QColor(0,0,0,0) ,1, Qt::SolidLine,Qt::FlatCap));
    if (direction_grad == 1)
    {
        QLinearGradient linearGrad(QPointF((x+r_verre*qCos(135*pi/180)), (y-r_verre*qSin(135*pi/180))), QPointF(x, y));
        linearGrad.setColorAt(0, QColor(255,255,255,80));
        linearGrad.setColorAt(1, QColor(0,0,0,0));
        QBrush brush(linearGrad);
        painter->setBrush(brush);
        painter->drawEllipse(carre);
    }
    else if (direction_grad == -1)
    {
        QLinearGradient linearGrad(QPointF((x+r_verre*qCos(135*pi/180)), (y-r_verre*qSin(135*pi/180))), QPointF(x, y));
        linearGrad.setColorAt(0, QColor(255,255,255,30));
        linearGrad.setColorAt(1, QColor(0,0,0,0));
        QBrush brush(linearGrad);
        painter->setBrush(brush);
        painter->drawEllipse(carre);
    }

    if (r > 60)
    {
        painter->setPen(QPen(QColor(Qt::green) , 6, Qt::SolidLine,Qt::FlatCap));
        painter->setFont(QFont("Arial", 14, -1,false));
        if (value_temp >= 100)
        {
            painter->drawText(x-20,35,QString::number(value_temp,'f',0));
        }
        else if(value_temp < 10)
        {
            // painter->drawText(x-8,35,QString("%1").arg(value_temp));     
            painter->drawText(x-20,35,QString::number(value_temp,'f',2));          
        }
        else 
        {            
            painter->drawText(x-20,35,QString::number(value_temp,'f',1));
        }
        // painter->drawText(x,30,"120");
    }
}

