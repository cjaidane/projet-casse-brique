#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "window.h"
#include "paddle.h" // Inclure l'en-tête du Paddle

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    Window win;
    Paddle* paddle; // Ajouter un pointeur vers le Paddle
    bool jeuTourne;
    SDL_Renderer* renderer; // Assurez-vous d'avoir un renderer
	Ball* ball; // Ajouter un pointeur vers la balle
};

#endif // GAME_H
