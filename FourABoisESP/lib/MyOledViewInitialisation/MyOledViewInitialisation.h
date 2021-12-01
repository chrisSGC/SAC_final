/**
    Class MyOledViewInitialisation : VUE pour le Oled quand le systeme s'initialise
    @file MyOledViewInitialisation.h 
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
#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewInitialisation : public MyOledView {
    public:
        void setNomDuSysteme(std::string value);
        void setIdDuSysteme(std::string value);
        void setSensibiliteBoutonAction(std::string value);
        void setSensibiliteBoutonReset(std::string value);

    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomDuSysteme;
        std::string idDuSysteme;
        std::string SensibiliteBoutonAction;
        std::string SensibiliteBoutonReset;
};
#endif