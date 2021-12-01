/**
    VUE pour le Oled quand l'ESP se connecte au Wi-Fi ou passe en mode Point d'accès
    @file MyOledViewWifiAp.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewWifiAp.h"
using namespace std;

// Défini le nom du systeme
void MyOledViewWifiAp::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

// Défini le SSID du systeme
void MyOledViewWifiAp::setSsIDDuSysteme(std::string value){
    ssIDDuSysteme = value;
}

// Défini le mot de passe du systeme
void MyOledViewWifiAp::setPassDuSysteme(std::string value){
    passDuSysteme = value;
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
void MyOledViewWifiAp::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    char strSSID[128];
    char strPass[128];
    sprintf(strId, "ID: %s", nomDuSysteme.c_str());
    sprintf(strSSID, "SSID: %s", ssIDDuSysteme.c_str());
    sprintf(strPass, "PASS: %s", passDuSysteme.c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    adafruit->println("AP configuration");
    adafruit->setCursor(20, 45);
    adafruit->println(strSSID);
    adafruit->setCursor(20, 55);
    adafruit->println(strPass);
    adafruit->display();
}

// Met à jour les données en ré-appelant la méthode display
void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}