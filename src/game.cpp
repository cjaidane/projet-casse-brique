#include "../include/game.h"
// #include <SDL2/SDL_ttf.h>
#include <iostream>


/*
    Fonction qui permet de créer les bricks
    Initialise les briques dans la fenêtre de jeu. Définit la position, la taille,
    et la résistance des briques en fonction de leur position verticale.
    Les briques plus proches du haut ont plus de résistance.
*/

void Game::initBricks() {
    const int windowWidth = 640;  // Largeur de la fenêtre
    const int startY = 50;        // Début des briques à partir du haut de la fenêtre
    const int brickHeight = 20;   // Hauteur de chaque brique
    const int padding = 5;        // Espace entre les briques
    const int rows = 4;           // Nombre de lignes de briques

    //Calculer le nombre de colonnes et la largeur des briques pour qu'elles remplissent toute la largeur
    const int cols = (windowWidth + padding) / (50 + padding);  // Approximation pour ajuster au mieux
    const int brickWidth = (windowWidth - (cols + 1) * padding) / cols;  // Largeur ajustée pour remplir l'espace

    for (int i = 0; i < rows; ++i) {
        int resistance = rows - i; // Plus de résistance pour les lignes supérieures
        for (int j = 0; j < cols; ++j) {
            int startX = j * (brickWidth + padding) + padding;  // Calcul de la position X de chaque brique
            bricks.emplace_back(startX, startY + i * (brickHeight + padding), 
                                brickWidth, brickHeight, resistance);
        }
    }
}

/*
    Fonction qui permet de lancer le jeu.
    Constructeur de la classe Game. Initialise les composants du jeu tels que
    la fenêtre, le renderer, le paddle, la balle, et charge les briques.

*/
Game::Game() : jeuTourne(false), gameStarted(false) {
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique", 640, 480);
    renderer = win.getRenderer(); 
    paddle = std::make_unique<Paddle>(renderer, 640, 480); // Créer le Paddle
    ball = std::make_unique<Ball>(320, 435, 10); 
    initBricks();
}

/*

    Destructeur de la classe Game. Nettoie les ressources en supprimant
    le paddle et la balle et en fermant SDL proprement.
*/
Game::~Game() {
    // delete paddle; // Libérer la mémoire allouée au Paddle
    // delete ball; // Libérer la mémoire allouée à la balle
}

/*
    Boucle principale du jeu. Gère les événements d'entrée, met à jour les éléments du jeu,
    vérifie les collisions, et rend les graphiques à l'écran. Arrête le jeu.
*/
void Game::run() {
    if (!renderer) {
        return;
    }
    jeuTourne = true;
    gameStarted = false;
    gameState = MENU;
    SDL_Rect gameNameRect;
    SDL_Rect startMessageRect;
    if ( TTF_Init() < 0 ) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
    }

    SDL_Event evenement;
    while (jeuTourne) {
        while (SDL_PollEvent(&evenement) != 0) {
            switch (evenement.type) {
                case SDL_QUIT:
                    jeuTourne = false;
                    break;
                case SDL_KEYDOWN:
                //Gère les mouvements du paddle via les touches fléchées et le lancement de la balle avec la barre d'espace.
                    switch (evenement.key.keysym.sym) {
                        case SDLK_LEFT:
                            if (gameState == JEU_EN_COURS) {
                                paddle->moveLeft();
                            }
                            break;
                        case SDLK_RIGHT:
                            if (gameState == JEU_EN_COURS) {
                                paddle->moveRight();
                            }
                            break;
                        case SDLK_SPACE:
                            if (gameState == JEU_EN_COURS) {
                                if (!gameStarted) {
                                    ball->launch();
                                    // for(auto& ball : ball){
                                    //     ball->launch();
                                    // }
                                    gameStarted = true;
                                }
                            }else if (gameState == MENU) {
                            
                                gameState = JEU_EN_COURS;
                            }
                            break;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    paddle->moveTo(evenement.motion.x);
                    break;
            }
        }
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer); //Nettoye l'écran

        switch (gameState) {
            case MENU:
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_RenderClear(renderer);


                // // Affichage des options du menu
                // SDL_Color textColor = {0, 0, 0, 255};
                // const char* options[] = {"Jouer", "Options", "Quitter"};
                // for (int i = 0; i < 3; ++i) {
                //     SDL_Surface* textSurface = TTF_RenderText_Solid(nullptr, options[i], textColor);
                //     SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
                //     SDL_FreeSurface(textSurface);
                //     SDL_RenderCopy(renderer, textTexture, nullptr, &textRects[i]);
                //     SDL_DestroyTexture(textTexture);
                // }
                //
                // // Mettre en surbrillance l'option sélectionnée
                // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                // SDL_RenderDrawRect(renderer, &textRects[selectedOption]);

                SDL_RenderPresent(renderer);

            break;

            case JEU_EN_COURS:

                //Dessiner les bricks active
                for (auto& brick : bricks) {
                    if (brick.isActive()) {
                        brick.render(renderer);
                    }
                }

                paddle->render(renderer);// Dessine le paddle
                ball->render(renderer); // Dessine la balle
                //permet de mettre à jour la position de la balle quand le paddle est en mouvement
                ball->update(gameStarted, 640, 480, paddle->getRect()); 
                if (ball->getVies() == 0) {
                    jeuTourne = false;

                }
                //Permet la collision entre la balle et les briques
                ballRect=ball-> getRect();

                // Boucle sur chaque brique pour vérifier si une collision avec la balle a lieu.
                for (auto& brick : bricks) {
                    if (brick.isActive() && brick.checkCollision(ballRect)) {
                        // Réagir à la collision
                        ball->reverseYVelocity();  
                    }
                }

                // Présente le contenu actuel du renderer à l'écran, mettant à jour l'affichage du jeu.
                SDL_RenderPresent(renderer);
            break;
            case GAME_OVER:

            break;
        }

        // Introduit un léger délai de 10 millisecondes entre chaque itération de la boucle de jeu.
        SDL_Delay(10); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}



