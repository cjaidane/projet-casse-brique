#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include "./window.h"

class Game {
public:
	Game();
	Game(Game  &&) = default;
	Game(const Game  &) = default;
	Game &operator=(Game  &&) = default;
	Game &operator=(const Game  &) = default;
	~Game();

	void run();

private:
	Window win;	

	bool jeuTourne;
};

#endif // !GAME_H
