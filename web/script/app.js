var URL_API = "http://localhost:3000/";
var tempsActuel = 0;

const verifConnexion = async () => {
    // récupere valeur de l'item
    if(typeof(localStorage.getItem("token")) == 'undefined'){
        // La clé existe pas alors on redirige
        window.location.replace("index.html");
    }else{
        // appel get
        const response = await fetch(URL_API+'api/verifierExistance/'+localStorage.getItem("token"), {
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
        }
    }
}

const definirTempsActuel = (temps) => {
    document.getElementById("tempsActuel").innerHTML = temps;
}

window.onload = async () => {
    verifConnexion();

    // get des donnees
    const response = await fetch(URL_API+'api/bois', {
        method: "GET", 
        headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}
    });

    const myJson = await response.json();

    if(myJson){
        var select = document.getElementById('typeBoisSelect');
        myJson.map((ligne) => { var opt = document.createElement('option'); opt.value = ligne.id; opt.innerHTML = ligne.nom; select.appendChild(opt); });
    }
}

document.getElementById('typeBoisSelect').addEventListener('change', async () => {
    verifConnexion();
    let idBois = document.getElementById('typeBoisSelect').value;
    tempsActuel = 0;
    definirTempsActuel(tempsActuel);

    if(idBois != 0){
        // get des donnees
        const response = await fetch(URL_API+'api/obtenirBois/'+idBois, {
            method: "GET", 
            headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}
        });

        const myJsonOB = await response.json();

        // si resultat, on affiche les donnees
        if(myJsonOB){
            console.log(myJsonOB);

            // On met en place les donneees
            document.getElementById('nomBoisDet').innerHTML = myJsonOB.nom;
            document.getElementById('typeBoisDet').innerHTML = myJsonOB.type;
            document.getElementById('origineBoisDet').innerHTML = myJsonOB.origine;
            document.getElementById('tempsSechBoisDet').innerHTML = myJsonOB.sechage;
            document.getElementById('temperatureBoisDet').innerHTML = myJsonOB.temperature;

            document.getElementById('tempsTotal').innerHTML = myJsonOB.sechage;
            document.getElementById('typeBoisChauff').innerHTML = myJsonOB.nom;
            document.getElementById('temperatureMini').innerHTML = "min "+myJsonOB.temperature+" Celcius";
        }
    }else{
        
        document.getElementById('nomBoisDet').innerHTML = document.getElementById('typeBoisDet').innerHTML = document.getElementById('origineBoisDet').innerHTML = document.getElementById('tempsSechBoisDet').innerHTML = document.getElementById('temperatureBoisDet').innerHTML = document.getElementById('tempsTotal').innerHTML = document.getElementById('typeBoisChauff').innerHTML = document.getElementById('temperatureMini').innerHTML = "";
    }
});