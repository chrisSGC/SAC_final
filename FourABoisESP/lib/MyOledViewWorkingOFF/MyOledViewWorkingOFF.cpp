/**
    VUE pour le Oled quand le four est à l'arret
    @file MyOledViewWorkingOFF.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewWorkingOFF.h"
#include "MyOledView.h"
using namespace std;

// Met en forme l'affichage de la vue
/**
 * Met en forme l'affichage de la vue
 * 
 * clearDisplay = vide l'écrna de tout ce qui était affiché
 * setCursor(x, y) = positionne le texte
 * println = écrit sur l'écran
 * setTextSize = determine la taille du texte
 * display = affiche sur l'écran ce qui a été défini depuis le clearDisplay
 * */
void MyOledViewWorkingOFF::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    sprintf(strId, "ID: %s", getTag("idEcran").c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(getTag("nomSystem").c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    adafruit->setCursor(95, 25);
    adafruit->println("Ready");
    adafruit->setCursor(95, 35);
    adafruit->println(getTag("temperature").c_str());
    adafruit->setCursor(35, 50);
    adafruit->println(getTag("ip").c_str());
    adafruit->display();
}