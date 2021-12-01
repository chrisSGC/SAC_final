-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le : mer. 01 déc. 2021 à 13:06
-- Version du serveur :  5.7.31
-- Version de PHP : 7.4.9

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `sac`
--
CREATE DATABASE IF NOT EXISTS `sac_christophe` DEFAULT CHARACTER SET utf8 COLLATE utf8_bin;
USE `sac_christophe`;

-- --------------------------------------------------------

--
-- Structure de la table `bois`
--

DROP TABLE IF EXISTS `bois`;
CREATE TABLE IF NOT EXISTS `bois` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nom` varchar(255) COLLATE utf8_bin NOT NULL,
  `type` varchar(255) COLLATE utf8_bin NOT NULL,
  `origine` varchar(255) COLLATE utf8_bin NOT NULL,
  `sechage` int(3) NOT NULL,
  `temperature` decimal(4,2) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Déchargement des données de la table `bois`
--

INSERT INTO `bois` (`id`, `nom`, `type`, `origine`, `sechage`, `temperature`) VALUES
(1, 'Erable', 'Dur', 'Etats Unis', 25, '25.75'),
(2, 'Erable du japon', 'Dur', 'Pakistan', 20, '20.00');

-- --------------------------------------------------------

--
-- Structure de la table `compte`
--

DROP TABLE IF EXISTS `compte`;
CREATE TABLE IF NOT EXISTS `compte` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `nom` varchar(255) COLLATE utf8_bin NOT NULL,
  `mot_de_passe` varchar(255) COLLATE utf8_bin NOT NULL,
  `token` varchar(255) COLLATE utf8_bin NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=utf8 COLLATE=utf8_bin;

--
-- Déchargement des données de la table `compte`
--

INSERT INTO `compte` (`id`, `nom`, `mot_de_passe`, `token`) VALUES
(1, 'chris', '$2b$10$KS.3KvKkxh5HIZb0wskHyOBMf8c5K4EFkqRZXycUXkOlvSEAfq.46', 'uxa0R865eJzZi@z'),
(2, 'ObjetsConnectes', '$2b$10$uyr4ZkpwxR3QiTH6HU620ulxS1ucs/zdfN7Wkn22IiSWbu/BbtpFS', 'l05lw5@94vzbsTz');
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
