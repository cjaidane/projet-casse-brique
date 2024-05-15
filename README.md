# Casse-Briques

## Description
Casse-Briques est un petit jeu développé en C++ avec l'utilisation de la bibliothèque SDL2. Le jeu consiste à détruire toutes les briques à l'aide d'une balle rebondissante contrôlée par le joueur.

Ce projet a été réalisé dans le cadre de nos études de Master 1 Informatique.
## Fonctionnalités
- Contrôle du paddle avec les touches fléchées gauche et droite et avec la souris.
- Lancement de la balle avec la barre d'espace.
- Destruction de briques en faisant rebondir la balle dessus.
- Briques rectangulaires et hexagonales en fonction du choix du joueur.
- Affichage du score en cours de jeu.
- Niveaux de difficulté croissants en fonction de fichiers issus du dossier level.


## Installation
Assurez-vous d'avoir installé les bonnes librairies sur votre système

**SDL2**
```bash
sudo apt-get install libsdl2-dev
```
**SDL2-ttf**
```bash
sudo apt-get install libsdl-ttf2.0-dev
```

**SDL2-Images**
```bash
apt-get install libsdl2-image-dev
```
Clonez ce dépôt dans votre répertoire local.
```bash
git clone https://github.com/cjaidane/projet-casse-brique.git && cd projet-casse-brique
```

Compilez le jeu à l'aide de CMake :
```bash
cmake . && cmake --build .
```
Lancez le jeu :
```bash
./casse_briques
```

## Contrôles
**Dans le menu**

La touche espace pour lancer le jeu classique.
La touche h pour mettre les briques hexagonales.

**Dans le jeu**

Flèche gauche ou souris vers la gauche : Déplacer le paddle vers la gauche.
Flèche droite ou souris vers la droite : Déplacer le paddle vers la droite.
Barre d'espace : Lancer la balle.

### Auteurs
Ce projet a été développé par Jaidane Chaïma et Oberhauser Clément.
