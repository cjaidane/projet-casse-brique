#ifndef GAME_H
#define GAME_H

#include "paddle.h"
#include "window.h"

class Game {
public:
    Game();
    ~Game();
    void run();

private:
    Window win;
    bool jeuTourne;
	Paddle* paddle;
	SDL_Renderer* renderer;
};

#endif // GAME_H
