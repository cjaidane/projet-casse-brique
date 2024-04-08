#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

class Ball {
public:
    Ball(int x, int y, int radius);
    void update(bool gameStarted, int windowWidth, int windowHeight, SDL_Rect paddleRect);
    void render(SDL_Renderer* renderer);
    void launch();
    void setPosition(int x, int y);

private:
    SDL_Point center;
    int radius;
    int velX, velY;
};

#endif // BALL_H
