#ifndef BONUSMALUS_H
#define BONUSMALUS_H

#include <SDL2/SDL.h>
#include "window.h"
#include "ball.h"

enum Bonus{
    MALUS,
    INCREASE_PADDLE,
    DECREASE_SPEED_BALL
};


enum Malus{
    BONUS,
    DECREASE_PADDLE,
    INCREASE_SPEED_BALL,
    MULTIBALL
};


class BonusMalus {
public:

    BonusMalus(int x, bool bonus, int radius, Bonus bon, Malus mal, bool act);
    ~BonusMalus();
    void render(SDL_Renderer* renderer);
    void update(int windowWidth, int windowHeight, SDL_Rect paddleRect);
    bool getActivationBonus();
    bool getUsed();
    void setUsed(bool us);
    void setActivationBonus(bool act);
    void setActive(bool act);
    Bonus getBonusType();
    Malus getMalusType();

private:
    
    SDL_Point center;
    int radius;
    int velX, velY;
    bool bonus;
    //Pour afficher le bonus/malus ou non
    bool active;
    //Pour lancer le malus (type augmenter la taille,...) 
    bool activationBonus;
    // Pour savoir si le bonus a ete utilise et ne pas le reutiliser
    bool used;

    Bonus bon;
    Malus mal;

};

#endif // !BONUSMALUS
