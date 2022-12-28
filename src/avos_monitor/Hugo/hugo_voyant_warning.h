#ifndef HUGO_VOYANT_WARNING_H
#define HUGO_VOYANT_WARNING_H
#include "objet_virtuel.h"


class hugo_voyant_warning : public objet_virtuel
{
public:
    hugo_voyant_warning();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;

    void OnDrawUltrasonic(QPainter *painter);

    int cligno;
    int cligno_ult;
    void MAJ();
    void MajUlt();
    QString icon_path_;
    QString brake_icon_path_;
    QPixmap image_warning, image_offline, image_wheel, image_car;
};

#endif // HUGO_VOYANT_WARNING_H
