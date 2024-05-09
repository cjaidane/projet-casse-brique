#ifndef WINDOW_H
#define WINDOW_H

#include <SDL2/SDL.h>

class Window {
public:
    Window();
    ~Window();

    int init(const char* title);
    void quit();
    SDL_Renderer* getRenderer() const; // Ajout de getRenderer

    int getWinHeight();
    int getWinWidth();
private:
    SDL_Window* fenetre;
    SDL_Renderer* renderer; // Ajout d'un pointeur vers le renderer
    int windowHeight;
    int windowWidth;
};

#endif // WINDOW_H
