#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "window.h"
#include "paddle.h" // Inclure l'en-tête du Paddle
#include "brick.h"
#include "../include/bonusmalus.h"
#include <memory>
// #include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h> // Pour charger des images
//
#include <vector>

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    Window win;
    std::unique_ptr<Paddle> paddle; // Ajouter un pointeur vers le Paddle
    bool jeuTourne;
    bool gameStarted;
    SDL_Renderer* renderer; 
    SDL_Rect ballRect;
	std::unique_ptr<Ball> ball; // Ajouter un pointeur vers la balle
    std::vector<Brick> bricks; 
    //int vies;   
    int activeCountBrick;
    void initBricks(const std::vector<std::vector<int> >& levelData);
    void initBonus();
    std::string getNextLevelFilename();
    void updateGameLogic();
    void resetGameState();
    //void resetGame();
    
    enum GameState  {
    	MENU,
	JEU_EN_COURS,
	GAME_OVER
    };

    GameState gameState;
    std::vector<BonusMalus> bonus;

};

#endif // GAME_H
