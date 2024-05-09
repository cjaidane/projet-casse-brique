#include <SDL2/SDL.h>
#include "window.h"

enum Bonus{
    MALUS,
    INCREASE_PADDLE
};


enum Malus{
    BONUS,
    DECREASE_PADDLE
};


class BonusMalus {
public:

    BonusMalus(int x, bool bonus, int radius, Bonus bon, Malus mal, bool act);
    ~BonusMalus();
    void render(SDL_Renderer* renderer);
    void update(int windowWidth, int windowHeight, SDL_Rect paddleRect);
    bool getActivationBonus();
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

    Bonus bon;
    Malus mal;

};

