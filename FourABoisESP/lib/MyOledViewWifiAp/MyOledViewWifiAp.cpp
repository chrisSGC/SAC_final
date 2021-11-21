#include <Arduino.h>
#include "MyOledViewWifiAp.h"
using namespace std;

void MyOledViewWifiAp::setNomDuSysteme(std::string value){
    nomDuSysteme = value;
}

void MyOledViewWifiAp::setSsIDDuSysteme(std::string value){
    ssIDDuSysteme = value;
}

void MyOledViewWifiAp::setPassDuSysteme(std::string value){
    passDuSysteme = value;
}

void MyOledViewWifiAp::display(Adafruit_SSD1306 *adafruit){
    char strId[128];
    char strSSID[128];
    char strPass[128];
    sprintf(strId, "ID: %s", nomDuSysteme.c_str());
    sprintf(strSSID, "SSID: %s", ssIDDuSysteme.c_str());
    sprintf(strPass, "PASS: %s", passDuSysteme.c_str());
    /*sprintf(strSSID, "    SSID: %s", ssIDDuSysteme.c_str());
    sprintf(strPass, "    PASS: %s", passDuSysteme.c_str());*/
    
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
void MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}