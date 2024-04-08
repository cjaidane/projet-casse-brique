#include "../include/game.h"

Game::Game() : jeuTourne(false), paddle(nullptr) {
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique", 640, 480);
    renderer = win.getRenderer(); // Suppose que Window a une méthode pour obtenir le renderer
    paddle = new Paddle(renderer, 640, 480); // Créer le Paddle
    ball = new Ball(320, 480 - 55, 10); 
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
            switch(evenement.type) {
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
                            if(!gameStarted) {
                                ball->launch();
                                gameStarted = true;
                            }
                    }
                    break;
                case SDL_MOUSEMOTION:
                    paddle->moveTo(evenement.motion.x);
                    break;
            }   

            
            // Gestion des événements pour déplacer le Paddle
            if (evenement.type == SDL_KEYDOWN) {
                switch (evenement.key.keysym.sym) {
                    case SDLK_LEFT:
                        paddle->moveLeft();
                        break;
                    case SDLK_RIGHT:
                        paddle->moveRight();
                        break;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        paddle->render(); // Dessiner le Paddle
        ball->update(gameStarted, 640, 480, paddle->getRect()); 
        ball->render(renderer);// Mettre à jour la position de la balle
        SDL_RenderPresent(renderer);

        SDL_Delay(10); // Léger délai
        
    }

    win.quit(); // Nettoyer et quitter
}
