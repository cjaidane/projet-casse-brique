#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "window.h"
#include "paddle.h" // Inclure l'en-tête du Paddle
#include <memory>
#include "brick.h"
// #include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_ttf.h>

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
    SDL_Renderer* renderer; 
    SDL_Rect ballRect;
	Ball* ball; // Ajouter un pointeur vers la balle
    std::vector<Brick> bricks; 
    //int vies;   
    void initBricks();
    void resetGame();
    
    enum GameState  {
    	MENU,
	JEU_EN_COURS,
	GAME_OVER
    };

    GameState gameState;

};

#endif // GAME_H
