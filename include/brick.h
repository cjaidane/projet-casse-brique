#ifndef BRICK_H
#define BRICK_H

#include <SDL2/SDL.h>
#include <memory>
#include <vector>
#include "ball.h"


class Brick {
public:
    Brick(int x, int y, int w, int h, int resistance);
    void render(SDL_Renderer* renderer) const;
    bool checkCollision(std::unique_ptr<Ball>& ball);
    bool isActive() const;
  

private:
    SDL_Rect rect;
    bool active; // Ajout de l'état actif qui sert à savoir si elles sont encore actives
    int resistance; // Ajout de la résistance qui indique combien de fois elles doivent être touchées avant d'être détruites
    
};

#endif // BRICK_H

