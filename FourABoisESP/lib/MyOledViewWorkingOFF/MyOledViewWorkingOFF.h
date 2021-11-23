#ifndef MyOledViewWorkingOFF_H
#define MyOledViewWorkingOFF_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"

class MyOledViewWorkingOFF : public MyOledViewWorking {
    private:
        void display(Adafruit_SSD1306 *adafruit);
};
#endif