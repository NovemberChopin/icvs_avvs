#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include "objet_virtuel.h"

class hugo_MyGraphicsitem : public objet_virtuel
{
public:
    hugo_MyGraphicsitem();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int current_speed = 30;
    QString station = "";
    float km = 0.0;
    QString icon_path_;
    // std::vector<QPixmap> image_vector;
};

#endif // MYGRAPHICSITEM_H
