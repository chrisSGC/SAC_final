#include <Arduino.h>
#include "MyOledViewWorking.h"
//#include "MyOledView.h"
using namespace std;

void MyOledViewWorking::init(std::string _id){
    //MyOledView myOledView;
    setParams("idEcran", _id);
    setParams("nomSystem", "SAC System");
    
    Fire24x24Pointers[0] = const_cast<unsigned char *>(&Fire24x24_1[0]);
    Fire24x24Pointers[1] = const_cast<unsigned char *>(&Fire24x24_2[0]);
    Fire24x24Pointers[2] = const_cast<unsigned char *>(&Fire24x24_3[0]);
    Fire24x24Pointers[3] = const_cast<unsigned char *>(&Fire24x24_4[0]);
    Fire24x24Pointers[4] = const_cast<unsigned char *>(&Fire24x24_5[0]);
    Fire24x24Pointers[5] = const_cast<unsigned char *>(&Fire24x24_6[0]);
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

void MyOledViewWorking::displayGifFire(Adafruit_SSD1306 *adafruit, int positionX, int positionY){
    adafruit->drawBitmap(positionX, positionY, Fire24x24Pointers[5], positionX + 24, positionY - 12, WHITE);
}