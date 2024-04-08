#include "../include/game.h"
#include <iostream>

Game::Game() : jeuTourne(false), paddle(nullptr) {
    // Initialiser Window et SDL avant d'initialiser Paddle
    win.init("Casse Brique", 640, 480);
    renderer = win.getRenderer(); // Suppose que Window a une méthode pour obtenir le renderer
    paddle = new Paddle(renderer, 640, 480); // Créer le Paddle
}

Game::~Game() {
    delete paddle;
    
}

void Game::run() {
    if (!renderer) {
        return;
    }

    jeuTourne = true;
    while (jeuTourne) {
        SDL_Event evenement;
        while (SDL_PollEvent(&evenement) != 0) {
            if (evenement.type == SDL_QUIT) {
                jeuTourne = false;
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
            switch(evenement.type){
                case SDL_QUIT:
                    jeuTourne = false;
                    break;
                case SDL_MOUSEMOTION:
                    paddle->moveTo(evenement.motion.x);
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond noir
        SDL_RenderClear(renderer);

        paddle->render(); // Dessiner le Paddle

        SDL_RenderPresent(renderer);

        SDL_Delay(10); // Léger délai
    }

    win.quit(); // Nettoyer et quitter
}
