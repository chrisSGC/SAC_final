/* Par Christophe Ferru
    All rights reserved.

    Projet Sac
    Cours Objets connectés - 2021

    @file     main.cpp
    @author   Christophe Ferru
    @version  1.0 - 30 Novembre 2021

    Historique des versions
           Version    Date          Auteur      Description
           1.0        30-11-2021    C.F         Première version totalement fonctionnelle du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps =         
        ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
        AsyncTCP-esphome                            (Pour utiliser les fonctionnalités TCP)
        bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)

    Autres librairies (à copier dans le répertoire lib)
        WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
        Arduino_GFX
        Arduino_GrayOLED
        Arduino_I2C
        Arduino_SPITFT
        Arduino_SSD1306
        DHT
        gfxfont
        MyButton - Par Alain Dubé
        myLib - Par Alain Dubé
        MyOled - Par Alain Dubé
        MyOledView - Par Alain Dubé
        MyOledViewErrorWifiConnexion
        MyOledViewInitialisation
        MyOledViewWifiAp
        MyOledViewWorking
        MyOledViewWorkingCOLD
        MyOledViewWorkingHEAT
        MyOledViewWorkingOFF
        MyServer - Par Alain Dubé
        Splash
        TemperatureStub - Par Alain Dubé
    
    Fonctions utiles (utilitaires)
        /lib/MYLIB/myFunctions.cpp
            //Pour vérifier plus simplement que deux chaines sont identiques
            bool isEqualString(std::string line1, std::string line2)
            //Pour extraire une partie d'une chaine de caractères avec l'aide d'un index
            std::string getValue(std::string data, char separator, int index)
            //Pour remplacer plus facilement une sous chaine
            bool replaceAll(std::string& source, const std::string& from, const std::string& to)
            //Pour obtenir un chaine aléatoire d'une certaine longeur
            std::string get_random_string(unsigned int len)

    Classes du système
        MyServer                                V1.0    Pour la gestion des routes pour le site WEB
            /data                                       Répertoire qui contient les fichiers du site WEB 
                index.html                      V1.0    Page index du site WEB
                application.html                V1.0    Page de l'application du site WEB
                /css
                    signin.css                  V1.0    CSS de la connexion
                    style.css                   V1.0    CSS de l'application
                /script
                    connexion.js                V1.0    JS (fonctions JavaScript) de la connexion
                    app.js                      V1.0    JS (fonctions JavaScript) de l'application
                /images
                    logoSAC.png                 V1.0    Logo de l'application
        MyOled                                          Pour la gestion de l'écran
        MyOledView                                      Pour la gestion des vues de l'écran
        MyOledViewInitialisation                        Vue d'initialisation
        MyOledViewWifiAp                                Vue point d'acces
        MyOledViewErrorWifiConnexion                    Vue en cas d'erreur de wifi
        MyOledViewWorkingOFF                            Vue quand le four n'est pas en marche
        MyOledViewWorkingCOLD                           Vue quand le four n'est pas assez chaud
        MyOledViewWorkingHEAT                           Vue quand le four est en fonction et chauffe
        TemperatureStub                                 Classe qui permet la gestion de la prise de température - Par Alain Dubé
        MyButton                                        Classe qui gère les boutons - Par Alain Dubé
*/

// Using nécessaires au projet
using namespace std;

// Include des classes nécessaires au projet
#include <iostream>
#include <string>
#include <Arduino.h>
#include <ArduinoJson.h>
#include "myFunctions.cpp" //fonctions utilitaires
#include <HTTPClient.h>
#include <WiFiManager.h>
#include "MyServer.h"
#include "MyOled.h"
#include "MyOledView.h"
#include "MyOledViewInitialisation.h"
#include "MyOledViewWifiAp.h"
#include "MyOledViewErrorWifiConnexion.h"
#include "MyOledViewWorkingOFF.h"
#include "MyOledViewWorkingCOLD.h"
#include "MyOledViewWorkingHEAT.h"
#include <wire.h>
#include "TemperatureStub.h"
#include "MyButton.h"

WiFiManager wm;
#define WEBSERVER_H
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C de l'écran Oled
#define DHTPIN  15 // Pin utilisée par le senseur DHT22
#define DHTTYPE DHT22 // Type de senseur utilisé: ici un DHT 22
#define GPIO_PIN_DEL_ROUGE 12 // La DEL rouge est branchee sur le GPIO 12
#define GPIO_PIN_DEL_VERT 14 //La DEL verte est branchée sur le GPIO14
#define GPIO_PIN_DEL_JAUNE 27 // La DEL jaune est branchée sur le GPIO27

MyServer *serveur = NULL; // Serveur sur l'ESP32
MyOled *ecran = NULL;
MyOledViewInitialisation *vueInitialisation = NULL;
MyOledViewWifiAp *vueAP = NULL;
MyOledViewErrorWifiConnexion *vueWFErreur = NULL;
MyOledViewWorkingOFF *vueOff = NULL;
MyOledViewWorkingCOLD *vueCold = NULL;
MyOledViewWorkingHEAT *vueHeat = NULL;
TemperatureStub *temperatureStub = NULL;
MyButton *myButtonAction = NULL;
MyButton *myButtonReset = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_CHRIS_";
const char *PASSWORD = "sac_";
String ssIDRandom;

float temperatureActuelle;
int dureeActuelle;
bool etatFour;

// DONNEES LIEES AU BOIS
int dureeNecessaire;
float tempMiniBois;
String nomBois;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message){
    while(replaceAll(message, std::string("  "), std::string(" ")));

    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    string arg2 = getValue(message, ' ', 2);
    string arg3 = getValue(message, ' ', 3);
    string arg4 = getValue(message, ' ', 4);
    string arg5 = getValue(message, ' ', 5);
    string arg6 = getValue(message, ' ', 6);
    string arg7 = getValue(message, ' ', 7);
    string arg8 = getValue(message, ' ', 8);
    string arg9 = getValue(message, ' ', 9);
    string arg10 = getValue(message, ' ', 10);
  
    if(string(actionToDo.c_str()).compare(string("action")) == 0){
        return(String("Ok").c_str());
    }else if(string(actionToDo.c_str()).compare(string("definirTypeBois")) == 0) {
        if(!etatFour){
            tempMiniBois = ::atof(arg1.c_str());
            dureeNecessaire = ::atoi(arg2.c_str());
            nomBois = arg3.c_str();
        }
        return(String("Ok").c_str());
    }else if(string(actionToDo.c_str()).compare(string("obtenirInfosFour")) == 0) {
        String a = String(temperatureActuelle).c_str();
        String b = String(etatFour).c_str();
        String contentPOST = "{\"code\": 200, \"donnees\": {\"etatFour\":"+b+", \"temperatureActuelle\":"+a+",\"idBois\":\""+nomBois+"\",\"tempMiniBois\":"+String(tempMiniBois).c_str()+",\"dureeNecessaire\":"+String(dureeNecessaire).c_str()+",\"dureeActuelle\":"+String(dureeActuelle).c_str()+"}}";
        return(contentPOST.c_str());
    }else if(string(actionToDo.c_str()).compare(string("lancerFour")) == 0) {
        dureeActuelle = 0;
        etatFour = !etatFour;
        String retour = (etatFour) ? "{\"code\": 200}" : "{\"code\": 400}";
        return(retour.c_str());
    }
   
    std::string result = "";
    return result;
}

void setup() { 
    Serial.begin(9600);
    delay(100);

    // Initialisation de 'lécran avec l'écran d'initialisation
	ecran = new MyOled(&Wire, OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);

    // Initialisation de l'ensemble des vues
	vueInitialisation = new MyOledViewInitialisation();
    vueAP = new MyOledViewWifiAp();
	vueCold = new MyOledViewWorkingCOLD();
	vueOff = new MyOledViewWorkingOFF();
	vueHeat = new MyOledViewWorkingHEAT();
    vueWFErreur = new MyOledViewErrorWifiConnexion();
	vueHeat->init("1245");
	vueOff->init("1245");
	vueCold->init("1245");
	ecran->init(OLED_I2C_ADDRESS, true);
    vueWFErreur->setNomDuSysteme("1245");
	vueInitialisation->setIdDuSysteme("1245");
	vueInitialisation->setNomDuSysteme("SAC System");
	vueInitialisation->setSensibiliteBoutonAction("????");
	vueInitialisation->setSensibiliteBoutonReset("????");

    // On affiche l'écran d'initialisation
	ecran->displayView(vueInitialisation);

    // On initilise les deux boutons puis on met leur valeur dans la variable associée
    myButtonAction = new MyButton();
    myButtonAction->init(T8);
    int sensibilisationButtonAction = myButtonAction->autoSensibilisation();

    myButtonReset = new MyButton();
    myButtonReset->init(T9);
    int sensibilisationButtonReset = myButtonReset->autoSensibilisation();

    // Mise à jour de l'initialisation avec les valeurs
	vueInitialisation->setSensibiliteBoutonAction(String(sensibilisationButtonAction).c_str());
	vueInitialisation->setSensibiliteBoutonReset(String(sensibilisationButtonReset).c_str());
	ecran->displayView(vueInitialisation);

    delay(500);

    // On initialise les deux temepratures a 0
    temperatureActuelle = 0;
    tempMiniBois = 0;
    dureeActuelle = 0;
    dureeNecessaire = 0;
    nomBois = "";
    etatFour = false;

	// Initialisation de la DEL Rouge,Verte et Jaune
	pinMode(GPIO_PIN_DEL_ROUGE, OUTPUT);
	pinMode(GPIO_PIN_DEL_VERT, OUTPUT);
	pinMode(GPIO_PIN_DEL_JAUNE, OUTPUT);

	// Initiation de la lecture de la température
    temperatureStub = new TemperatureStub;
    temperatureStub->init(DHTPIN, DHTTYPE); //Pin 15 et Type DHT22

    // Permet la connexion au WifiManager
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

    // Permet l'affichage des identifiants du wifi
	char strToPrint[128];
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom.c_str(), PASSRandom.c_str());
    Serial.println(strToPrint);

    // On met en place les informations de la vue Access point puis on affiche cette derniere
    vueAP->setNomDuSysteme("SAC System");
    vueAP->setSsIDDuSysteme(ssIDRandom.c_str());
    vueAP->setPassDuSysteme(PASSRandom.c_str());
    ecran->displayView(vueAP);

    // Affiche une erreur en cas d'echec
	if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        // affichage de l'écran d'acces AP
        Serial.println("Erreur de connexion.");
        ecran->displayView(vueWFErreur);
    } else {
        Serial.println("Connexion Établie.");
    }

    // ----------- Routes du serveur ----------------
    serveur = new MyServer(80);
    serveur->initAllRoutes();
    serveur->initCallback(&CallBackMessageListener);

    // On fait clignoter toutes les DELs 2 fois
    for(int a = 0; a < 2; a++){
        digitalWrite(GPIO_PIN_DEL_ROUGE, HIGH);
        digitalWrite(GPIO_PIN_DEL_VERT, HIGH);
        digitalWrite(GPIO_PIN_DEL_JAUNE, HIGH);
        delay(500);
        digitalWrite(GPIO_PIN_DEL_ROUGE, LOW);
        digitalWrite(GPIO_PIN_DEL_VERT, LOW);
        digitalWrite(GPIO_PIN_DEL_JAUNE, LOW);
        delay(500);
    }

    // une fois que tout est prêt, on allume la Del verte pour indiquer que le four est prêt à être utilisé
    digitalWrite(GPIO_PIN_DEL_VERT, HIGH);

    // On ajoute le parametre IP a toutes les vues qui en ont besoin
    vueOff->setParams("ip", WiFi.localIP().toString().c_str());
    vueCold->setParams("ip", WiFi.localIP().toString().c_str());
    vueHeat->setParams("ip", WiFi.localIP().toString().c_str());
}

/**
 * Permet de gérer l'allumage des DELs d'un même endroit en fonction de l'état du four, de la durée et de la température
 * 
 * Si le four est lancé est que la cuisson n'est pas terminée: DEL rouge allumé uniquement (four vérouillé)
 * Si le four est allumé et que la durée est terminée: DEL Verte allumée (four ouvert)
 * Si le four est allumé, que la cuisson n'est pas terminée et que la température est inférieure à la température mini: DELs rouge et jaune allumées
 * Dans les autres cas: aucune DEL n'est allumée
 * */
void allumerDelEtatFour(){
    if(etatFour && (dureeActuelle < dureeNecessaire)){
        digitalWrite(GPIO_PIN_DEL_ROUGE, HIGH);
        digitalWrite(GPIO_PIN_DEL_VERT, LOW);
        digitalWrite(GPIO_PIN_DEL_JAUNE, LOW);
    }else if(etatFour && (dureeActuelle >= dureeNecessaire)){
        digitalWrite(GPIO_PIN_DEL_ROUGE, LOW);
        digitalWrite(GPIO_PIN_DEL_VERT, HIGH);
        digitalWrite(GPIO_PIN_DEL_JAUNE, LOW);
    }else if(etatFour && (dureeActuelle < dureeNecessaire) && (temperatureActuelle < tempMiniBois)){
        digitalWrite(GPIO_PIN_DEL_ROUGE, HIGH);
        digitalWrite(GPIO_PIN_DEL_VERT, LOW);
        digitalWrite(GPIO_PIN_DEL_JAUNE, HIGH);
    }else{
        digitalWrite(GPIO_PIN_DEL_ROUGE, LOW);
        digitalWrite(GPIO_PIN_DEL_VERT, LOW);
        digitalWrite(GPIO_PIN_DEL_JAUNE, LOW);
    }
}

void loop() {
    // Initialisation de la véirfication des boutons, sont-ils appuyés?
    int buttonReset = myButtonReset->checkMyButton();
    int buttonAction = myButtonAction->checkMyButton();

    if(buttonReset > 300)  {  // Si le bouton est appuyé plus de 300ms, on réinitilise l'ensemble du systeme: ESP et WiFi Manager
        Serial.println("Button Reset pressed\n");
        Serial.println("Button Hard reset pressed\n");
        Serial.println("Suppression des réglages et redémarrage...\n");
        wm.resetSettings();
        ESP.restart();
    }

    if(buttonAction > 300){ // Si le bouton d'action est appuyé plus de 300ms, on met l'écran en veille ou, si il l'est déjà, on le sort de son état de veille
        Serial.println("Button action pressed\n");
        if(ecran->veilleCheck()){
            ecran->veilleExit();
        }else{
            ecran->veilleDelay(30);
        }
    }

	// Recuperation de la température
    temperatureActuelle = temperatureStub->getTemperature();

    // On met la température dans les params des vues qui en ont besoin
	vueOff->setParams("temperature", String(temperatureActuelle).c_str());
	vueCold->setParams("temperature", String(temperatureActuelle).c_str());
	vueHeat->setParams("temperature", String(temperatureActuelle).c_str());

    /* 
    * Si le four est activé, on vérifie la température et on affiche l'écran en fonction de la température apr rapport à la température minimale requise.
    */
    if(etatFour){
        // On allume la del rouge pour indiquer que la porte du four est fermée
        allumerDelEtatFour();

        if(temperatureActuelle > tempMiniBois){ // Si la temperature est superieure a la temperature entree par l'utilisateur, on allume la DEL. Strictement superieure car dans le sujet il est ecrit " lorsque la température est supérieure à une certaine valeur en Celsius." et non "supérieure ou égale"
            if(dureeActuelle < dureeNecessaire){
                digitalWrite(GPIO_PIN_DEL_JAUNE, HIGH); // On allume la DEL car la temperature est superieure a la temperature entree par l'utilisateur, le bois est donc en train d'être chauffé
	            ecran->displayView(vueHeat);

                //delay(250); // la loop est quasiment instantanée, donc on va afficher un delay de 0.998 secondes pour simuler le passage d'une seconde et ajouter 1 à la durée actuelle
                dureeActuelle++;
            }else{ // Si la durée est égale, on allume la del verte pour indiquer que le four est pret à être lancé
                allumerDelEtatFour();
                etatFour = !etatFour;
            }
        }else{ 
            digitalWrite(GPIO_PIN_DEL_JAUNE, LOW); // On coupe la DEL car la temperature est inferieure ou egale a la temperature entree par l'utilisateur
	        ecran->displayView(vueCold);
        }
    }else{
        // on affiche l'écran éteint
	    ecran->displayView(vueOff);
    }
}