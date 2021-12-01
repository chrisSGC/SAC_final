//const URL_API = "http://172.16.210.211/";
const URL_API = "http://10.0.0.15/";
var tempsActuel = 0;
var tempsMax = 0;
var dureeMax = 0;
var etatFourActuel = 0;

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

/*const definirTempsActuel = (temps) => {
    document.getElementById("tempsActuel").innerHTML = temps;
}*/

window.onload = async () => {
    verifConnexion();

    document.getElementById('delRouge').classList.remove("active");
    document.getElementById('delJaune').classList.remove("active");
    document.getElementById('delVerte').classList.add("active");

    // get des donnees
    const response = await fetch(URL_API+'listeBois', { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'}});

    const myJson = await response.json();

    if(myJson.code === 200){
        var select = document.getElementById('typeBoisSelect');
        myJson.donnees.map((ligne) => { var opt = document.createElement('option'); opt.value = ligne.id; opt.innerHTML = ligne.nom; select.appendChild(opt); });
    }
}

document.getElementById('typeBoisSelect').addEventListener('change', async () => {
    verifConnexion();
    let idBois = document.getElementById('typeBoisSelect').value;
    //tempsActuel = 0;
    //definirTempsActuel(tempsActuel);
    if(etatFourActuel === 0){
        if(idBois != 0){
            const response = await fetch(URL_API+'obtenirBois?token='+localStorage.getItem("token")+'&idBois='+idBois, { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*'} });
            const myJsonOB = await response.json();

            // si resultat, on affiche les donnees
            if(myJsonOB.code === 200){
                // On met en place les donneees
                document.getElementById('nomBoisDet').innerHTML = myJsonOB.donnees.nom;
                document.getElementById('typeBoisDet').innerHTML = myJsonOB.donnees.type;
                document.getElementById('origineBoisDet').innerHTML = myJsonOB.donnees.origine;
                document.getElementById('tempsSechBoisDet').innerHTML = myJsonOB.donnees.sechage;
                document.getElementById('temperatureBoisDet').innerHTML = myJsonOB.donnees.temperature;
                dureeMax = myJsonOB.donnees.sechage;

                document.getElementById('tempsTotal').innerHTML = myJsonOB.donnees.sechage;
                document.getElementById('typeBoisChauff').innerHTML = myJsonOB.donnees.nom;
                document.getElementById('temperatureMini').innerHTML = "min "+myJsonOB.donnees.temperature+" Celcius";
                document.getElementById('tempVal').innerHTML = 0
                //document.getElementById('tempsActuel').innerHTML = 0;
            }
        }else{
            document.getElementById('nomBoisDet').innerHTML = document.getElementById('typeBoisDet').innerHTML = document.getElementById('origineBoisDet').innerHTML = document.getElementById('tempsSechBoisDet').innerHTML = document.getElementById('temperatureBoisDet').innerHTML = document.getElementById('tempsTotal').innerHTML = document.getElementById('typeBoisChauff').innerHTML = document.getElementById('temperatureMini').innerHTML = "";
        }
    }
});

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

        document.getElementById('tempVal').innerHTML = infosFour.donnees.temperatureActuelle;
        document.getElementById('tempsActuel').innerHTML = infosFour.donnees.dureeActuelle;
    }
}, 2000);

document.getElementById('demarrerFour').addEventListener('click', async () => {
    //let idBois = document.getElementById('typeBoisSelect').value;
    
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