/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
#include <ArduinoJson.h>
using namespace std;

/********************************
 * TODO: ajouter la vérificaiton d'existence de compteur
 * TODO: ajouter le statut au retour d'api
 * TODO: traiter ce statut
 * 
 * */

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

    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/index.html", "text/html"); });

    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/application.html", "text/html"); });

    this->on("/connexion", HTTP_POST, [](AsyncWebServerRequest *request) {
        if((request->hasParam("nomCompte", true)) && (request->hasParam("motDePasse", true))){
            HTTPClient http;
            String apiSAC = "http://172.16.210.7:3000/api/connexion";

            http.begin(apiSAC);
            http.addHeader("Accept", "application/json");
            http.addHeader("Content-Type", "application/json");

            AsyncWebParameter* paramNomCompte = request->getParam("nomCompte", true);
            String a = paramNomCompte->value().c_str();
            AsyncWebParameter* paramMotDePasse = request->getParam("motDePasse", true);
            String b = paramMotDePasse->value().c_str();
            String contentPOST = "{\"nomCompte\":\""+a+"\",\"motDePasse\":\""+b+"\"}";

            http.POST(contentPOST.c_str());

            String response = http.getString();

            request->send(200, "text/plain", response);
        }else{
            request->send(404, "text/plain", "Impossible");
        }
    });

    this->on("/listeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String apiSAC = "http://172.16.210.7:3000/api/bois";
        http.begin(apiSAC);
        http.GET();
        String response = http.getString();

        request->send(200, "text/plain", response);
    });
    
    this->on("/obtenirNomBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois") && request->hasParam("token")){
            HTTPClient http;

            String paramIdBois = request->getParam("idBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            String apiSAC = "http://172.16.210.7:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;

            http.begin(apiSAC);
            http.GET();

            String response = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);

            String donnees = doc["donnees"];

            DynamicJsonDocument docDeux(1024);
            deserializeJson(docDeux, donnees);
            String nom = docDeux["nom"];
            String contentPOST = "{\"code\": 200, \"donnees\": {\"nom\":"+nom+"}}";

            request->send(200, "text/plain", contentPOST.c_str());
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });
    
    this->on("/obtenirBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois") && request->hasParam("token")){
            HTTPClient http;

            String paramIdBois = request->getParam("idBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            String apiSAC = "http://172.16.210.7:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;

            http.begin(apiSAC);
            http.GET();

            String response = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);

            std::string code = doc["code"];
            String donnees = doc["donnees"];

            DynamicJsonDocument docDeux(1024);
            deserializeJson(docDeux, donnees);
            std::string temperatureSechage = docDeux["temperature"];
            std::string dureeSechage = docDeux["sechage"];
            std::string nom = docDeux["nom"];
            std::string id = docDeux["id"];

            std::string repString = "";
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("definirTypeBois "+temperatureSechage+" "+dureeSechage+" "+id); //Exemple pour appeler une fonction CallBack
            //String resultatTemperature = String(repString.c_str());

            request->send(200, "text/plain", response);
        }else if(request->hasParam("nomBois") && request->hasParam("token")){
            HTTPClient http;

            String paramNomBois = request->getParam("nomBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            String apiSAC = "http://172.16.210.7:3000/api/obtenirBoisNom/"+paramToken+"/"+paramNomBois;

            http.begin(apiSAC);
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

    this->on("/definirTypeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if((request->hasParam("idBois") && request->hasParam("token"))){
            HTTPClient http;
            String paramIdBois = request->getParam("idBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            String apiSAC = "http://172.16.210.7:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;
            
            http.begin(apiSAC);
            http.GET();

            String response = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);

            String donnees = doc["donnees"];

            DynamicJsonDocument docDeux(1024);
            deserializeJson(docDeux, donnees);
            std::string temperatureSechage = docDeux["temperature"];
            std::string dureeSechage = docDeux["sechage"];
            std::string nom = docDeux["nom"];

            std::string repString = "";
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("definirTypeBois "+temperatureSechage+" "+dureeSechage+" "+nom); //Exemple pour appeler une fonction CallBack
            String resultatTemperature = String(repString.c_str());

            request->send(200, "text/plain", response);
        }else{
            request->send(404, "text/plain", "Not Found");
        }
    });

    this->on("/lancerFour", HTTP_POST, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("lancerFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });
   
    this->onNotFound([](AsyncWebServerRequest *request){ request->send(404, "text/plain", "Page Not Found"); });
    
    this->begin();
};