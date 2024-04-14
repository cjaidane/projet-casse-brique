#include "../include/game.h"

void Game::initBricks() {
    const int brickRows = 5, brickColumns = 10;
    const int brickWidth = 64, brickHeight = 20, brickPadding = 5;
    const int startX = (640 - (brickColumns * (brickWidth + brickPadding))) / 2;
    const int startY = 50;
    for (int row = 0; row < brickRows; ++row) {
        for (int col = 0; col < brickColumns; ++col) {
            int x = startX + col * (brickWidth + brickPadding);
            int y = startY + row * (brickHeight + brickPadding);
            bricks.emplace_back(x, y, brickWidth, brickHeight); // Pas besoin de résistance ici car elle n'est pas utilisée
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



