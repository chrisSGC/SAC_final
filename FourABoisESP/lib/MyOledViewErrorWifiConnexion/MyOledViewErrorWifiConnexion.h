
#ifndef MyOledViewErrorWifiConnexion_H
#define MyOledViewErrorWifiConnexion_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewErrorWifiConnexion : public MyOledView {
    public:
        void setNomDuSysteme(std::string value);

    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomDuSysteme;
};
#endif