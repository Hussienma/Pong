#pragma once
#include <SDL2/SDL_events.h>
#include <queue>

#include "Index.hpp"

class Controller {
	public:
	static SDL_Event event;
	static ControllerKey input;
	static std::queue<bufferedInput> inputBuffer;
	Controller();
	static void handleInput();
	static ControllerKey getInputFromBuffer();
	static void updateBuffer();
};
