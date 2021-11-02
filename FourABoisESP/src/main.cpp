/* Copyright (C) 2021 Alain Dube
 * All rights reserved.
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Alain Dubé
    @version  1.1 21/08/15 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain       Première version du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
                      
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les focntionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)

    Autres librairies (à copier dans le répertoire lib)
         WifiManagerDevelopment
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
    
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
         
        MyServer                        V1.0    Pour la gestion des routes pour le site WEB
            /data                               Répertoire qui contient les fichiers du site WEB 
                index.html              V1.0    Page index du site WEB
                index.css               V1.0    CSS
                script.js               V1.0    JS (fonctions JavaScript)
              
 * */
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

WiFiManager wm;
#define WEBSERVER_H

MyServer *serveur = NULL; // Serveur sur l'ESP32

//Variable pour la connection Wifi
const char *SSID = "SAC_CHRIS_";
const char *PASSWORD = "sac_";
String ssIDRandom;

float temperatureMin;
float temp;

// SENSEUR DE TEMPERATURE
#include "TemperatureStub.h"
#define DHTPIN  15   // Pin utilisée par le senseur DHT22
#define DHTTYPE DHT22  // Type de senseur utilisé: ici un DHT 22
TemperatureStub *temperatureStub = NULL;

// Information sur la DEL
#define GPIO_PIN_LED_ROUGE 12 // La DEL est branchee sur le GPIO 12

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
        temperatureMin = ::atof(arg1.c_str());
        Serial.print("Température a depasser : ");
        Serial.println(arg1.c_str());

        return(String("Ok").c_str());
    }else if(string(actionToDo.c_str()).compare(string("obtenirInfosFour")) == 0) {
        return(String(temp).c_str());
    }else if(string(actionToDo.c_str()).compare(string("lancerFour")) == 0) {
        return(String(temp).c_str());
    }
   
    std::string result = "";
    return result;
}

void setup() { 
    Serial.begin(9600);
    delay(100);

    // On initialise les deux temepratures a 0
    temp = 0;
    temperatureMin = 0;

	// Initialisation de la DEL Rouge
	pinMode(GPIO_PIN_LED_ROUGE, OUTPUT);

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

    // Affiche une erreur en cas d'echec
	if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        Serial.println("Erreur de connexion.");
    } else {
        Serial.println("Connexion Établie.");
    }

    // ----------- Routes du serveur ----------------
    serveur = new MyServer(80);
    serveur->initAllRoutes();
    serveur->initCallback(&CallBackMessageListener);
}

void loop() {
	// Recuperation de la température
    temp = temperatureStub->getTemperature();

	if(temp > temperatureMin){ // Si la temperature est superieure a la temperature entree par l'utilisateur, on allume la DEL. Strictement superieure car dans le sujet il est ecrit " lorsque la température est supérieure à une certaine valeur en Celsius." et non "supérieure ou égale"
        digitalWrite(GPIO_PIN_LED_ROUGE, HIGH); // On allume la DEL car la temperature est superieure a la temperature entree par l'utilisateur
	}else{ 
        digitalWrite(GPIO_PIN_LED_ROUGE, LOW); // On coupe la DEL car la temperature est inferieure ou egale a la temperature entree par l'utilisateur
	}
}