#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "window.h"
#include "paddle.h" // Inclure l'en-tête du Paddle
#include "brick.h"
#include "briqueHexagone.h"
#include "bonusmalus.h"
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
    void updateGameLogic();
    void loadNextLevel();
    int countdown;  // Ajouter ceci dans la section des attributs de la classe
    bool isCounterPaused;

private:
    Window win;
    std::unique_ptr<Paddle> paddle; // Ajouter un pointeur vers le Paddle
    bool jeuTourne;
    bool gameStarted;
    SDL_Renderer* renderer; 
    SDL_Rect ballRect;
	
	std::vector<std::unique_ptr<Ball> > balls;  // Gérer plusieurs balles
    //std::unique_ptr<Ball> ball; // Ajouter un pointeur vers la balle
    std::vector<Brick> bricks; 
    std::vector<BrickHexagone> bricksH; 
    //int vies;   
    bool brickOrH;
    int activeCountBrick;
    int activeCountBrickH;
    void initBricks(const std::vector<std::vector<int> >& levelData);
    void initBricksH(const std::vector<std::vector<int> >& levelData);
    void initBonus();
    void resetGameState();
    void displayNextLevelCountdown();
    void displayLevel();
    
    enum GameState  {
    	MENU,
        JEU_EN_COURS,
        CHANGEMENT_NIVEAU,
        GAME_OVER,
        TOUS_NIVEAUX_ATTEINTS
    };
    int niveauActuel=1;
    

    GameState gameState;
    std::vector<BonusMalus> bonus;

};

#endif // GAME_H