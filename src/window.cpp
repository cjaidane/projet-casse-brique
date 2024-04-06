#include "../include/window.h"
#include <stdio.h>

Window::Window() {
    this->fenetre = SDL_CreateWindow("Clement le bg",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          640, 480,
                                          SDL_WINDOW_SHOWN);
}

Window::~Window() {
}

int Window::init() {
    
    // Initialisation de SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return -1;
    }

    // Création de la fenêtre
    if (!this->fenetre) {
        SDL_Log("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    return 0;
}

int Window::quit() {
    SDL_DestroyWindow(this->fenetre);
    SDL_Quit();
    return 0;
}
