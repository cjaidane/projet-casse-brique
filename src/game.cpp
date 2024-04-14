#include "../include/game.h"

void Game::initBricks() {
    const int windowWidth = 640;  // Largeur de la fenêtre
    const int startY = 50;        // Début des briques à partir du haut de la fenêtre
    const int brickHeight = 20;   // Hauteur de chaque brique
    const int padding = 5;        // Espace entre les briques
    const int rows = 4;           // Nombre de lignes de briques

    // Calculer le nombre de colonnes et la largeur des briques pour qu'elles remplissent toute la largeur
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

Game::Game() : jeuTourne(false), paddle(nullptr), ball(nullptr) {
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique", 640, 480);
    renderer = win.getRenderer(); // Suppose que Window a une méthode pour obtenir le renderer
    paddle = new Paddle(renderer, 640, 480); // Créer le Paddle
    ball = new Ball(320, 435, 10); 
    initBricks();
}

Game::~Game() {
    delete paddle; // Libérer la mémoire allouée au Paddle
}

void Game::run() {
    if (!renderer) {
        return;
    }
    jeuTourne = true;
    bool gameStarted = false;
    SDL_Event evenement;
    while (jeuTourne) {
        while (SDL_PollEvent(&evenement) != 0) {
            switch (evenement.type) {
                case SDL_QUIT:
                    jeuTourne = false;
                    break;
                case SDL_KEYDOWN:
                    switch (evenement.key.keysym.sym) {
                        case SDLK_LEFT:
                            paddle->moveLeft();
                            break;
                        case SDLK_RIGHT:
                            paddle->moveRight();
                            break;
                        case SDLK_SPACE:
                            if (!gameStarted) {
                                ball->launch();
                                gameStarted = true;
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
        SDL_RenderClear(renderer);

        for (auto& brick : bricks) {
                if (brick.isActive()) {
                    brick.render(renderer);
                }
            }
        // Dessiner le Paddle
        paddle->render();
        // Mettre à jour et dessiner la Balle
        ball->update(gameStarted, 640, 480, paddle->getRect()); 
        SDL_Rect ballRect =ball-> getRect();

        for (auto& brick : bricks) {
            if (brick.isActive() && brick.checkCollision(ballRect)) {
                // Réagir à la collision, par exemple inverser la direction de la balle
                ball->reverseYVelocity();  // Supposons que vous avez une méthode pour inverser la vitesse Y de la balle
                break;  // Supposer que la balle ne peut toucher qu'une brique à la fois
            }
        }
        ball->render(renderer);
       
        SDL_RenderPresent(renderer);
        SDL_Delay(10); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}



