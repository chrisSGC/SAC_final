
#include <Arduino.h>
#include "MyOledViewInitialisation.h"
using namespace std;

void MyOledViewInitialisation::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

void MyOledViewInitialisation::setIdDuSysteme(std::string value){
    idDuSysteme = value;
}

void MyOledViewInitialisation::setSensibiliteBoutonAction(std::string value){
    SensibiliteBoutonAction = value;
}

void MyOledViewInitialisation::setSensibiliteBoutonReset(std::string value){
    SensibiliteBoutonReset = value;
}

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
void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}