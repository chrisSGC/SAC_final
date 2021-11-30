#include <Arduino.h>
#include "MyOledViewWorkingCOLD.h"
#include "MyOledView.h"
using namespace std;

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
    //adafruit->setCursor(95, 35);
    adafruit->setTextSize(1);
    displayGifFire(adafruit, 95, 35);
    //adafruit->println(getTag("temperature").c_str());
    adafruit->setCursor(35, 55);
    adafruit->println(getTag("ip").c_str());
    adafruit->display();
}

void MyOledViewWorkingCOLD::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}