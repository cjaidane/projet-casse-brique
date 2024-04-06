#include <stdio.h>
#include <SDL2/SDL.h>

class Window {
public:
	Window();
	Window(Window &&) = default;
	Window(const Window &) = default;
	Window &operator=(Window &&) = default;
	Window &operator=(const Window &) = default;
	~Window();

	int init();
	int quit();
private:
	SDL_Window* fenetre;
};

