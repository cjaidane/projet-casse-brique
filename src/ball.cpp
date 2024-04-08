#include "../include/ball.h"

Ball::Ball(int x, int y, int radius) : radius(radius), velX(0), velY(0) {
    center.x = x;
    center.y = y;
}

void Ball::update(bool gameStarted, int windowWidth, int windowHeight, SDL_Rect paddleRect) {
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
    if (center.y - radius < 0 || center.y + radius > windowHeight) {
        velY = -velY;
    }
}

void Ball::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_Rect ballRect = {center.x - radius, center.y - radius, 2 * radius, 2 * radius};
    SDL_RenderFillRect(renderer, &ballRect);
}

void Ball::launch() {
    velX = 5; // Vitesse initiale sur l'axe X
    velY = -5; // Vitesse initiale sur l'axe Y
}

void Ball::setPosition(int x, int y) {
    center.x = x;
    center.y = y;
}
