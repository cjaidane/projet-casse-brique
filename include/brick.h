#ifndef BRICK_H
#define BRICK_H

#include <SDL2/SDL.h>

class Brick {
public:
    Brick(int x, int y, int w, int h);
    void render(SDL_Renderer* renderer) const;
    bool checkCollision(const SDL_Rect& ballRect);
    bool isActive() const;
    void deactivate();

private:
    SDL_Rect rect;
    bool active;
};

#endif // BRICK_H
