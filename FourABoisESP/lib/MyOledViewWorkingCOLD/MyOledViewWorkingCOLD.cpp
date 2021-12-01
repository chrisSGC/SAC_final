/**
    VUE pour le Oled quand le four est lancé mais que la température est trop basse
    @file MyOledViewWorkingCOLD.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewWorkingCOLD.h"
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
void MyOledViewWorkingCOLD::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    sprintf(strId, "ID: %s", getTag("idEcran").c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(getTag("nomSystem").c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    // change position
    adafruit->setCursor(85, 25);
    adafruit->println("Waiting");
    adafruit->setCursor(10, 35);
    adafruit->setTextSize(2);
    adafruit->println(getTag("temperature").c_str());
    adafruit->setTextSize(1);
    displayGifFire(adafruit, 95, 35);
    adafruit->setCursor(35, 55);
    adafruit->println(getTag("ip").c_str());
    adafruit->display();
}

// Met à jour les données en ré-appelant la méthode display
void MyOledViewWorkingCOLD::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}