/**
    VUE pour le Oled quand l'ESP se connecte au Wi-Fi ou passe en mode Point d'accès
    @file MyOledViewWifiAp.h 
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
    
    Historique des versions   
        Versions    Date            Auteur      Description
        1.0         01-Nov-2021     C.F         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Adafruit_SSD1306
        MyOledView
**/
#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewWifiAp : public MyOledView {
    public:
        void setNomDuSysteme(std::string value);
        void setSsIDDuSysteme(std::string value);
        void setPassDuSysteme(std::string value);

    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomDuSysteme;
        std::string ssIDDuSysteme;
        std::string passDuSysteme;
};
#endif