#ifndef HUGO_VOYANTS_SIMPLES_H
#define HUGO_VOYANTS_SIMPLES_H

#include "objet_virtuel.h"



class hugo_voyants_simples : public objet_virtuel
{
public:
    hugo_voyants_simples(int,int, QString,int red=255,int green=0,int blue=0,int param_size=23);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

protected:
    int x,y,size=30;
    QString chemin;
    QColor couleur;
    QString icon_path_;
    QPixmap image_battery, image_stop, image_belt, image_door,image_temp, image_position, image_croisement, image_route, image_aeb;
};

#endif // HUGO_VOYANTS_SIMPLES_H
