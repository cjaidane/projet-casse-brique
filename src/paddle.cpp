#include "../include/paddle.h"

Paddle::Paddle(SDL_Renderer* renderer, int window_width, int window_height)
    : renderer(renderer), window_width(window_width), window_height(window_height), paddle_speed(5) {
    paddleRect.w = 100; // Largeur de la plateforme
    paddleRect.h = 20;  // Hauteur de la plateforme
    paddleRect.x = (window_width - paddleRect.w) / 2; // Position de départ au milieu de la fenêtre
    paddleRect.y = window_height - paddleRect.h - 30; // Un peu au-dessus du bas de la fenêtre
}

Paddle::~Paddle() {
    // Nettoyage, si nécessaire
}

void Paddle::render() {
    // Couleur de la plateforme
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc
    SDL_RenderFillRect(renderer, &paddleRect);
}

void Paddle::moveLeft() {
    paddleRect.x -= paddle_speed;
    if (paddleRect.x < 0) {
        paddleRect.x = 0;
    }
}

void Paddle::moveRight() {
    paddleRect.x += paddle_speed;
    if (paddleRect.x > window_width - paddleRect.w) {
        paddleRect.x = window_width - paddleRect.w;
    }
}

void Paddle::moveTo(int x) {
    paddleRect.x = x - paddleRect.w / 2; // Centre le paddle sur le curseur de la souris
    if (paddleRect.x < 0) {
        paddleRect.x = 0; // Garde le paddle dans les limites de la fenêtre
    } else if (paddleRect.x > window_width - paddleRect.w) {
        paddleRect.x = window_width - paddleRect.w;
    }
}
// Dans paddle.cpp
SDL_Rect Paddle::getRect() const {
    return paddleRect; // Assurez-vous que paddleRect est correctement défini et mis à jour
}
