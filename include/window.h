#include <stdio.h>
#include <SDL2/SDL.h>

#ifndef WINDOW_H
#define WINDOW_H



class Window {
public:
    Window();
    ~Window();

    int init(const char* title, int width, int height);
    void quit();
	SDL_Renderer* getRenderer() const;

private:
    SDL_Window* fenetre;
	SDL_Renderer* renderer;
};

#endif // WINDOW_H
