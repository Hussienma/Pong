#include <SDL2/SDL_events.h>

class Controller {
	public:
	static SDL_Event event;
	Controller();
	static void handleInput();
};
