#ifndef BALL_H
#define BALL_H

#include <SDL2/SDL.h>

class Ball {
public:
    Ball(int x, int y, int radius);
    void update(bool& gameStarted, int windowWidth, int windowHeight, SDL_Rect paddleRect);
    void render(SDL_Renderer* renderer);
    // C'est la vitesse de la balle
    void launch();
    //void setPosition(int x, int y);
    bool checkCollision(SDL_Rect paddleRect) const;
    // Pour réinitialiser la position de la balle
    void reset(int paddleX, int paddleY, int paddleWidth);
    // Pour inverser la vitesse verticale de la balle
    void reverseYVelocity();
    SDL_Rect getRect() const;

    void setVel(int velX, int velY);

    int getVelX();
    int getVelY();

    int getVies();

private:
    
    SDL_Point center;
    int radius;
    int velX, velY;
    int vies;
};


#endif // BALL_H

