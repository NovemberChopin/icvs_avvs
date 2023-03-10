#ifndef COMPTEUR_H
#define COMPTEUR_H
#include <QGraphicsItem>
#include <QColor>
#include "objet_virtuel.h"



class hugo_Compteur : public objet_virtuel
{
public:
    hugo_Compteur(int,int,int,int,int,QStringList,int,int,int,int,int,int,int param_critique=100, int red2=100,int green2=100,int blue2=100);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    double x=0,y=0,r=100,start_angle=0,end_angle=360,  nbre_graduations=12,critique,angle=0,value2=0,r_verre,direction_grad=1;
    QColor couleur=QColor(100,100,100);
    QColor couleur2=QColor(100,100,100);
    QColor couleurgrad=QColor(100,100,100,50);
    QColor couleurgrad2=QColor(100,100,100,50);
    QStringList graduations;
    double per_angle = 20;
};

#endif // COMPTEUR_H
