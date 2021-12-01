/**
    Classe qui gere les différentes vues de l'état de fonctionnement du four
    @file MyOledViewErrorWifiConnexion.cpp
    @author Christophe Ferru
    @version 1.0 01-Nov-2021 
*/
#include <Arduino.h>
#include "MyOledViewWorking.h"
//#include "MyOledView.h"
using namespace std;

// Initialisation de la classe, on détermine les pointeurs du gif et on met en form les params qui seront toujours là à saovir l'id et le nom du system
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

// Met en forme l'affichage de la vue
/**
 * Met en forme l'affichage de la vue
 * 
 * clearDisplay = vide l'écrna de tout ce qui était affiché
 * setCursor(x, y) = positionne le texte
 * println = écrit sur l'écran
 * setTextSize = determine la taille du texte
 * display = affiche sur l'écran ce qui a été défini depuis le clearDisplay
 * */
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

// Met à jour les données en ré-appelant la méthode display
void MyOledViewWorking::update(Adafruit_SSD1306 *adafruit){
    display(adafruit);
}

// Permet d'afficher le gif de feu de maniere immobile
void MyOledViewWorking::displayGifFire(Adafruit_SSD1306 *adafruit, int positionX, int positionY){
    adafruit->drawBitmap(positionX, positionY, Fire24x24Pointers[0], 24, 12, WHITE);
}

// Permet d'afficher le feu animé. Actuellement le feu n'est pas animé.
void MyOledViewWorking::displayGifFireAnimated(Adafruit_SSD1306 *adafruit, int positionX, int positionY){
    int indexFeu = 0;
    //int lastFramePosition = 12;
    //int posX = 0;
    //int posY = 20;
    for (int positionXs = -128; positionXs <= positionX; positionXs++) {
        adafruit->drawBitmap(positionX, positionY, Fire24x24Pointers[(++indexFeu) % 6], 24, 12, WHITE);
        //delay(5);
    }
    //adafruit->drawBitmap(positionX, positionY, Fire24x24Pointers[5], positionX + 24, positionY - 12, WHITE);
}