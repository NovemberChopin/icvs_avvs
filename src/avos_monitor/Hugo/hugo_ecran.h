#ifndef HUGO_ECRAN_H
#define HUGO_ECRAN_H
#include "objet_virtuel.h"



class hugo_ecran : public objet_virtuel
{
public:
    hugo_ecran();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPixmap Get4GSignalPixmap(const int linenum);//获取信号位图
    QPixmap GetWifiSignalPixmap(const QColor &color,const int linenum);//获取信号位图
    QString GetLocationStatus(QPainter *painter);
    QString GetVehicleStatus(QPainter *painter);

};

#endif // HUGO_ECRAN_H
