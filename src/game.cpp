#include "../include/game.h"
#include "SDL2/SDL_rect.h"
#include "SDL2/SDL_render.h"
#include "SDL2/SDL_timer.h"
// #include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_keycode.h>
#include <iostream>
#include <iterator>
#include <ostream>
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

/*
    Fonction pour charger un niveau à partir d'un fichier texte.
    Le fichier texte contient des données de niveau sous forme de grille.
    Chaque caractère représente un type de brique, où '0' signifie pas de brique.

    @param filename Nom du fichier texte contenant les données du niveau.
    @return Un vecteur de vecteurs d'entiers représentant les données du niveau.

*/
std::vector<std::vector<int> > loadLevel(const std::string& filename) {
    std::vector<std::vector<int> > levelData;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Pas possible d'ouvrir le fichier: " << filename << std::endl;
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
    std::cout << "Niveau charge avec succes." << std::endl;
    return levelData;
}



/*
    Fonction qui initialise les briques du jeu.
    Calcule la position de chaque brique en fonction de la taille de la fenêtre
    et de la taille des briques. Les briques sont stockées dans un vecteur de briques.

    @param levelData Un vecteur de vecteurs d'entiers représentant les données du niveau.
*/
void Game::initBricks(const std::vector<std::vector<int> >& levelData) {
    const int windowWidth = 640;
    const int brickHeight = 20;
    const int padding = 5;
    const int startY = 50;
    activeCountBrick = 0; // Réinitialise le compteur de briques actives
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

void Game::initBricksH(const std::vector<std::vector<int> >& levelData) {
    const int windowWidth = win.getWinWidth();
    activeCountBrickH = 0;
    int cols = levelData.empty() ? 0 : levelData[0].size();
    bricksH.clear(); // Nettoie toutes les briques existantes avant de remplir de nouvelles

    for (int i = 0; i < levelData.size(); ++i) {
        for (int j = 0; j < levelData[i].size(); ++j) {
            if (levelData[i][j] != 0) {
                activeCountBrickH++;
                int resistance=levelData[i][j];
                // std::cout << i << " " << j << " " << -j-i << std::endl;
                bricksH.emplace_back(resistance, j, i, -j-i);
            }
        }
    }
}

void Game::initBonus(){
    
    //Malus
    bonus.emplace_back(generateRandomNumber(20, 600), false, 10, MALUS, DECREASE_PADDLE, false);
    bonus.emplace_back(generateRandomNumber(20, 600), false, 10, MALUS, INCREASE_SPEED_BALL, false);
        //Multiball
    bonus.emplace_back(generateRandomNumber(20, 600), false, 10, MALUS,MULTIBALL, false);
    //Bonus
    bonus.emplace_back(generateRandomNumber(20, 600), true, 10, INCREASE_PADDLE, BONUS, false);
    bonus.emplace_back(generateRandomNumber(20, 600), true, 10, DECREASE_SPEED_BALL, BONUS, false);
    
}

/*
    Fonction qui permet de lancer le jeu.
    Constructeur de la classe Game. Initialise les composants du jeu tels que
    la fenêtre, le renderer, le paddle, la balle, et charge les briques.

*/
Game::Game() : jeuTourne(false), gameStarted(false), isCounterPaused(false), brickOrH(false){
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique");
    renderer = win.getRenderer(); 
    paddle = std::make_unique<Paddle>(renderer, 640, 480); // Créer le Paddle
    //ball = std::make_unique<Ball>(320, 435, 10); 
    balls.push_back(std::make_unique<Ball>(320, 435, 10));
    // bonus(generateRandomNumber(20, 600), false, 10, MALUS, DECREASE_PADDLE, false)
    auto levelData = loadLevel("level/1.txt");
        if (levelData.empty()) {
            std::cerr << "Le fichier est vide" << std::endl;
            return; 
        }
    initBricks(levelData);
    initBricksH(levelData);
    initBonus();
}


/*
    Fonction qui permet de réinitialiser l'état du jeu.
    Réinitialise la position de la balle, du paddle, et d'autres états de jeu
    pour commencer un nouveau niveau.

*/
void Game::resetGameState() {
    // Réinitialiser ou configurer d'autres états de jeu nécessaires
    //ball->reset(320, 435, 10); // Supposons que `reset` repositionne et remet la balle en jeu
    //countdown=0;
   // paddle->render(renderer); // Supposons une méthode de réinitialisation pour le paddle
    
    for (auto& b : balls) {
        b->reset(320, 435, 10); // Réinitialisation de chaque balle
    }
    gameStarted = false;  // Prêt à redémarrer le jeu pour le nouveau niveau
}

// Callback du timer
static Uint32 changeLevelCallback(Uint32 interval, void* param) {
    Game* game = static_cast<Game*>(param);
    game->loadNextLevel();
    return 0;  // Arrêter le timer
}

static Uint32 countdownTimerCallback(Uint32 interval, void* param) {
    Game* game = static_cast<Game*>(param);
    if (game->isCounterPaused) {
        return 0;  // Ne pas rappeler le timer si en pause
    }
    game->countdown--;
    if (game->countdown > 0) {
        return 1000;  // Continue à appeler le timer toutes les secondes
    } else {
        game->loadNextLevel();  // Charger le niveau suivant lorsque le compteur atteint 0
        return 0;  // Arrêter le timer
    }
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
    if (activeCountBrick == 0 || activeCountBrickH == 0) {
        countdown = 5;  // Commence le compteur à 3 secondes
        gameState=CHANGEMENT_NIVEAU;
        SDL_AddTimer(1000, countdownTimerCallback, this);  // Démarre le timer pour décompter chaque seconde
    }
    for (auto& b : balls) {
        b->update(gameStarted, win.getWinWidth(), win.getWinHeight(), paddle->getRect());
    }
}

/*
    Fonction qui permet de charger le niveau suivant.
    Charge le prochain niveau à partir d'un fichier texte.
    Si tous les niveaux sont atteints, déclare la fin du jeu.

*/
void Game::loadNextLevel() {
    niveauActuel++;
    std::string nextLevelFilename = "level/"+std::to_string(niveauActuel)+".txt";
   // std::string nextLevelFilename = getNextLevelFilename(); 
    //auto levelData = loadLevel(getNextLevelFilename());
    auto levelData = loadLevel(nextLevelFilename);
    if (!levelData.empty()) {
        initBricks(levelData);
        initBricksH(levelData);
        resetGameState();
        gameState = JEU_EN_COURS;
    } else {
        gameState = TOUS_NIVEAUX_ATTEINTS;
    }
}

/*
    Fonction qui permet d'afficher le niveau actuel à l'écran.
    Utilise la bibliothèque SDL_ttf pour afficher du texte à l'écran.

*/
void Game::displayLevel() {
    TTF_Font* font = TTF_OpenFont("./fonts/Roboto-Medium.ttf", 24);
    if (!font) {
        std::cerr << "Erreur lors du chargement de la police : " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color textColor = {255, 255, 255};  // Blanc
    std::string levelText = "Niveau: " + std::to_string(niveauActuel);
    SDL_Surface* surface = TTF_RenderText_Solid(font, levelText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int text_width = 0, text_height = 0;
    SDL_QueryTexture(texture, NULL, NULL, &text_width, &text_height);
    SDL_Rect renderQuad = {win.getWinWidth() - text_width - 10, 10, text_width, text_height};

    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}


/*
    Fonction qui permet d'afficher le compte à rebours pour le prochain niveau.
    Utilise la bibliothèque SDL_ttf pour afficher du texte à l'écran.
*/
void Game::displayNextLevelCountdown() {
    if (gameState != CHANGEMENT_NIVEAU) return; // Assurez-vous que le jeu est dans le bon état

    TTF_Font* font = TTF_OpenFont("./fonts/Roboto-Medium.ttf", 24);
    SDL_Color textColor = {255, 255, 255}; // Blanc
    std::string countdownText = "Starting in: " + std::to_string(countdown);
    SDL_Surface* surface = TTF_RenderText_Solid(font, countdownText.c_str(), textColor);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    int textWidth, textHeight;
    SDL_QueryTexture(texture, NULL, NULL, &textWidth, &textHeight);
    SDL_Rect renderQuad = { (win.getWinWidth() - textWidth) / 2, (win.getWinHeight() - textHeight) / 2, textWidth, textHeight };

    SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}


/*
    Destructeur de la classe Game. Nettoie les ressources en supprimant
    le paddle et la balle et en fermant SDL proprement.
*/
Game::~Game() {
   SDL_DestroyRenderer(renderer);   
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

// void Game::activateMultiball() {
//     if (!balls.empty()) {  // Vérifiez s'il y a déjà des balles pour éviter les erreurs.
//         auto newBall = std::make_unique<Ball>(*balls.front());  // Utiliser la première balle comme modèle
//         // Vous pouvez ajuster la vélocité pour différencier légèrement la nouvelle balle.
//         newBall->setVel(newBall->getVelX()+2 , newBall->getVelY());  // Modifier légèrement la vélocité
//         balls.push_back(std::move(newBall));
//     }
// }

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

    // Afficher un message pour changer de niveau
    SDL_Surface* nextLevelSurface = TTF_RenderText_Solid(font, "Next Level", textColor);
    SDL_Texture* nextLeveltexture = SDL_CreateTextureFromSurface(renderer, nextLevelSurface);
    SDL_Rect nextLevelReact = {0, 0, 300, 100};
    nextLevelReact.x = (win.getWinWidth() - nextLevelReact.w) / 2;
    nextLevelReact.y = (win.getWinHeight() - nextLevelReact.h) / 2 - 80;

    // Afficher messages niveaux atteint
    SDL_Surface* allLevelsCompleteSurface = TTF_RenderText_Solid(font, "Tous les niveaux ont ete atteints!", textColor);
    SDL_Texture* allLevelsCompleteTexture = SDL_CreateTextureFromSurface(renderer, allLevelsCompleteSurface);
    SDL_Rect allLevelsCompleteRect = {0, 0, 500, 200};
    allLevelsCompleteRect.x = (win.getWinWidth() - allLevelsCompleteRect.w) / 2;
    allLevelsCompleteRect.y = (win.getWinHeight() - allLevelsCompleteRect.h) / 2 - 80;
   
    // Afficher message de pause
    SDL_Surface* pauseMessage = TTF_RenderText_Solid(font, "Appuyez sur entree pour mettre pause", messageColor);
    SDL_Texture* pauseTexture = SDL_CreateTextureFromSurface(renderer, pauseMessage);
    SDL_Rect pauseMessageReact = {0, 0, 400, 50};
    pauseMessageReact.x = (win.getWinWidth() - pauseMessageReact.w) / 2;
    pauseMessageReact.y = (win.getWinHeight() - pauseMessageReact.h) / 2 + 40;
    
    //Afficher message pour hexagone
    TTF_Font* font_hexagone = TTF_OpenFont("./fonts/Roboto-Medium.ttf", 24);
    SDL_Surface* hexagoneMessage = TTF_RenderText_Solid(font_hexagone, "Appuyez sur H pour activer les hexagones", {255,255,255});
    SDL_Texture* hexagoneTexture = SDL_CreateTextureFromSurface(renderer, hexagoneMessage);
    SDL_Rect hexagoneMessageReact = {0, 0, 300, 30};
    hexagoneMessageReact.x = (win.getWinWidth() - hexagoneMessageReact.w) / 2;
    hexagoneMessageReact.y = (win.getWinHeight() - hexagoneMessageReact.h) / 2 + 100;

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
                        // case SDLK_SPACE:
                        //     if (gameState == JEU_EN_COURS) {
                        //         if (!gameStarted) {
                        //             ball->launch();
                        //             // for(auto& ball : ball){
                        //             //     ball->launch();
                        //             // }
                        //             gameStarted = true;
                        //         }
                        //     }else if (gameState == MENU) {
                            
                        //         gameState = JEU_EN_COURS;
                        //     }
                        //     break;
                        case SDLK_SPACE:
                            if (gameState == JEU_EN_COURS) {
                                if (!gameStarted) {
                                    for (auto& b : balls) { b->launch(); }
                                    gameStarted = true;
                                }
                            } else if (gameState == MENU) {
                                gameState = JEU_EN_COURS;
                            }
                            break;
                        case SDLK_ESCAPE:
                            if (gameState == GAME_OVER) {
                                jeuTourne = false; 
                            }

                        break;
                        case SDLK_h:

                            if (gameState == MENU) {
                                brickOrH = true;
                                gameState = JEU_EN_COURS;
                            }

                        break;
                        case SDLK_RETURN:
                            if (gameState == CHANGEMENT_NIVEAU) {
                                isCounterPaused = !isCounterPaused;  // Bascule l'état de pause
                                if (isCounterPaused) {
                                    // Supprimer le timer si en pause
                                    SDL_RemoveTimer(countdown);
                                } else {
                                    // Recréer le timer si reprise
                                    countdown = SDL_AddTimer(1000, countdownTimerCallback, this);
                                }
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
                SDL_RenderCopy(renderer,hexagoneTexture, nullptr, &hexagoneMessageReact);
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

                displayLevel();

                if (brickOrH == true) {
                    for (auto& brickH : bricksH) {
                        if (brickH.isActive() ) {
                           brickH.render( renderer);
                        }
                    }
                    for(auto& brickH : bricksH){
                        if (brickH.isActive()) {
                            for(auto& ball : balls){
                                SDL_Rect ballRect = ball->getRect();
                                if (brickH.checkCollision(ballRect)) {
                                    ball->reverseYVelocity();  // Inverser la vélocité de la balle après collision.
                                    if (!brickH.isActive()) {  // Vérifiez si la brique est devenue inactive après cette collision.
                                        activeCountBrickH--;  // Décrémentez seulement si la brique est désormais inactive.
                                    }
                                }
                            }
                            // SDL_Rect ballRect = ball->getRect();
                            // if (brickH.checkCollision(ballRect)) {
                            //     ball->reverseYVelocity();  // Inverser la vélocité de la balle après collision.
                            //     if (!brickH.isActive()) {  // Vérifiez si la brique est devenue inactive après cette collision.
                            //         activeCountBrickH--;  // Décrémentez seulement si la brique est désormais inactive.
                            //     }
                            // }
                        }
                    }
                } else {
                
                    //Dessiner les bricks active
                    for (auto& brick : bricks) {
                        if (brick.isActive()) {
                            brick.render(renderer);
                        }
                    }
                    // Vérifiez la collision de la balle avec chaque brique active
                    for (auto& brick : bricks) {
                        if (brick.isActive()) {
                            for(auto& ball : balls){
                                SDL_Rect ballRect = ball->getRect();
                                if (brick.checkCollision(ball)) {
                                    // ball->reverseYVelocity();  // Inverser la vélocité de la balle après collision.
                                    if (!brick.isActive()) {  // Vérifiez si la brique est devenue inactive après cette collision.
                                        activeCountBrick--;  // Décrémentez seulement si la brique est désormais inactive.
                                    }
                                }
                            }
                            // SDL_Rect ballRect = ball->getRect();
                            // if (brick.checkCollision(ballRect)) {
                            //     ball->reverseYVelocity();  // Inverser la vélocité de la balle après collision.
                            //     if (!brick.isActive()) {  // Vérifiez si la brique est devenue inactive après cette collision.
                            //         activeCountBrick--;  // Décrémentez seulement si la brique est désormais inactive.
                            //     }
                            // }
                        }
                    }
                }
               

            
                paddle->render(renderer);// Dessine le paddle
                // ball->render(renderer); // Dessine la balle
                // //permet de mettre à jour la position de la balle quand le paddle est en mouvement
                // ball->update(gameStarted, win.getWinWidth(), win.getWinHeight(), paddle->getRect()); 
                //for (auto& b : balls) {  }
                for (auto& b : balls) {
                    b->render(renderer);
                    b->update(gameStarted, win.getWinWidth(), win.getWinHeight(), paddle->getRect());
                }
                //Permet la collision entre la balle et les briques
                
              //  for(auto& ball:balls){ballRect=ball-> getRect();}


              

                //Fonction qui permet de mettre à jour quand on passe au prochain niveau
                updateGameLogic();
               
                
                SDL_Rect newPaddleSize;

                switch (balls.front()->getVies()) {
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
                               for(auto& ball:balls){ball->setVel(ball->getVelX() + 1, ball->getVelY() - 1);}
                            //    for(auto& ball:balls){
                            //     ball->adjustSpeed(-1);
                            //    }
                                
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
                                for(auto& ball:balls){
                                 ball->setVel(ball->getVelX() - 1, ball->getVelY() + 1);
                               // ball->adjustSpeed(1);
                                }
                                break;
                            case MULTIBALL:
                                    bonus.activateMultiball(balls, renderer);
                                    bonus.setUsed(true);
                                    bonus.setActivationBonus(false);
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

            case CHANGEMENT_NIVEAU:
                SDL_RenderCopy(renderer, pauseTexture, NULL, &pauseMessageReact);
                SDL_RenderCopy(renderer, nextLeveltexture, nullptr, &nextLevelReact);
                displayNextLevelCountdown(); // Cette fonction sera appelée chaque seconde grâce au timer
                SDL_RenderPresent(renderer);
                break;


            case TOUS_NIVEAUX_ATTEINTS:
                SDL_RenderCopy(renderer, allLevelsCompleteTexture, nullptr, &allLevelsCompleteRect);
                SDL_RenderPresent(renderer);
                break;
        }

        // Introduit un léger délai de 10 millisecondes entre chaque itération de la boucle de jeu.
        SDL_Delay(20); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}
