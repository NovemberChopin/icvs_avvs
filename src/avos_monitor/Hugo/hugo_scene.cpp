#include "hugo_scene.h"
#include "hugo_mygraphicsitem.h"
#include "hugo_compteur.h"
#include "hugo_ecran.h"
#include "hugo_voyant_warning.h"
#include <QDebug>
#include <QTimer>


hugo_scene::hugo_scene(scene_globale *parent):scene_globale(parent)
{
    std::string icon_path = "";
    int bias_dis = 25;

    //Création de la scène de fond
    Graphicsitem = new hugo_MyGraphicsitem();
    this->addItem(Graphicsitem);

    // //Création de l'affichage de l'heure et du compteur km
    CompteurKm = new hugo_ecran;
    this->addItem(CompteurKm);

    // //Compteur vitesse
    QStringList graduations_vitesse;
    graduations_vitesse<<"  0"<<"  5"<<" 10"<<" 15"<<" 20"<<" 25"<<" 30"<<" 35"<<" 40"<<" 45"<<" 50"<<" 55";
    Vitesse = new hugo_Compteur(125,0,90,180,-30,graduations_vitesse,55,100,1,0,64,128,9,120,0,0);
    Vitesse->setValue(0);
    this->addItem(Vitesse);

    // //Compte tours
    QStringList graduations_rpm;
    graduations_rpm<<"  0"<<"  1"<<"  2"<<"  3"<<"  4"<<"  5"<<"  6"<<"  7"<<"  8";
    CompteTours = new hugo_Compteur(-125,0,90,210,30,graduations_rpm,800,100,1,0,64,128,6,120,0,0);
    // graduations_rpm<<"  0"<<" 50"<<"100"<<"150"<<"200"<<"250"<<"300"<<"350"<<"400"<<"450"<<"500";
    // graduations_rpm<<"  0"<<"100"<<"200"<<"300"<<"400"<<"500"<<"600"<<"700"<<"800"<<"900"<<"1000";
    // CompteTours = new hugo_Compteur(-125,0,90,210,0,graduations_rpm,1000,100,1,0,64,128);
    CompteTours->setValue(0);
    this->addItem(CompteTours);

    //Jauge température huile moteur
    QStringList graduations_temperature;
    graduations_temperature << " C" << " H";
    Temperature = new hugo_Compteur(-75,40,25,-45,45,graduations_temperature,100,25,-1,0,64,128);
    Temperature->setValue(0);
    this->addItem(Temperature);

    //Voyant en cas de problème avec la batterie
    icon_path = ros::package::getPath("avos_monitor")+"/resources/Icones/batterie.jpg";
    VoyantBatterie = new hugo_voyants_simples(-145, -125, QString::fromStdString(icon_path));
    this->addItem(VoyantBatterie);

    //Clignotants
    Clignotant = new hugo_voyants_clignotant;
    this->addItem(Clignotant);

    //Warnings
    Warning = new hugo_voyant_warning;
    Warning->setValue(0);
    this->addItem(Warning);
}

