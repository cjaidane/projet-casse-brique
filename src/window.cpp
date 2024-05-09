#include "../include/window.h"

Window::Window() : fenetre(nullptr), renderer(nullptr), windowHeight(480), windowWidth(640) {
}

Window::~Window() {
    quit();
}

int Window::init(const char* title) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("Erreur lors de l'initialisation de SDL : %s", SDL_GetError());
        return -1;
    }

    fenetre = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
    if (!fenetre) {
        SDL_Log("Erreur lors de la création de la fenêtre : %s", SDL_GetError());
        SDL_Quit();
        return -1;
    }

    // Création du renderer
    renderer = SDL_CreateRenderer(fenetre, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur lors de la création du renderer : %s", SDL_GetError());
        SDL_DestroyWindow(fenetre);
        SDL_Quit();
        return -1;
    }

    return 0;
}

void Window::quit() {
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
    }
    if (fenetre != nullptr) {
        SDL_DestroyWindow(fenetre);
    }
    SDL_Quit();
}

SDL_Renderer* Window::getRenderer() const {
    return renderer;
}

int Window::getWinWidth(){
    return windowWidth;
}

int Window::getWinHeight(){
    return windowHeight;
}
