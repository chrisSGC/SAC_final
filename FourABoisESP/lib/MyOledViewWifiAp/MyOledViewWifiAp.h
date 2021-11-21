
#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewWifiAp : public MyOledView {
    public:
        void setNomDuSysteme(std::string value);
        void setSsIDDuSysteme(std::string value);
        void setPassDuSysteme(std::string value);

    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomDuSysteme;
        std::string ssIDDuSysteme;
        std::string passDuSysteme;
};
#endif