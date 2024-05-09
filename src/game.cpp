#include "../include/game.h"
#include "SDL2/SDL_rect.h"
// #include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_keycode.h>
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
    win.init("Casse Brique");
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



// Fonction pour charger une image en tant que texture
SDL_Texture* loadTexture(SDL_Renderer* renderer, const char* imagePath) {
    // Charger l'image en tant que surface
    SDL_Surface* surface = IMG_Load(imagePath);
    if (!surface) {
        // Gérer l'erreur de chargement de l'image
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", imagePath, IMG_GetError());
        return nullptr;
    }

    // Créer une texture à partir de la surface chargée
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        // Gérer l'erreur de création de la texture
        SDL_Log("Unable to create texture from %s! SDL Error: %s\n", imagePath, SDL_GetError());
        SDL_FreeSurface(surface);
        return nullptr;
    }

    // Libérer la surface car elle n'est plus nécessaire après la création de la texture
    SDL_FreeSurface(surface);

    return texture;
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
    bool malus_paddle=false;

    // A modifier pour mettre dans la classe
    SDL_Rect gameNameRect;
    SDL_Rect startMessageRect;
    if ( TTF_Init() < 0 ) {
        std::cout << "Error initializing SDL_ttf: " << TTF_GetError() << std::endl;
        return;
    }
    // Chargement de la police (par exemple, Arial avec une taille de 24 points)
    TTF_Font* font = TTF_OpenFont("./fonts/Roboto-Medium.ttf", 10);
    if (font == nullptr) {
        std::cout << "Error initializing font" << std::endl;
        return;
    }

    // Création des surfaces de texte pour le nom du jeu et le message de démarrage
    SDL_Color textColor = {255, 255, 255};
    SDL_Surface* gameNameSurface = TTF_RenderText_Solid(font, "Casse Brique", {206, 98, 98});
    SDL_Surface* startMessageSurface = TTF_RenderText_Solid(font, "Appuyez sur Espace pour commencer", {235, 234, 199});

    // Conversion des surfaces de texte en textures SDL
    SDL_Texture* gameNameTexture = SDL_CreateTextureFromSurface(renderer, gameNameSurface);
    SDL_Texture* startMessageTexture = SDL_CreateTextureFromSurface(renderer, startMessageSurface);

    // Mettre à jour la position de gameNameRect
    // Position et taille du nom du jeu
    gameNameRect = {0, 0, 300, 100};
    // Position et taille du message de démarrage
    startMessageRect = {0, 0, 500, 50};

    gameNameRect.x = (win.getWinWidth() - gameNameRect.w) / 2;
    gameNameRect.y = (win.getWinHeight() - gameNameRect.h) / 2 - 80; 
    startMessageRect.x = (win.getWinWidth() - startMessageRect.w) / 2 ;
    startMessageRect.y = (win.getWinHeight() - startMessageRect.h) / 2 + 40; 
    bool showStartMessageBlink = true;

    // Afficher le message de game over
    SDL_Surface* gameOverSurface = TTF_RenderText_Solid(font, "Game Over", textColor);
    SDL_Texture* gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOverSurface);
    SDL_Rect gameOverRect = {0, 0, 200, 50};
    gameOverRect.x = (win.getWinWidth() - gameOverRect.w) / 2;
    gameOverRect.y = (win.getWinHeight() - gameOverRect.h) / 2;
    
    // Afficher un message pour quitter
    SDL_Surface* quitMessageSurface = TTF_RenderText_Solid(font, "Appuyez sur Echap pour quitter", textColor);
    SDL_Texture* quitMessageTexture = SDL_CreateTextureFromSurface(renderer, quitMessageSurface);
    SDL_Rect quitMessageRect = {0, 0, 400, 50};
    quitMessageRect.x = (win.getWinWidth() - quitMessageRect.w) / 2;
    quitMessageRect.y = (win.getWinHeight() - quitMessageRect.h) / 2 + 50;

    SDL_Texture* heartTexture = loadTexture(renderer, "./assets/Hearts/PNG/basic/heart.png");
    SDL_Texture* heartTexture2 = loadTexture(renderer, "./assets/Heart/Heart-2-life.png");
    SDL_Texture* heartTexture1 = loadTexture(renderer, "./assets/Heart/Heart-1-life.png");
    SDL_Rect destRectHeart;
    SDL_Rect destRectSize;

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
                        case SDLK_ESCAPE:
                            if (gameState == GAME_OVER) {
                                jeuTourne = false; 
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
                // SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                // SDL_RenderClear(renderer);


                // Affichage du texte
                SDL_RenderCopy(renderer, gameNameTexture, nullptr, &gameNameRect);

                if (showStartMessageBlink) {
                
                    SDL_RenderCopy(renderer, startMessageTexture, nullptr, &startMessageRect);
                }

                SDL_RenderPresent(renderer);
                if (SDL_GetTicks() % 1000 < 500) {
                    showStartMessageBlink = true; // Afficher le texte
                } else {
                    showStartMessageBlink = false; // Masquer le texte
                }
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
                ball->update(gameStarted, win.getWinWidth(), win.getWinHeight(), paddle->getRect()); 
                if (ball->getVies() == 0) {
                   gameState = GAME_OVER;

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
                SDL_Rect newPaddleSize;

                switch (ball->getVies()) {
                    case 0:

                    break;
                    case 1:

                        destRectHeart = {0, 0, 30, 30}; 
                        destRectHeart.y = win.getWinHeight() - destRectHeart.h;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);

                        newPaddleSize = paddle->getRect();
                        newPaddleSize.w = 70;
                        paddle->setRect(newPaddleSize);
                        malus_paddle=true;
                    break;

                    case 2:

                        destRectHeart = {0, 0, 30, 30}; 
                        destRectHeart.y = win.getWinHeight() - destRectHeart.h;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);
                        destRectHeart.x += destRectHeart.w;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);
                        malus_paddle=false;
                    break;

                    case 3:
                        destRectHeart = {0, 0, 30, 30}; 
                        destRectHeart.y = win.getWinHeight() - destRectHeart.h;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);
                        destRectHeart.x += destRectHeart.w;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);
                        destRectHeart.x += destRectHeart.w;
                        SDL_RenderCopy(renderer, heartTexture, nullptr, &destRectHeart);

                        malus_paddle=false;
                    break;
                }

                // AJOUT MALUS REDUCTION PADDLE
                // if(ball->getVies()==1){
                // }else if (ball->getVies()>1){
                // }

                // Présente le contenu actuel du renderer à l'écran, mettant à jour l'affichage du jeu.
                SDL_RenderPresent(renderer);
            break;
            case GAME_OVER:

                SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
                SDL_RenderCopy(renderer, quitMessageTexture, nullptr, &quitMessageRect);

                SDL_RenderPresent(renderer);
            break;
        }

        // Introduit un léger délai de 10 millisecondes entre chaque itération de la boucle de jeu.
        SDL_Delay(10); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}
