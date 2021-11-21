const URL_API = "http://172.16.210.211/";
var tempsActuel = 0;

const verifConnexion = async () => {
    // récupere valeur de l'item
    if(typeof(localStorage.getItem("token")) == 'undefined'){
        // La clé existe pas alors on redirige
        window.location.replace("index.html");
    }else{
        // appel get
        /*const response = await fetch(URL_API+'api/verifierExistance/'+localStorage.getItem("token"), {
            method: "GET", 
            headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}
        });
    
        const myJson = await response.json();

        // si valeur non correcte on redirige
        if(myJson.retour == 'false'){
            // On retire l'item
            localStorage.removeItem('token');
    
            // Redirection vers l'index
            window.location.replace("index.html");
        }else{
            return true;
        }*/

        return true;
    }
}

const definirTempsActuel = (temps) => {
    document.getElementById("tempsActuel").innerHTML = temps;
}

window.onload = async () => {
    verifConnexion();

    // get des donnees
    //const response = await fetch(URL_API+'api/bois', {
    const response = await fetch(URL_API+'listeBois', { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}});

    const myJson = await response.json();

    if(myJson.code === 200){
        var select = document.getElementById('typeBoisSelect');
        myJson.donnees.map((ligne) => { var opt = document.createElement('option'); opt.value = ligne.id; opt.innerHTML = ligne.nom; select.appendChild(opt); });
    }E

    //function getFromSP_getAllWoodOptions() {
        /*var xhttp = new XMLHttpRequest();
        xhttp.onreadystatechange = function () {
            if (this.readyState == 4 && this.status == 200) {
                var arrayOfStrings = this.responseText.split("&");
                var select = document.getElementById('typeBoisSelect');*/
                /*for (i = 0; i < arrayOfStrings.length; i=i+2) {
                    var opt = document.createElement('option'); opt.value = ligne.id; opt.innerHTML = ligne.nom; select.appendChild(opt);
                }*/

                //console.log(this.responseText);
    
                //Refresh le contenu
                /*var siteHeader = document.getElementById('typeBois_ListBox_Select');
                siteHeader.style.display='none';
                siteHeader.offsetHeight; // no need to store this anywhere, the reference is enough
                siteHeader.style.display='block';*/
    
           /* }
        };
        xhttp.open("GET", "listeBois", true);
        xhttp.send();*/
    //}
}

document.getElementById('typeBoisSelect').addEventListener('change', async () => {
    verifConnexion();
    let idBois = document.getElementById('typeBoisSelect').value;
    tempsActuel = 0;
    definirTempsActuel(tempsActuel);

    if(idBois != 0){
        const response = await fetch(URL_API+'obtenirBois?idBois='+idBois, { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json'} });
        const myJsonOB = await response.json();

        // si resultat, on affiche les donnees
        if(myJsonOB.code === 200){
            // On met en place les donneees
            document.getElementById('nomBoisDet').innerHTML = myJsonOB.donnees.nom;
            document.getElementById('typeBoisDet').innerHTML = myJsonOB.donnees.type;
            document.getElementById('origineBoisDet').innerHTML = myJsonOB.donnees.origine;
            document.getElementById('tempsSechBoisDet').innerHTML = myJsonOB.donnees.sechage;
            document.getElementById('temperatureBoisDet').innerHTML = myJsonOB.donnees.temperature;

            document.getElementById('tempsTotal').innerHTML = myJsonOB.donnees.sechage;
            document.getElementById('typeBoisChauff').innerHTML = myJsonOB.donnees.nom;
            document.getElementById('temperatureMini').innerHTML = "min "+myJsonOB.donnees.temperature+" Celcius";
        }
    }else{
        document.getElementById('nomBoisDet').innerHTML = document.getElementById('typeBoisDet').innerHTML = document.getElementById('origineBoisDet').innerHTML = document.getElementById('tempsSechBoisDet').innerHTML = document.getElementById('temperatureBoisDet').innerHTML = document.getElementById('tempsTotal').innerHTML = document.getElementById('typeBoisChauff').innerHTML = document.getElementById('temperatureMini').innerHTML = "";
    }
});

setInterval(async () => {
    const response = await fetch(URL_API+'/obtenirInfosFour', { method: "GET", headers: {'Accept': 'application/json', 'Content-Type': 'application/json'} });

    const myJsonSec = await response.json();
    // si resultat, on affiche les donnees

    if(myJsonSec.code === 200){
        // On met en place les donneees
        document.getElementById('typeBoisChauff').innerHTML = myJsonSec.donnees.nomBois;
        document.getElementById('tempVal').innerHTML = myJsonSec.donnees.temperatureActuelle;
        document.getElementById('tempsActuel').innerHTML = myJsonSec.donnees.dureeActuelle;
        document.getElementById('tempsTotal').innerHTML = myJsonSec.donnees.dureeNecessaire;
        document.getElementById('temperatureMini').innerHTML = myJsonSec.donnees.tempMiniBois;
    }
}, 1000);

document.getElementById('demarrerFour').addEventListener('click', async () => {
    let idBois = document.getElementById('typeBoisSelect').value;

    console.log(idBois);
    
    /*const response = await fetch(URL_API+'definirTypeBois', { method: "POST", body: "idBois="+idBois, });*/
    /*const response = await fetch(URL_API+'definirTypeBois', {
    //const response = await fetch(URL_API+'definirTypeBois?idBois='+idBois, {
        method: "POST", 
        //body: JSON.stringify(data),
        body: "idBois="+idBois,
        //body: { idBois : idBois },
        headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}
    });*/
    
    const response = await fetch(URL_API+'lancerFour', {
        method: "POST"
    });
    
    /*const response = await fetch(URL_API+'definirTypeBois?idBois='+idBois, {
        method: "GET"
    });*/

    const myJson = await response.json();

    console.log(myJson);
});