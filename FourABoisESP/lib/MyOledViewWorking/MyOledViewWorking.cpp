#include <Arduino.h>
#include "MyOledViewWorking.h"
//#include "MyOledView.h"
using namespace std;

void MyOledViewWorking::init(std::string _id){
    //MyOledView myOledView;
    setParams("idEcran", _id);
    setParams("nomSystem", "SAC System");
    //setParams("ip", "192.168.210.211");
    //setParams("temperature", "23.00");
}

void MyOledViewWorking::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    sprintf(strId, "ID: %s", getTag("idEcran").c_str());
    
    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);

    adafruit->println(getTag("nomSystem").c_str());
    adafruit->setTextSize(1);
    adafruit->println("");
    adafruit->println(strId);
    adafruit->println("Erreur de connexion");
    adafruit->display();
}
void MyOledViewWorking::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}