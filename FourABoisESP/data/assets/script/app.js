/**
 * Christophe Ferru
 * Projet Sac
 * Cours Objets connectés - 2021
 * 
 * Script JS qui permet la gestion de l'application
 * 
 * Au chargement de cette derneire, on vérifie la présence de l'item token du localStorage et on charge la liste des types de bois si il existe.
 * 
 * Dans le cas ou il n'existe pas, on redirige vers l'index
 * 
 * On ajoute un eventListener sur le submit du formulaire qui va appeler le serveur web de l'ESP pour vérifier si le compte qui essaye de se connecter existe ou non et si les identifiants correspondent.
 * 
 * Si la vérification est faite, on redirige vers l'application, si ce n'est pas le cas, on affiche un message d'erreur
 * 
 */

//const URL_API = "http://172.16.210.211/";
const URL_API = "http://10.0.0.15/";
var tempsActuel = 0;
var tempsMax = 0;
var dureeMax = 0;
var etatFourActuel = 0;

/**
 * Méthode asynchrone qui permet la vérification de la présence et de la validité du token.
 * 
 * Si le token n'est pas présent ou n'est pas valide, on redirige vers l'accueil
 */
const verifConnexion = async () => {
    // récupere valeur de l'item
    if(localStorage.getItem("token") === null){
        // La clé existe pas alors on redirige
        window.location.replace("/");
    }else{
        // appel get
        const response = await fetch(URL_API+'verifierExistance?token='+localStorage.getItem("token"), {
            method: "GET", 
            headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'}
        });
    
        const myJson = await response.json();

        // si valeur non correcte on redirige
        if(myJson.code !== 200){
            // On retire l'item
            localStorage.removeItem('token');
    
            // Redirection vers l'index
            window.location.replace("/");
        }
    }
}

/**
 * Au chargement de la page, on vérifie l'existence du token sur le navigateur.
 * 
 * Si il existe, on récupere la liste des bois et on met le tout dans le select prévu à cet effet.
 */
window.onload = async () => {
    verifConnexion();

    document.getElementById('delRouge').classList.remove("active");
    document.getElementById('delJaune').classList.remove("active");
    document.getElementById('delVerte').classList.add("active");

    const response = await fetch(URL_API+'listeBois', { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'}});

    const myJson = await response.json();

    if(myJson.code === 200){
        var select = document.getElementById('typeBoisSelect');
        myJson.donnees.map((ligne) => { var opt = document.createElement('option'); opt.value = ligne.id; opt.innerHTML = ligne.nom; select.appendChild(opt); });
    }
}

/**
 * EventListener sur le changement de valeur du select
 * 
 * commence bien évidement par une vérification de la connexion
 * 
 * Au changement de valeur, on va récupérer les informations du bois sélectionné uniquement si le four n'est pas lancé
 * 
 * une fois les information récupérées, si le serveur a renvoyé un code 200 ( se référer à l'API pour connaitre les détails), on place les informations dans les zones associées
 */
document.getElementById('typeBoisSelect').addEventListener('change', async () => {
    verifConnexion();
    let idBois = document.getElementById('typeBoisSelect').value;

    if(etatFourActuel === 0){
        if(idBois != 0){
            const response = await fetch(URL_API+'obtenirBois?token='+localStorage.getItem("token")+'&idBois='+idBois, { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'} });
            const myJsonOB = await response.json();

            // si resultat, on affiche les donnees
            if(myJsonOB.code === 200){
                // On met en place les donneees
                document.getElementById('nomBoisDet').innerText = myJsonOB.donnees.nom;
                document.getElementById('typeBoisDet').innerText = myJsonOB.donnees.type;
                document.getElementById('origineBoisDet').innerText = myJsonOB.donnees.origine;
                document.getElementById('tempsSechBoisDet').innerText = myJsonOB.donnees.sechage;
                document.getElementById('temperatureBoisDet').innerText = myJsonOB.donnees.temperature;
                dureeMax = myJsonOB.donnees.sechage;

                document.getElementById('tempsTotal').innerText = myJsonOB.donnees.sechage;
                document.getElementById('typeBoisChauff').innerText = myJsonOB.donnees.nom;
                document.getElementById('temperatureMini').innerText = "min "+myJsonOB.donnees.temperature+" Celcius";
                document.getElementById('tempVal').innerText = 0
                //document.getElementById('tempsActuel').innerHTML = 0;
            }
        }else{
            document.getElementById('nomBoisDet').innerText = document.getElementById('typeBoisDet').innerText = document.getElementById('origineBoisDet').innerText = document.getElementById('tempsSechBoisDet').innerText = document.getElementById('temperatureBoisDet').innerText = document.getElementById('tempsTotal').innerText = document.getElementById('typeBoisChauff').innerText = document.getElementById('temperatureMini').innerText = "";
        }
    }
});

/**
 * Toutes les deux secondes le systeme va récupérer les informations du four
 * 
 * En fonction de l'état du four, de sa température, on va afficher un voyant différent. Pour conntaire la signification des voyants, se réferer à la méthode allumerDelEtatFour du main de l'ESP
 * 
 * On détermine ainsi la variable etatFourActuel afin de pouvoir gérer l'envoi ou non d'informations vers le serveurWeb
 */
setInterval(async () => {
    const obtenirInfosFour = await fetch(URL_API+'obtenirInfosFour', { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'} });
    const infosFour = await obtenirInfosFour.json();

    if(infosFour.code === 200){
        if(infosFour.donnees.dureeActuelle === dureeMax) {
            document.getElementById('demarrerFour').innerText = "Demarrer le four";
            document.getElementById('demarrerFour').classList.remove("fourActif");

            document.getElementById('delRouge').classList.remove("active");
            document.getElementById('delJaune').classList.remove("active");
            document.getElementById('delVerte').classList.add("active");
        }else{
            if(infosFour.donnees.etatFour === 1){
                etatFourActuel = 1;

                if(infosFour.donnees.temperatureActuelle < infosFour.donnees.tempMiniBois){
                    document.getElementById('delRouge').classList.add("active");
                    document.getElementById('delJaune').classList.remove("active");
                    document.getElementById('delVerte').classList.remove("active");
                }else{
                    document.getElementById('delRouge').classList.add("active");
                    document.getElementById('delJaune').classList.add("active");
                    document.getElementById('delVerte').classList.remove("active");
                }
            }else{
                etatFourActuel = 0;

                document.getElementById('delRouge').classList.remove("active");
                document.getElementById('delJaune').classList.remove("active");
                document.getElementById('delVerte').classList.add("active");
            }
        }

        document.getElementById('tempVal').innerText = infosFour.donnees.temperatureActuelle;
        document.getElementById('tempsActuel').innerText = infosFour.donnees.dureeActuelle;
    }
}, 2000);

/**
 * EventListener sur le click du bouton de démarrage du four
 * 
 * On démarre ainsi le four et, en fonction du code de retour, on gere l'affichage des voyants et du bouton
 */
document.getElementById('demarrerFour').addEventListener('click', async () => {
    const lancerFour = await fetch(URL_API+'lancerFour', {
        method: "POST", headers: { 'Access-Control-Allow-Origin': '*'}
    });
    const etatFourRetour = await lancerFour.json();

    if(etatFourRetour.code){
        if(etatFourRetour.code === 200){
            document.getElementById('delRouge').classList.add("active");
            document.getElementById('delJaune').classList.remove("active");
            document.getElementById('delVerte').classList.remove("active");

            document.getElementById('demarrerFour').innerText = "Arreter le four";
            document.getElementById('demarrerFour').classList.add("fourActif");

            etatFourActuel = 1;
        }else if(etatFourRetour.code === 400){
            document.getElementById('delRouge').classList.remove("active");
            document.getElementById('delJaune').classList.remove("active");
            document.getElementById('delVerte').classList.remove("active");

            document.getElementById('demarrerFour').innerText = "Demarrer le four";
            document.getElementById('demarrerFour').classList.remove("fourActif");

            etatFourActuel = 0;
        }
    }
});