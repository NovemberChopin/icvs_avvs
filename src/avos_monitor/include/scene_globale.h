#ifndef SCENE_GLOBALE_H
#define SCENE_GLOBALE_H
#include "objet_virtuel.h"


class scene_globale : public QGraphicsScene
{
public:
    scene_globale(QGraphicsScene *parent=nullptr);

    objet_virtuel *Vitesse;
    objet_virtuel *Essence;
    objet_virtuel *CompteTours;
    objet_virtuel *Temperature;
    objet_virtuel *Clignotant;
    objet_virtuel *VoyantBatterie;
    objet_virtuel *Position;
    objet_virtuel *Croisement;
    objet_virtuel *Route;
    objet_virtuel *Warning;
    objet_virtuel *CompteurKm;
    objet_virtuel *AdaptiveSuspensionDampers;
    objet_virtuel *AutomaticTransmissionMode;
    objet_virtuel *FrontAntifog;
    objet_virtuel *RearAntifog;
    objet_virtuel *SeatBelt;
    objet_virtuel *RearWindowHeating;
    objet_virtuel *CheckEngine;
    objet_virtuel *Graphicsitem;
};

#endif // SCENE_GLOBALE_H
