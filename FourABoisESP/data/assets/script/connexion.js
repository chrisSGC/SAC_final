/**
 * Christophe Ferru
 * Projet Sac
 * Cours Objets connectés - 2021
 * 
 * Script JS qui permet la gestion de la page de connexion
 * 
 * Au chargement de cette derneire, on supprime l'item token du localStorage
 * 
 * On ajoute un eventListener sur le submit du formulaire qui va appeler le serveur web de l'ESP pour vérifier si le compte qui essaye de se connecter existe ou non et si les identifiants correspondent.
 * 
 * Si la vérification est faite, on redirige vers l'application, si ce n'est pas le cas, on affiche un message d'erreur
 * 
 */

//const URL_API = "http://172.16.210.211/";
const URL_API = "http://10.0.0.15/";

/**
 * On annule l'evenement de soumission du formulaire pour faire appel à la vérification de connexion
 */
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

/**
 * Méthode asynchrone qui permet de faire appel au serveur web sur l4ESP pour vérifier si les identifiants sont corrects et si le compte existe bel et bien.
 * 
 * Dans le cas ou le compte n'existe pas, on affiche un message et on ré-initialise les inputs,
 * 
 * En revanche, si il existe, on défini le localStorage token et on redirige vers l'application
 */
const verifConnexion = async () => {
    let data = {nomCompte: document.getElementById("floatingInput").value, motDePasse: document.getElementById("floatingPassword").value};

    const response = await fetch(URL_API+'connexion', {
        method: "POST", 
        body: "nomCompte="+data.nomCompte+"&motDePasse="+data.motDePasse, headers: { 'Access-Control-Allow-Origin': '*'}
    });

    const myJson = await response.json();

    if(myJson.code === 200){
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