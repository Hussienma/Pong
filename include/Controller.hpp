#pragma once
#include <SDL2/SDL_events.h>
#include <queue>
#include <string>

#include "Index.hpp"

class Controller {
	public:
	static SDL_Event event;
	static ControllerKey input;
	static std::queue<bufferedInput> serverBuffer;
	static std::queue<bufferedInput> clientBuffer;
	Controller();
	static void handleInput();
	static std::string getInputServer();
	static ControllerKey getInputClient();
	static void confirmInput(int number);
	static void updateBuffer();
};
