#ifndef MyOledViewWorkingCOLD_H
#define MyOledViewWorkingCOLD_H

#include <Adafruit_SSD1306.h>
#include "MyOledViewWorking.h"

class MyOledViewWorkingCOLD : public MyOledViewWorking {
    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
};
#endif