var URL_API = "http://172.16.210.211/";

document.getElementById("signin").addEventListener("submit", (evt) => {
    evt.preventDefault();

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

    const response = await fetch(URL_API+'connexion', {
        method: "POST", 
        body: "nomCompte="+data.nomCompte+"&motDePasse="+data.motDePasse,
    });

    const myJson = await response.json();

    if(typeof myJson.token !== 'undefined'){
        // On stocke l'id de l'utilisateur
        localStorage.setItem('token', myJson.token);

        // Redirection vers l'application
        window.location.replace("application");
    }else{
        // Afficher une alerte d'erreur
        document.getElementById("mauvaisId").innerHTML = '<div class="alert alert-danger" role="alert">Identifiants incorrects. Merci de ré-essayer!</div>';
        
        // vider les champs
        document.getElementById("floatingInput").value = document.getElementById("floatingPassword").value = "";
    }
}