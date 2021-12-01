/**
    VUE pour le Oled quand le systeme s'initialise
    @file MyOledViewInitialisation.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"
using namespace std;

// Défini le nom du systeme
void MyOledViewInitialisation::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

// Défini l'id' du systeme
void MyOledViewInitialisation::setIdDuSysteme(std::string value){
    idDuSysteme = value;
}

// Défini la sensibilité du bouton action
void MyOledViewInitialisation::setSensibiliteBoutonAction(std::string value){
    SensibiliteBoutonAction = value;
}

// Défini la sensibilité du bouton reset
void MyOledViewInitialisation::setSensibiliteBoutonReset(std::string value){
    SensibiliteBoutonReset = value;
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
void MyOledViewInitialisation::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    char strAction[128];
    char strReset[128];
    sprintf(strId, "ID: %s", idDuSysteme.c_str());
    sprintf(strAction, "Bouton ACTION: %s", SensibiliteBoutonAction.c_str());
    sprintf(strReset, "Bouton RESET: %s", SensibiliteBoutonReset.c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    adafruit->println("INITIALISATION");
    adafruit->println(strAction);
    adafruit->println(strReset);
    adafruit->display();
}

// Met à jour les données en ré-appelant la méthode display
void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}