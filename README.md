<div id="top"></div>
<!--
***
*** https://github.com/othneildrew/Best-README-Template
*** Thanks for checking out the Best-README-Template. If you have a suggestion
*** that would make this better, please fork the repo and create a pull request
*** or simply open an issue with the tag "enhancement".
*** Don't forget to give the project a star!
*** Thanks again! Now go create something AMAZING! :D
-->



<!-- PROJECT SHIELDS -->
<!--
*** I'm using markdown "reference style" links for readability.
*** Reference links are enclosed in brackets [ ] instead of parentheses ( ).
*** See the bottom of this document for the declaration of the reference variables
*** for contributors-url, forks-url, etc. This is an optional, concise syntax you may use.
*** https://www.markdownguide.org/basic-syntax/#reference-style-links
-->

<!-- PROJECT LOGO -->
<br />
<div align="center">
  <img src="FourABoisESP/data/assets/images/logoSAC.PNG" alt="Logo" width="80" height="80">

  <h3 align="center">TP SAC Objets connectes</h3>
  <h4 align="center">Cegep de Riviere-du-Loup</h4>
</div>
<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table des matieres</summary>
  <ol>
    <li>
      <a href="#a-propos-du-projet">A propos du projet</a>
    </li>
    <li>
      <a href="#installation-du-serveur-nodejs">Installation du serveur NodeJS</a>
      <ul>
        <li><a href="#prerequis">Prerequis</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#roadmap">Roadmap</a></li>
    <li><a href="#contributing">Contributing</a></li>
    <li><a href="#license">License</a></li>
    <li><a href="#contact">Contact</a></li>
    <li><a href="#acknowledgments">Acknowledgments</a></li>
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## A propos du projet

Il s'agit du TP de session du cours Objets connectes du Cegep de Riviere-du-Loup, technique d'informatique, realise a la session automne 2021.

Nous devons réaliser un système connecté pour faire la gestion du four de séchage de l’entreprise. La température doit être précise et constante afin de ne pas modifier les propriétés du produit fini.

A noter que l'ESP doit etre branche sur du 3.3v.

<p align="right">(<a href="#top">Retour au debut</a>)</p>

<!-- GETTING STARTED -->
## Installation du serveur NodeJS

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequis

<ul>
	<li>NodeJS version 16.7.0 ou supérieure</li>
	<li>phpMyAdmin 5.0.2</li>
	<li>MySQL 5.7.31 </li>
</ul>

### Installation

_Importer la base de donnees avec le script fourni dnas le dossier api._

* Cloner le projet
* npm install
  ```sh
  npm i
  ```
* modifier le .env avec les informations de connexion a la bd
* Demarrer le serveur NodeJS
   ```sh
   npm run devStart
   ```
* Suivre le document de mise en ligne

<!-- CONTACT -->
## Contact

Christophe Ferru - 1941676@cegeprdl.ca