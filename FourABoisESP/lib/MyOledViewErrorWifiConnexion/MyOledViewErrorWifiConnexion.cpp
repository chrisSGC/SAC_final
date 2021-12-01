/**
    VUE pour le Oled quand la connexion au Wi-Fi échoue
    @file MyOledViewErrorWifiConnexion.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewErrorWifiConnexion.h"
using namespace std;

// Défini le nom du systeme
void MyOledViewErrorWifiConnexion::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

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
void MyOledViewErrorWifiConnexion::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    sprintf(strId, "ID: %s", nomDuSysteme.c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    adafruit->println("Erreur de connexion");
    adafruit->display();
}

// Met à jour les données en ré-appelant la méthode display
void MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}