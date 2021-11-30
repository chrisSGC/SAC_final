/**
 * TODO: Ajouter encryption des données
 * */

require("dotenv").config();

const express = require('express');
var cors = require('cors');
const app = express();
const mysql = require('mysql');
const bcrypt = require('bcrypt');

const entities = require('html-entities');
const saltL = 10;

app.use(cors());
app.use(express.json());
app.listen(process.env.PORT, () => console.log("RUNNING"));

const connexion = mysql.createPool({
    user: process.env.DB_USER,
    password: process.env.DB_PASSWORD,
    database: process.env.DATABASE,
    host: process.env.INSTANCE
});

/**
 * Permet de creer un token a un compte
 */
function creerToken(long) {
    var result           = '';
    var characters       = 'ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789@';
    var charactersLength = characters.length;

    for ( var i = 0; i < long; i++ ) {
        result += characters.charAt(Math.floor(Math.random() * charactersLength));
    }
   return result;
}

/**
 * Permet de recuperer la liste des bois enregistres
 */
app.get('/api/bois', async (req, res) => {
    try{
        const query = "SELECT id, nom FROM bois";
    
        connexion.query(query, (error, results) => {
            if(!results){
                res.json({code: 400, status: "Aucun type de bois disponible."});
            }else{
                res.json({code: 200, donnees: results});
            }
        });
    }catch{
        res.json({code: 400, status: "Aucun type de bois disponible."});
    }
});

/**
 * Recupere les informations d'un type de bois a partir de son id
 */
app.get('/api/obtenirBois/:tokenCompte/:idBois', async (req, res) => {
    try{
        const verifierCompte = "SELECT id FROM compte WHERE token=?";
    
        connexion.query(verifierCompte, [entities.encode(req.params.tokenCompte)], (error, results) => {
            if(!results[0]){
                res.json({code: 400, status: "Type de bois inconnu."});
            }else{
                const query = "SELECT * FROM bois WHERE id=?";
            
                connexion.query(query, [req.params.idBois], (error, results) => {
                    if(!results){
                        res.json({code: 400, status: "Type de bois inconnu."});
                    }else{
                        res.json({code: 200, donnees: results[0]});
                    }
                });
            }
        });
    }catch{
        res.json({code: 400, status: "Type de bois inconnu."});
    }
});

/**
 * Récupère les informations d'un type de bois à partir de son nom (jamais utilisé mais alain le voulait donc ...)
 */
app.get('/api/obtenirBoisNom/:tokenCompte/:nomBois', async (req, res) => {
    try{
        const verifierCompte = "SELECT id FROM compte WHERE token=?";
    
        connexion.query(verifierCompte, [entities.encode(req.params.tokenCompte)], (error, results) => {
            if(!results[0]){
                res.json({code: 400, status: "Type de bois inconnu."});
            }else{
                const query = "SELECT * FROM bois WHERE nom LIKE '%"+entities.encode(req.params.nomBois)+"%'";
            
                connexion.query(query, (error, results) => {
                    if(!results){
                        res.json({code: 400, status: "Type de bois inconnu.", informations: error.message});
                    }else{
                        res.json({code: 200, donnees: results});
                    }
                });
            }
        });
    }catch{
        res.json({code: 400, status: "Type de bois inconnu."});
    }
});

/**
 * PErmet de verifier si un compte existe avec son token
 */
app.get('/api/verifierExistance/:tokenCompte', async (req, res) => {
    const verifierCompte = "SELECT id FROM compte WHERE token=?";

    connexion.query(verifierCompte, [entities.encode(req.params.tokenCompte)], (error, results) => {
        if(!results[0]){
            res.json({retour: "false"});
        }else{
            res.json({retour: "true"});
        }
    });
});

/**
 * Permet de creer un nouvel utilisateur
 * 
 * TODO: ajouter une securisation avec un mot de passe
 */
app.post("/api/nouvelUtilisateur", async (req, res) => {
    try{
        // Mot de passe
        const hashedPass = await bcrypt.hash(req.body.motDePasse, saltL);

        const query = "INSERT INTO compte(nom, mot_de_passe, token) VALUES (?)";
        let valeurs = [entities.encode(req.body.nomCompte), hashedPass, entities.encode(creerToken(15))];

        connexion.query(query, [valeurs], (error, results) => {
            if (error) throw res.json({status: "error"});
            res.json({code: 200, status: "Compte(s) ajouté(s): "+results.affectedRows+"."});
        });
    }catch{
        res.json({code: 400, status: "Erreur, impossible d'ajouter le compte!"});
    }
});

/**
 * Permet de se connecter a l'application avec ses identifiants et son mot de passe
 */
app.post("/api/connexion", (req, res) => {
    try{
        const query = "SELECT mot_de_passe FROM compte WHERE nom=?";
        connexion.query(query, [req.body.nomCompte], (error, results) => {
            if (error) throw res.json({status: error});
            if(!results[0]){
                res.json({status: "Not found"});
            }else{
                try{
                    if(bcrypt.compare(req.body.motDePasse, results[0].mot_de_passe)){
                        let tokenCree = creerToken(15);
                        console.log(tokenCree);

                        // On modifie le token du compte
                        const requeteToken = "UPDATE compte SET token = ? WHERE nom=?";
                        connexion.query(requeteToken, [tokenCree, req.body.nomCompte], (error, results) => {});

                        res.json({code: 200, token: tokenCree});
                    }else{
                        res.json({code: 400, status: "Erreur d'identifiants."});
                    }
                }catch{
                    res.json({code: 400, status: "Erreur d'identifiants."});
                }
            }
        });
    }catch{
        res.json({code: 400, status: "Erreur d'identifiants."});
    }
});