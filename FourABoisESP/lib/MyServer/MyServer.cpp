/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
using namespace std;

typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback){ ptrToCallBackFunction = callback; }

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // Permet d'envoyer l'ensemble des fichiers css et js en meme temps
    this->serveStatic("/", SPIFFS, "/assets/");

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/index.html", "text/html"); });

    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/application.html", "text/html"); });

    //Route du script JavaScript
   /* this->on("/script/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script/app.js", "text/javascript");
    });

    this->on("/css/signin.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/signin.css", "text/css");
    });

    this->on("/css/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/css/style.css", "text/css");
    });

    this->on("/images/logoSAC.PNG", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/images/logoSAC.PNG", "image/png");
    });*/

    this->on("/connexion", HTTP_GET, [](AsyncWebServerRequest *request) {
    //this->on("/connexion", HTTP_POST, [](AsyncWebServerRequest *request) {
        //HTTPClient http;
        Serial.println("ICI");
        /*String apiSAC = "http://172.16.210.7:3000/api/connexion";
        http.begin(apiSAC);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST("{\"nomCompte\":\""++"\",\"motDePasse\":\"BME280\"}");
        String response = http.getString();*/

        if(request->hasParam("nomCompte")){
            Serial.println("LA");
        }

        if(request->hasArg("nomCompte")){
            Serial.println("TDC");
        }
        /** POST FUNCTIONNALITY ACCORDING TO DOCUMENTATION
        if(request->hasParam("nomCompte", true)){
            Serial.println("LA");
        }

        if(request->hasArg("nomCompte", true)){
            Serial.println("TDC");
        }*/

        //Serial.println(request->getParam("nomCompte")->value().c_str());
        //Serial.println(request->getParam("motDePasse")->value().c_str());

        request->send(200, "text/plain", "12");
    });

    this->on("/listeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApiRestAddress = "http://172.16.210.7:3000/api/bois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();

        request->send(200, "text/plain", response);
    });
    
    this->on("/obtenirBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois")){
            HTTPClient http;

            String paramIdBois = request->getParam("idBois")->value().c_str();
            String woodApiRestAddress = "http://172.16.210.7:3000/api/obtenirBois/"+paramIdBois;

            http.begin(woodApiRestAddress);
            http.GET();

            String response = http.getString();

            request->send(200, "text/plain", response);
        }else if(request->hasParam("nomBois")){
            HTTPClient http;

            String paramNomBois = request->getParam("nomBois")->value().c_str();
            String woodApiRestAddress = "http://172.16.210.7:3000/api/obtenirBoisNom/"+paramNomBois;

            http.begin(woodApiRestAddress);
            http.GET();

            String response = http.getString();

            request->send(200, "text/plain", response);
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });

    this->on("/obtenirInfosFour", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("obtenirInfosFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });

    this->on("/definirTypeBois", HTTP_POST, [](AsyncWebServerRequest *request) {
        /**
         * 1/ obtenir infos du type de bois
         * 2/ Passer ces infos au main
         * 3/ f(type de retour) on envoi le statut d'etat approprié
         * 
         * */
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("obtenirInfosFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });

    this->on("/lancerFour", HTTP_POST, [](AsyncWebServerRequest *request) {
        /**
         * 1/ envoi de la requete au main
         * 2/ f(type de retour) on envoi le statut d'etat approprié
         * 
         * */
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("lancerFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });

    /*this->on("/listeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApiRestAddress = "http://172.16.210.7:3000/api/bois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();
        request->send(200, "text/plain", response);
    });*/
   
    this->onNotFound([](AsyncWebServerRequest *request){ request->send(404, "text/plain", "Page Not Found"); });
    
    this->begin();
};