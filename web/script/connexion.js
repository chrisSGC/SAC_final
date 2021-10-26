var URL_API = "http://localhost:3000/";

document.getElementById("signin").addEventListener("submit", (evt) => {
    evt.preventDefault();

    //document.getElementById("mauvaisId").innerHTML = "DATA SETTED";
    verifConnexion();
});


/**
 * Au chargement on supprime un eventuel item
 */
window.onload = () => {
    localStorage.removeItem('token');
}

const verifConnexion = async () => {
    let data = {nomCompte: document.getElementById("floatingInput").value, motDePasse: document.getElementById("floatingPassword").value};

    const response = await fetch(URL_API+'api/connexion', {
        method: "POST", 
        body: JSON.stringify(data),
        headers: {'Accept': 'application/json', 'Content-Type': 'application/json'}
    });

    const myJson = await response.json();

    if(typeof myJson.token !== 'undefined'){
        // On stocke l'id de l'utilisateur
        localStorage.setItem('token', myJson.token);

        // Redirection vers l'application
        window.location.replace("application.html");
    }else{
        // Afficher une alerte d'erreur
        document.getElementById("mauvaisId").innerHTML = '<div class="alert alert-danger" role="alert">Identifiants incorrects. Merci de ré-essayer!</div>';
        
        // vider les champs
        document.getElementById("floatingInput").value = document.getElementById("floatingPassword").value = "";
    }
}