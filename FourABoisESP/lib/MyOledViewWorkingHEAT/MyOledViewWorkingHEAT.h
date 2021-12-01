/**
    Class MyOledViewWorkingHEAT : VUE pour le Oled quand le four est lancé et chauffe
    @file MyOledViewWorkingHEAT.h 
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
        MyOledViewWorking
**/
#ifndef MyOledViewWorkingHEAT_H
#define MyOledViewWorkingHEAT_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"

class MyOledViewWorkingHEAT : public MyOledViewWorking {
    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif