#include <Arduino.h>
#include "MyOledViewErrorWifiConnexion.h"
using namespace std;

void MyOledViewErrorWifiConnexion::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

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

void MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}