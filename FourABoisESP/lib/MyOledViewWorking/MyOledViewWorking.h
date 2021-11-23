#ifndef MyOledViewWorking_H
#define MyOledViewWorking_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewWorking : public MyOledView {
    public:
        void init(std::string _id);

    protected:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
        void displayGifFire(Adafruit_SSD1306 *adafruit, int positionX, int positionY);
        void displayGifFirAnimated(Adafruit_SSD1306 *adafruit, int positionX, int positionY);

        unsigned char *PushMan24x48Pointers[7];
        int indexFire;
        int indexFireDelay;
};
#endif