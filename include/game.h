#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "window.h"
#include "paddle.h" // Inclure l'en-tête du Paddle
#include <memory>
#include "brick.h"

#include <vector>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    Window win;
    Paddle* paddle; // Ajouter un pointeur vers le Paddle
    bool jeuTourne;
    bool gameStarted;
    SDL_Renderer* renderer; // Assurez-vous d'avoir un renderer
	Ball* ball; // Ajouter un pointeur vers la balle
    std::vector<Brick> bricks;    
    void initBricks();
    void resetGame();
    

};

#endif // GAME_H
