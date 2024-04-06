#include "../include/game.h"
#include <cstdio>

Game::Game() {
    this->win = Window();
    this->jeuTourne = false;
}

Game::~Game() {
}

void Game::run() {
    this->win.init();

    this->jeuTourne = true;
    while (this->jeuTourne) {
    
        SDL_Event evenement;
        while (SDL_PollEvent(&evenement) != 0) {
            if (evenement.type == SDL_QUIT) {
                printf("Bonjour");
                this->jeuTourne = false;
            }
        SDL_Delay(10); // Ajout d'un léger délai pour éviter la surcharge CPU
        }
    }

    // Libération des ressources
    this->win.quit();
}
