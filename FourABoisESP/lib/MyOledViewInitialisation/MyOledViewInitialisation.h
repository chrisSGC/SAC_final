
#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include <Adafruit_SSD1306.h>
#include "MyOledView.h"

class MyOledViewInitialisation : public MyOledView {
    public:
        void setNomDuSysteme(std::string value);
        void setIdDuSysteme(std::string value);
        void setSensibiliteBoutonAction(std::string value);
        void setSensibiliteBoutonReset(std::string value);

    private:
        void display(Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);

        std::string nomDuSysteme;
        std::string idDuSysteme;
        std::string SensibiliteBoutonAction;
        std::string SensibiliteBoutonReset;
};
#endif