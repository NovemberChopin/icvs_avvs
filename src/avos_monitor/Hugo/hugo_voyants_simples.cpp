#include "hugo_voyants_simples.h"

hugo_voyants_simples::hugo_voyants_simples(int param_x, int param_y, QString param_chemin,int red,int green,int blue,int param_size)
{
    value=0;
    x = param_x;
    y = param_y;
    chemin = param_chemin;
    couleur = QColor(red, green, blue, 160);
    size = param_size;
    QPixmap image(chemin);
    image_battery = image.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/emergency_stop.png");
    QPixmap image1(icon_path_);
    image_stop = image1.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/ceinture.png");
    QPixmap image2(icon_path_);
    image_belt = image2.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/door_open.png");
    QPixmap image3(icon_path_);
    image_door = image3.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/temperature.png");
    QPixmap image4(icon_path_);
    image_temp = image4.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/lidar_status.png");
    QPixmap image5(icon_path_);
    image_position = image5.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/dipped_headlight.png");
    QPixmap image6(icon_path_);
    image_croisement = image6.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/full_headlight.png");
    QPixmap image7(icon_path_);
    image_route = image7.scaled(size,size);

    icon_path_ = QString::fromStdString(ros::package::getPath("avos_monitor")+"/resources/Icones/aeb_enable.png");
    QPixmap image8(icon_path_);
    image_aeb = image8.scaled(size,size);
}


QRectF hugo_voyants_simples::boundingRect() const
{
    return QRect(-800,-450,1600,900);
}


void hugo_voyants_simples::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));
    int bias_dis = 40;
    if (getValue() == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2, y+size/2), size/2);
        radialGrad.setColorAt(0, couleur);
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x,y,size,size,image_battery);
        painter->drawEllipse(x,y,size,size);
    }

    if (getValue(BRAKE_LIGHT) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, couleur);
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+bias_dis,y,size,size,image_stop);
        painter->drawEllipse(x+bias_dis,y,size,size);        
    }
    
    if (getValue(LIGHT_STATUS) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 2*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, QColor(0,255,0,160));
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+2*bias_dis,y,size,size,image_croisement);
        painter->drawEllipse(x+2*bias_dis,y,size,size);        
    }    
    else if (getValue(LIGHT_STATUS) == 2)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 2*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, QColor(0,100,255,160));
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+2*bias_dis,y,size,size,image_route);
        painter->drawEllipse(x+2*bias_dis,y,size,size);        
    }

    if (getValue(AEB_ENABLE) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 3*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, QColor(200,200,0,160));
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+3*bias_dis,y,size,size,image_aeb);
        painter->drawEllipse(x+3*bias_dis,y,size,size);        
    }

    if (getValue(OVER_TEMP) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 4*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, couleur);
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+4*bias_dis,y,size,size,image_temp);
        painter->drawEllipse(x+4*bias_dis,y,size,size);        
    }

    if (getValue(LIDAR_STATUS) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 5*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, QColor(255,0,0,160));
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+5*bias_dis,y,size,size,image_position);
        painter->drawEllipse(x+5*bias_dis,y,size,size);        
    }
    
    if (getValue(BELT_STATUS) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 6*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, couleur);
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+6*bias_dis,y,size,size,image_belt);
        painter->drawEllipse(x+6*bias_dis,y,size,size);        
    }

    if (getValue(DOOR_STATUS) == 1)
    {
        QRadialGradient radialGrad(QPointF(x+size/2 + 7*bias_dis, y+size/2), size/2);
        radialGrad.setColorAt(0, couleur);
        radialGrad.setColorAt(1, QColor(0,0,0,120));
        QBrush brush(radialGrad);
        painter->setBrush(brush);
        painter->drawPixmap(x+7*bias_dis,y,size,size,image_door);
        painter->drawEllipse(x+7*bias_dis,y,size,size);        
    }
}

