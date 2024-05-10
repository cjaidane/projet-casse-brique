#include "../include/ball.h"
#include <iostream>

/*
    Constructeur de la classe Ball.
    Initialise une nouvelle balle avec une position donnée (x, y), un rayon spécifié,
    et une vitesse initiale nulle.

    @param x Position horizontale initiale de la balle.
    @param y Position verticale initiale de la balle.
    @param radius Rayon de la balle.
*/
Ball::Ball(int x, int y, int radius) : radius(radius), velX(0), velY(0), vies(3) {
    center.x = x;
    center.y = y;
}

/*
    Met à jour la position et le comportement de la balle en fonction de l'état du jeu.

    @param gameStarted Booléen indiquant si le jeu a commencé.
    @param windowWidth Largeur de la fenêtre de jeu pour gérer les collisions latérales.
    @param windowHeight Hauteur de la fenêtre pour gérer les collisions verticales.
    @param paddleRect Structure SDL_Rect représentant la position et la taille du paddle.
*/

void Ball::update(bool& gameStarted, int windowWidth, int windowHeight, SDL_Rect paddleRect) {
    // Si le jeu n'a pas encore commencé, la balle doit suivre le paddle
    if (!gameStarted) {
        center.x = paddleRect.x + paddleRect.w / 2; // Centre la balle sur le paddle
        center.y = paddleRect.y - radius - 1; // Place la balle juste au-dessus du paddle
        return;
    }

    // Met à jour la position de la balle en fonction de sa vitesse
    center.x += velX;
    center.y += velY;

    // Gère le rebond sur les bords de la fenêtre
    if (center.x - radius < 0 || center.x + radius > windowWidth) {
        velX = -velX;
    }
    // Gère le rebond sur le haut et le bas de la fenêtre
    else if (center.y - radius < 0) {
        velY = -velY;
    }
    // Gère la perte de la balle
    else if (center.y - radius > windowHeight) {
        gameStarted = false;
        velX = velY = 0;
       reset(paddleRect.x, paddleRect.y, paddleRect.w); 
        vies = vies - 1;
    }
    // Vérification de collision avec le paddle
    else if (center.y + radius >= paddleRect.y &&
        center.y + radius <= paddleRect.y + paddleRect.h &&
        center.x >= paddleRect.x &&
        center.x <= paddleRect.x + paddleRect.w) {
        velY = -abs(velY); // Assurez-vous que la balle rebondit vers le haut
    }
}

/*
    Fonction qui dessine la balle sur l'écran
*/
void Ball::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_Rect ballRect = {center.x - radius, center.y - radius, 2 * radius, 2 * radius};
    SDL_RenderFillRect(renderer, &ballRect);
}

/*
    Fonction qui permet de gérer la vitesse
    Lance la balle en définissant une vitesse initiale positive en x (droite) et négative en y (haut).
*/
void Ball::launch() {
    velX = 5; // Vitesse initiale sur l'axe X
    velY = -5; // Vitesse initiale sur l'axe Y
}

// void Ball::setPosition(int x, int y) {
//     center.x = x;
//     center.y = y;
// }

/*
    Fonction
*/
void Ball::reverseYVelocity() {
    velY = -velY;  // Inverser la vitesse verticale
}

/*
    Réinitialise la position de la balle au-dessus du paddle.

    @param paddleX Position horizontale du paddle.
    @param paddleY Position verticale du paddle.
    @param paddleWidth Largeur du paddle.
    Utilisée après que la balle tombe en dessous du niveau du paddle pour la remettre en position de départ.
*/
void Ball::reset(int paddleX, int paddleY, int paddleWidth) {
    center.x = paddleX + paddleWidth / 2;
    center.y = paddleY - radius - 1;
}

/*
   Retourne un rectangle SDL décrivant la position et la taille de la balle.
*/
SDL_Rect Ball::getRect() const {
    return {center.x - radius, center.y - radius, 2 * radius, 2 * radius};
}

int Ball::getVies(){
    return this->vies;
}


void Ball::setVel(int velX, int velY){
    this->velX = velX;
    this->velY = velY;
}


int Ball::getVelX(){
    return this->velX;
}


int Ball::getVelY(){
    return this->velY;
}
