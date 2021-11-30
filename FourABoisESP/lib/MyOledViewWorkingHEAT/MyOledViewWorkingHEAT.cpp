#include <Arduino.h>
#include "MyOledViewWorkingHEAT.h"
#include "MyOledView.h"
using namespace std;

void MyOledViewWorkingHEAT::display(Adafruit_SSD1306 *adafruit){

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
    adafruit->println("Heating");
    adafruit->setCursor(10, 35);
    adafruit->setTextSize(2);
    adafruit->println(getTag("temperature").c_str());
    displayGifFireAnimated(adafruit, 95, 35);
    //adafruit->setCursor(95, 35);
    adafruit->setTextSize(1);
    //adafruit->println(getTag("temperature").c_str());
    adafruit->setCursor(35, 55);
    adafruit->println(getTag("ip").c_str());
    adafruit->display();
}

void MyOledViewWorkingHEAT::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}