#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window {
public:
    Window();
    ~Window();

    int init(const char* title, int width, int height);
    void quit();
    SDL_Renderer* getRenderer() const; // Ajout de getRenderer

private:
    SDL_Window* fenetre;
    SDL_Renderer* renderer; // Ajout d'un pointeur vers le renderer
};

#endif // WINDOW_H
