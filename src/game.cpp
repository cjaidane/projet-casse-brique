#include "../include/game.h"
#include "SDL2/SDL_rect.h"
// #include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_keycode.h>
#include <iostream>
#include <random>
#include <fstream>

// Fonction pour générer une valeur aléatoire dans une plage donnée
// Recupere sur internet
int generateRandomNumber(int min, int max) {
    static std::random_device rd; // Initialise le générateur de nombres aléatoires
    static std::mt19937 gen(rd()); // Utilise Mersenne Twister pour générer des nombres aléatoires
    std::uniform_int_distribution<int> dis(min, max); // Distribution uniforme des nombres entiers dans la plage [min, max]
    return dis(gen); // Retourne un nombre aléatoire dans la plage [min, max]
}

// Fonction pour vérifier si un bonus doit apparaître
bool spawnBonus() {
    int randomNumber = generateRandomNumber(0, 10000);
    int count = 5;
    // std::cout << randomNumber << std::endl;
    return randomNumber < count;
}

// Fonction pour charger un niveau à partir d'un fichier
std::vector<std::vector<int> > loadLevel(const std::string& filename) {
    std::vector<std::vector<int> > levelData;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return levelData;
    }

    while (getline(file, line)) {
        std::vector<int> row;
        for (char c : line) {
            row.push_back(c != '0' ? c - '0' : 0);
        }
        levelData.push_back(row);
    }
    file.close();
    std::cout << "Level Loaded Successfully." << std::endl;
    return levelData;
}



/*
    Fonction qui permet de créer les bricks
    Initialise les briques dans la fenêtre de jeu. Définit la position, la taille,
    et la résistance des briques en fonction de leur position verticale.
    Les briques plus proches du haut ont plus de résistance.
*/

// void Game::initBricks() {
//     const int windowWidth = 640;  // Largeur de la fenêtre
//     const int startY = 50;        // Début des briques à partir du haut de la fenêtre
//     const int brickHeight = 20;   // Hauteur de chaque brique
//     const int padding = 5;        // Espace entre les briques
//     const int rows = 4;           // Nombre de lignes de briques

//     //Calculer le nombre de colonnes et la largeur des briques pour qu'elles remplissent toute la largeur
//     const int cols = (windowWidth + padding) / (50 + padding);  // Approximation pour ajuster au mieux
//     const int brickWidth = (windowWidth - (cols + 1) * padding) / cols;  // Largeur ajustée pour remplir l'espace

//     for (int i = 0; i < rows; ++i) {
//         int resistance = rows - i; // Plus de résistance pour les lignes supérieures
//         for (int j = 0; j < cols; ++j) {
//             int startX = j * (brickWidth + padding) + padding;  // Calcul de la position X de chaque brique
//             bricks.emplace_back(startX, startY + i * (brickHeight + padding), 
//                                 brickWidth, brickHeight, resistance);
//         }
//     }
// }
void Game::initBricks(const std::vector<std::vector<int> >& levelData) {
    const int windowWidth = 640;
    const int brickHeight = 20;
    const int padding = 5;
    const int startY = 50;
    activeCountBrick = 0;
    int cols = levelData.empty() ? 0 : levelData[0].size();
    int brickWidth = (windowWidth - (cols + 1) * padding) / cols;

    bricks.clear(); // Nettoie toutes les briques existantes avant de remplir de nouvelles

    for (int i = 0; i < levelData.size(); ++i) {
        for (int j = 0; j < levelData[i].size(); ++j) {
            if (levelData[i][j] != 0) {
                activeCountBrick++;
                int startX = j * (brickWidth + padding) + padding;
                int resistance = levelData[i][j];
                bricks.emplace_back(startX, startY + i * (brickHeight + padding),
                                    brickWidth, brickHeight,resistance);
            }
        }
    }
}
void Game::initBonus(){
    
    //Malus
    bonus.emplace_back(generateRandomNumber(20, 600), false, 10, MALUS, DECREASE_PADDLE, false);
    bonus.emplace_back(generateRandomNumber(20, 600), false, 10, MALUS, INCREASE_SPEED_BALL, false);
    //Bonus
    bonus.emplace_back(generateRandomNumber(20, 600), true, 10, INCREASE_PADDLE, BONUS, false);
    bonus.emplace_back(generateRandomNumber(20, 600), true, 10, DECREASE_SPEED_BALL, BONUS, false);
}

/*
    Fonction qui permet de lancer le jeu.
    Constructeur de la classe Game. Initialise les composants du jeu tels que
    la fenêtre, le renderer, le paddle, la balle, et charge les briques.

*/
Game::Game() : jeuTourne(false), gameStarted(false){
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique");
    renderer = win.getRenderer(); 
    paddle = std::make_unique<Paddle>(renderer, 640, 480); // Créer le Paddle
    ball = std::make_unique<Ball>(320, 435, 10); 
    // bonus(generateRandomNumber(20, 600), false, 10, MALUS, DECREASE_PADDLE, false)
    auto levelData = loadLevel("level/1.txt");
        if (levelData.empty()) {
            std::cerr << "Failed to load level data, exiting." << std::endl;
            return;  // Handle failure appropriately
        }
    initBricks(levelData);
    initBonus();
}

/*
    Fonction qui permet de passer au niveau suivant.
    Construit le chemin du fichier de niveau en fonction du niveau actuel.
    Incrémente le niveau actuel pour passer au niveau suivant.
    @return Le chemin du fichier de niveau suivant.

*/
std::string Game::getNextLevelFilename() {
    static int currentLevel = 1;  // Stocker le niveau actuel
    currentLevel++;  // Incrémenter pour passer au prochain niveau
    return "level/" + std::to_string(currentLevel) + ".txt";  // Construire le chemin du fichier de niveau
}

/*
    Fonction qui permet de réinitialiser l'état du jeu.
    Réinitialise la position de la balle, du paddle, et d'autres états de jeu
    pour commencer un nouveau niveau.

*/
void Game::resetGameState() {
    // Réinitialiser ou configurer d'autres états de jeu nécessaires
    ball->reset(320, 435, 10); // Supposons que `reset` repositionne et remet la balle en jeu
    paddle->render(renderer); // Supposons une méthode de réinitialisation pour le paddle
    gameStarted = false;  // Prêt à redémarrer le jeu pour le nouveau niveau
}
/*
    Fonction qui permet de mettre à jour la logique du jeu.
    Vérifie si toutes les briques sont détruites pour passer au niveau suivant.
    Charge le prochain niveau s'il existe, sinon déclare la fin du jeu.
*/
void Game::updateGameLogic() {
    // Dessiner et mettre à jour les briques, le paddle, la balle, etc.
    // Vérifier les collisions
    // Vérifier si toutes les briques sont détruites
    if (activeCountBrick == 0) {
        std::string nextLevelFilename = getNextLevelFilename(); 
        auto levelData = loadLevel(nextLevelFilename);
        if (!levelData.empty()) {
            initBricks(levelData);
            resetGameState();  // Réinitialiser l'état du jeu pour le nouveau niveau
        } else {
            gameState = GAME_OVER;
        }
    }
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
    SDL_Color textColor ={206, 98, 98};
    SDL_Color messageColor =  {235, 234, 199};
    SDL_Surface* gameNameSurface = TTF_RenderText_Solid(font, "Casse Brique", textColor);
    SDL_Surface* startMessageSurface = TTF_RenderText_Solid(font, "Appuyez sur Espace pour commencer", messageColor);

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
    SDL_Rect gameOverRect = {0, 0, 300, 100};
    gameOverRect.x = (win.getWinWidth() - gameOverRect.w) / 2;
    gameOverRect.y = (win.getWinHeight() - gameOverRect.h) / 2 - 80;
    
    // Afficher un message pour quitter
    SDL_Surface* quitMessageSurface = TTF_RenderText_Solid(font, "Appuyez sur Echap pour quitter", messageColor);
    SDL_Texture* quitMessageTexture = SDL_CreateTextureFromSurface(renderer, quitMessageSurface);
    SDL_Rect quitMessageRect = {0, 0, 400, 50};
    quitMessageRect.x = (win.getWinWidth() - quitMessageRect.w) / 2;
    quitMessageRect.y = (win.getWinHeight() - quitMessageRect.h) / 2 + 40;

    SDL_Texture* heartTexture = loadTexture(renderer, "./assets/Hearts/PNG/basic/heart.png");
    SDL_Rect destRectHeart;

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

                //Permet la collision entre la balle et les briques
                ballRect=ball-> getRect();

                // Boucle sur chaque brique pour vérifier si une collision avec la balle a lieu.
                for (auto& brick : bricks) {
                    if (brick.isActive() && brick.checkCollision(ballRect)) {
                        // Réagir à la collision
                        ball->reverseYVelocity();  
                        activeCountBrick--;
                    }
                }
                //Fonction qui permet de mettre à jour quand on passe au prochain niveau
                updateGameLogic();
                
                SDL_Rect newPaddleSize;

                switch (ball->getVies()) {
                    case 0:
                        gameState = GAME_OVER;
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

                for (auto& bonus : this->bonus) {
                
                    if (gameStarted && !bonus.getUsed() && spawnBonus()) {
                        bonus.setActive(true);
                        bonus.setUsed(true);
                        // std::cout << "Un bonus pop" << std::endl; 
                    }
                    bonus.render(renderer);
                    bonus.update(win.getWinWidth(), win.getWinHeight(), paddle->getRect());

                    if (bonus.getActivationBonus()) {
                        switch (bonus.getBonusType()) {
                            case INCREASE_PADDLE: 
                                newPaddleSize = paddle->getRect();
                                newPaddleSize.w = paddle->getRect().w + 20;
                                paddle->setRect(newPaddleSize);
                                break;
                            case DECREASE_SPEED_BALL:
                                ball->setVel(ball->getVelX() + 1, ball->getVelY() - 1);
                                break;
                            default:
                                //Prendre en compte si MALUS et autre
                                break;
                        }

                        switch (bonus.getMalusType()) {
                            case DECREASE_PADDLE: 
                                newPaddleSize = paddle->getRect();
                                newPaddleSize.w = paddle->getRect().w - 20;
                                paddle->setRect(newPaddleSize);
                                break;
                            case INCREASE_SPEED_BALL:
                                ball->setVel(ball->getVelX() - 1, ball->getVelY() + 1);
                                break;
                            default:
                                //Prendre en compte si BONUS et autre
                                break;
                        }

                        bonus.setActivationBonus(false);
                    }

                }
                // Présente le contenu actuel du renderer à l'écran, mettant à jour l'affichage du jeu.
                SDL_RenderPresent(renderer);
            break;
            case GAME_OVER:
                SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);

                if (showStartMessageBlink) {  // Utilisez la même logique de clignotement que le message de démarrage
                    SDL_RenderCopy(renderer, quitMessageTexture, nullptr, &quitMessageRect);
                }

                SDL_RenderPresent(renderer);

                if (SDL_GetTicks() % 1000 < 500) {
                    showStartMessageBlink = true; // Afficher le texte
                } else {
                    showStartMessageBlink = false; // Masquer le texte
                }
                break;

                // SDL_RenderCopy(renderer, gameOverTexture, nullptr, &gameOverRect);
                // SDL_RenderCopy(renderer, quitMessageTexture, nullptr, &quitMessageRect);

                // SDL_RenderPresent(renderer);
            //break;
        }

        // Introduit un léger délai de 10 millisecondes entre chaque itération de la boucle de jeu.
        SDL_Delay(10); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}
