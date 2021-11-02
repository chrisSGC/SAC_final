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
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
}

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
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
    });

    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/application.html", "text/html");
    });

    //Route du script JavaScript
    this->on("/script/app.js", HTTP_GET, [](AsyncWebServerRequest *request) {
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
    });

    this->on("/connexion", HTTP_GET, [](AsyncWebServerRequest *request) {
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

        /*String tempToSend;
        StaticJsonDocument<2048> doc;
        deserializeJson(doc, response);
        JsonObject obj1 = doc.as<JsonObject>();
        std::string wood;
        String  woodName;
      
        for (JsonPair kv1 : obj1) {
            wood = kv1.key().c_str();
            Serial.print("Element : ");Serial.println(wood.c_str());

            JsonObject elem = obj1[wood];
            woodName = elem["nom"].as<String>();
            if(tempToSend!="") tempToSend += "&";
            tempToSend +=  String(wood.c_str()) + String("&") + String(woodName.c_str());
           
            Serial.print(woodName);Serial.print(" ");*/
                          
            //Pour parcourir les éléments de l'objet
            //for (JsonPair kv2 : elem) {
            //    Serial.print("   Sous element : ");Serial.print(kv2.key().c_str());
            //    Serial.print("    :  ");Serial.println(kv2.value().as<char*>());
            //    }
            //}
        
        //request->send(200, "text/plain", tempToSend);
        request->send(200, "text/plain", response);
    });
    
    this->on("/obtenirBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois")){
            HTTPClient http;
            //AsyncWebParameter* p = request->getParam("idBois");
            String paramIdBois = request->getParam("idBois")->value().c_str();
            String woodApiRestAddress = "http://172.16.210.7:3000/api/obtenirBois/"+paramIdBois;
            http.begin(woodApiRestAddress);
            http.GET();
            String response = http.getString();

            request->send(200, "text/plain", response);
            /*Serial.println(paramIdBois);
            Serial.println(response);*/
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });

    /*this->on("/listeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        String woodApiRestAddress = "http://172.16.210.7:3000/api/bois";
        http.begin(woodApiRestAddress);
        http.GET();
        String response = http.getString();
        request->send(200, "text/plain", response);
    });*/
   
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
    });
    
    this->begin();
};