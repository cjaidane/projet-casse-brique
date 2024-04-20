#ifndef PADDLE_H
#define PADDLE_H

#include <SDL2/SDL.h>

class Paddle {
public:
    Paddle(SDL_Renderer* renderer, int window_width, int window_height);
    ~Paddle();
    void render(SDL_Renderer* renderer);
    void moveLeft();
    void moveRight();
    void moveTo(int x);
    SDL_Rect getRect() const;

private:    
    SDL_Rect paddleRect;
    int window_width;
    int window_height;
    int paddle_speed;
};

#endif // PADDLE_H
