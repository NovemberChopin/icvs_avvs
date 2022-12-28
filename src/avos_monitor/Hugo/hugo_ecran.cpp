#include "hugo_ecran.h"
#include <QTime>

hugo_ecran::hugo_ecran()
{
    value = 0;
}

QRectF hugo_ecran::boundingRect() const
{
    qreal penWidth = 5;
    return QRectF(-10 - penWidth / 2, -10 - penWidth / 2, 20 + penWidth, 20 + penWidth);
}

void hugo_ecran::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    const int bias_dis = 120;

    painter->setPen(QPen(QColor(0,0,0,0) , 1, Qt::SolidLine,Qt::FlatCap));
    painter->setBrush(QColor(0,128,255,15));
    painter->drawRect(-45 + bias_dis,92,90,30);
    QRadialGradient radialGrad(QPointF(0, 45), 60);
    radialGrad.setColorAt(0, QColor(0,204,0));
    radialGrad.setColorAt(1, QColor(30, 30, 30));
    painter->setPen(QPen(QColor(155,155,155) , 1, Qt::SolidLine,Qt::FlatCap));
    painter->setBrush(radialGrad);

    // painter->drawRect(-40 + bias_dis,52,90,25);

    QTime time = QTime::currentTime();
    time.currentTime();
    painter->setPen(QPen(Qt::green, 2, Qt::SolidLine,Qt::FlatCap));
    painter->setFont(QFont("Arial", 10, -1,false));
    painter->drawText(-15,-156,time.toString("hh:mm:ss"));

    // painter->setPen(QPen(Qt::green, 6, Qt::SolidLine,Qt::FlatCap));
    // painter->setFont(QFont("Arial", 14, -1,false));
    // painter->drawText(-32 + bias_dis,72,GetVehicleStatus(painter));
    // painter->drawText(-10 + bias_dis,75, QString::number(getValue(),'f', 1));

    painter->setPen(QPen(QColor( 26, 13, 0), 1, Qt::SolidLine,Qt::FlatCap));
    painter->setBrush(QColor( 26,13,0));
    QPixmap image_4g = Get4GSignalPixmap(getValue());
    painter->drawPixmap(-205,-170,20,15,image_4g);

    if (getValue() > 0)
    {
        painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
    }
    else
    {
        painter->setPen(QPen(Qt::white, 1, Qt::SolidLine,Qt::FlatCap));
    }
    painter->setFont(QFont("Arial", 10, -1,false));
    painter->drawText(-225,-156,"4G");

    // painter->setPen(QPen(Qt::white, 1, Qt::SolidLine,Qt::FlatCap));
    painter->drawText(-180,-156, GetLocationStatus(painter));

    // if (getValue(ON_LINE_STATUS) == 0)
    // {
    //     painter->setPen(QPen(Qt::red , 1, Qt::SolidLine,Qt::FlatCap));
    //     painter->drawText(-120,-156,"车辆离线");
    // }
    // else
    // {        
    //     painter->setPen(QPen(Qt::green , 1, Qt::SolidLine,Qt::FlatCap));
    //     painter->drawText(-120,-156,"车辆在线");
    // }

    painter->setPen(QPen(Qt::green , 1, Qt::SolidLine,Qt::FlatCap));
    // painter->drawText(-170,75, QString("续航:%1km").arg(getValue(DRIVING_MILEAGE)));
    // painter->drawText(-215,160, QString("行驶里程:%1km").arg(getValue(TOTAL_DISTANCE)));

    // QPixmap image_wifi = GetWifiSignalPixmap(QColor(255, 255, 255),4);
    // painter->drawPixmap(205,-120,30,25,image_wifi);
    int battery_value = getValue(BATTERY_VALUE);
    painter->setFont(QFont("Arial", 8, -1,false)); 
    if (battery_value <= 20)
    {
        painter->setPen(QColor(200,0,0));
        painter->drawText(170,-156, QString("%1%").arg(battery_value)); 
        painter->drawRoundedRect(200, -167, 23, 14, 2, 2);  
        painter->setBrush(QColor(255, 0, 0));
    }
    else
    {
        painter->setPen(QColor(137, 249, 83));
        painter->drawText(170,-156, QString("%1%").arg(battery_value));
        painter->drawRoundedRect(200, -167, 23, 14, 2, 2);  
        painter->setBrush(QColor(137, 249, 83));
    }
    painter->drawRect(223, -163, 2, 6);
    painter->drawRect(202, -165, battery_value/5, 10);
}

QPixmap hugo_ecran::Get4GSignalPixmap(const int linenum)//获取信号位图
{ 
    int recWidth = 20;       //绘制区域的宽度和高度
    int recHeight = 15;
    int signalWidth = recWidth/4;
    int signalHeight = recHeight/4;
    QPixmap pixmap(recWidth,recHeight+2);
    QBrush brush(QColor( 0, 255, 0));    
    pixmap.fill(QColor(100,100,100,0));
    QPainter painter(&pixmap);
    painter.setPen(QColor(100,100,100)); 

    for (int i = 1; i <= 4; ++i)
    {
        painter.drawRect(signalWidth*(i-1),recHeight-signalHeight*i, signalWidth-3,signalHeight*i);
    } 

    if ((linenum > 0) && (linenum < 5))
    {        
        painter.setPen(QColor(0,255,0)); 
        painter.setBrush(brush);
        for (int i = 1; i <= linenum; ++i)
        {
            painter.drawRect(signalWidth*(i-1),recHeight-signalHeight*i, signalWidth-3,signalHeight*i);
        }
    }
    return pixmap;
}

QPixmap hugo_ecran::GetWifiSignalPixmap(const QColor &color,const int linenum)//获取信号位图
{
    QPixmap pixmap(30,25);
    pixmap.fill(QColor(255,255,255,0));
    QPainter painter(&pixmap);
    painter.setPen(QPen(color,1));

    for(int i = 1, xpos = 0; i <= linenum; ++i, ++xpos)
    {
        painter.drawArc(15-i*3,25-i*5,i*6,i*10,53.1*16,36.9*2*16);    
    }
    return pixmap;
}

QString hugo_ecran::GetLocationStatus(QPainter *painter)
{
    QString strstatus = "";

    switch((int)getValue(LOCATION_STATUS))
    {
        case 0:
        {
            strstatus = "定位成功";
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 1:
        {
            strstatus = "定位一般";
            painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 2:
        {
            strstatus = "定位失败";
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        default:
        {
            strstatus = "定位异常" ;   
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine,Qt::FlatCap));        
        }
        break;
    }

    return strstatus;
}

QString hugo_ecran::GetVehicleStatus(QPainter *painter)
{
    QString strstatus = "";

    switch((int)getValue(VEHICLE_STATUS))
    {
        case 0:
        {
            strstatus = "  Init...";
            // strstatus = "初始化...";
            painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 1:
        {
            strstatus = "   Sleep";
            // strstatus = "车辆休眠";
            painter->setPen(QPen(Qt::yellow, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 2:
        {
            strstatus = "   Ready";
            // strstatus = "遥控就绪";
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 3:
        {
            strstatus = "   Ready";
            // strstatus = "自动就绪";
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 4:
        {
            strstatus = "  Remote";
            // strstatus = "遥控驾驶";
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 5:
        {
            strstatus = "    Auto";
            // strstatus = "自动驾驶";
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 7:
        {
            strstatus = "    Stop";
            // strstatus = "车辆急停";
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        case 8:
        {
            strstatus = "   Fault";
            // strstatus = "车辆故障";
            painter->setPen(QPen(Qt::red, 1, Qt::SolidLine,Qt::FlatCap));
        }
        break;
        default:
        {
            strstatus = "Abnormal" ;   
            // strstatus = "车辆异常" ; 
            painter->setPen(QPen(Qt::green, 1, Qt::SolidLine,Qt::FlatCap));        
        }
        break;
    }
    return strstatus;
}