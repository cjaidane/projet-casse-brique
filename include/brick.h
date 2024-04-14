#ifndef BRICK_H
#define BRICK_H

#include <SDL2/SDL.h>

class Brick {
public:
    Brick(int x, int y, int w, int h, int resistance);
    void render(SDL_Renderer* renderer) const;
    bool checkCollision(const SDL_Rect& ballRect);
    bool isActive() const;
    void deactivate();

private:
    SDL_Rect rect;
    int resistance; // Ajout de la résistance
    bool active;
    float initialResistance ; // Résistance initiale des briques
};

#endif // BRICK_H
