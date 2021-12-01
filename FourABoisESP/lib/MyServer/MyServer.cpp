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

    // Retourne la page de connexion du site
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/index.html", "text/html"); });

    // Retourne la page application du site
    this->on("/application", HTTP_GET, [](AsyncWebServerRequest *request){ request->send(SPIFFS, "/application.html", "text/html"); });

    /**
     * Permet de traiter le demande en POST sur le chemin /connexion
     * 
     * On fait l'appel à l'API avec les données recues si elles existent et on retourne le résultat de l'appel
     * 
     * Si les données n'existent pas, on renvoi un 404
     * */
    this->on("/connexion", HTTP_POST, [](AsyncWebServerRequest *request) {
        if((request->hasParam("nomCompte", true)) && (request->hasParam("motDePasse", true))){
            HTTPClient http;
            //String apiSAC = "http://172.16.210.7:3000/api/connexion";
            String apiSAC = "http://10.0.0.60:3000/api/connexion";

            http.begin(apiSAC);
            http.addHeader("Accept", "application/json");
            http.addHeader("Content-Type", "application/json");

            AsyncWebParameter* paramNomCompte = request->getParam("nomCompte", true);
            String nomCompte = paramNomCompte->value().c_str();
            AsyncWebParameter* paramMotDePasse = request->getParam("motDePasse", true);
            String motDePasse = paramMotDePasse->value().c_str();
            String contentPOST = "{\"nomCompte\":\""+nomCompte+"\",\"motDePasse\":\""+motDePasse+"\"}"; // On formate le json à la main car la librairie json fournie par alain génrere des erreurs dans l'API

            http.POST(contentPOST.c_str());

            String response = http.getString();

            request->send(200, "text/plain", response);
        }else{
            request->send(404, "text/plain", "Impossible");
        }
    });

    /**
     * Permet de récupérer la liste des bois en GET
     * 
     * On lance l'appel vers l'API et on retourne nos données
     * */
    this->on("/listeBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        HTTPClient http;
        //String apiSAC = "http://172.16.210.7:3000/api/bois";
        String apiSAC = "http://10.0.0.60:3000/api/bois";
        http.begin(apiSAC);
        http.GET();
        String response = http.getString();

        request->send(200, "text/plain", response);
    });
    
    /**
     * Route en GET qui permet de récupérer uniquement le nom du bois
     * 
     * 
     * On vérifie la présence du token et de l'id du bois, si ils sont présents, on lance l'appel vers l'api afin de récupérer les informations du bois en question. Suite à ca, on défait le json pour récupérer uniquement le nom et ne renvoyer que le nom.
     * */
    this->on("/obtenirNomBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois") && request->hasParam("token")){
            HTTPClient http;

            String paramIdBois = request->getParam("idBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            //String apiSAC = "http://172.16.210.7:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;
            String apiSAC = "http://10.0.0.60:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;

            http.begin(apiSAC);
            http.GET();

            String response = http.getString();

            DynamicJsonDocument doc(1024);
            deserializeJson(doc, response);

            String donnees = doc["donnees"];

            DynamicJsonDocument docDeux(1024);
            deserializeJson(docDeux, donnees);
            String nom = docDeux["nom"];
            String contentGET = "{\"code\": 200, \"donnees\": {\"nom\":"+nom+"}}";

            request->send(200, "text/plain", contentGET.c_str());
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });
    
    /**
     * Permet de vérifier l'existence d'un compte via un token.
     * 
     * On recoit un token par cette route, si il est défini, on lance l'appel vers l'API pour renvoyer le résultat.
     * */
    this->on("/verifierExistance", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("token")){
            HTTPClient http;

            String paramToken = request->getParam("token")->value().c_str();
            //String apiSAC = "http://172.16.210.7:3000/api/verifierExistence/"+paramToken+"/"+paramIdBois;
            String apiSAC = "http://10.0.0.60:3000/api/verifierExistence/"+paramToken;

            http.begin(apiSAC);
            http.GET();

            String response = http.getString();

            request->send(200, "text/plain", response);
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });
    
    /**
     * Route permettant d'obtenir les détails d'un bois.
     * 
     * GET
     * 
     * Vérifie la présence de l'id du bois et du token pour faire l'papel vers l'api. Une fois le résultat API recu, on défait le json pour envoyer certaines données au callback car le main en aura besoin (ex: temperature requise, duree requise).
     * 
     * On renvoi ensuite le json original au client
     * 
     * Cette route peut aussi traiter le cas ou l'on recoit un nom au lieu d'un id. Alain a fait mention rapidement que ce serait "mieux" mais je ne vois absolument pas dans quel cas ca serait mieux dnas le cadre de notre application actuell. Mais comme Alain l'a mentionné, c'est là: mais inutile.
     * */
    this->on("/obtenirBois", HTTP_GET, [](AsyncWebServerRequest *request) {
        if(request->hasParam("idBois") && request->hasParam("token")){
            HTTPClient http;

            String paramIdBois = request->getParam("idBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            //String apiSAC = "http://172.16.210.7:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;
            String apiSAC = "http://10.0.0.60:3000/api/obtenirBois/"+paramToken+"/"+paramIdBois;

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
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("definirTypeBois "+temperatureSechage+" "+dureeSechage+" "+id); 

            request->send(200, "text/plain", response);
        }else if(request->hasParam("nomBois") && request->hasParam("token")){
            HTTPClient http;

            String paramNomBois = request->getParam("nomBois")->value().c_str();
            String paramToken = request->getParam("token")->value().c_str();
            //String apiSAC = "http://172.16.210.7:3000/api/obtenirBoisNom/"+paramToken+"/"+paramNomBois;
            String apiSAC = "http://10.0.0.60:3000/api/obtenirBoisNom/"+paramToken+"/"+paramNomBois;

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
            if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("definirTypeBois "+temperatureSechage+" "+dureeSechage+" "+id);

            request->send(200, "text/plain", response);
        }else{
            request->send(400, "text/plain", "Envoyez les parametres requis.");
        }
    });

    /**
     * Cette route se contente de récupérer les informations sur le four à un moment x auprès du callback et les renvoie au client.
     * 
     * */
    this->on("/obtenirInfosFour", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("obtenirInfosFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });

    /**
     * Permet de modifier l'état du four.
     * 
     * Cette route ne fait rien d'autre qu'appeller le callback pour changer le four ( etatFour = !etatFour) et renvoyer cet état au client
     * */
    this->on("/lancerFour", HTTP_POST, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("lancerFour"); //Exemple pour appeler une fonction CallBack
        String resultatTemperature = String(repString.c_str());

        request->send(200, "text/plain", resultatTemperature);
    });
   
    this->onNotFound([](AsyncWebServerRequest *request){ request->send(404, "text/plain", "Page Not Found"); });
    
    this->begin();
};