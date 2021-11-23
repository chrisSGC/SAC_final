#include <Arduino.h>
#include "MyOledViewWorkingOFF.h"
#include "MyOledView.h"
using namespace std;

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
    // change position
    adafruit->setCursor(95, 25);
    adafruit->println("Ready");
    adafruit->setCursor(95, 35);
    adafruit->println(getTag("temperature").c_str());
    adafruit->setCursor(35, 50);
    adafruit->println(getTag("ip").c_str());
    adafruit->display();
}