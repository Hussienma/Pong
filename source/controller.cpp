#include "Controller.hpp"
#include "Game.hpp"

Controller::Controller(){}

SDL_Event Controller::event;

void Controller::handleInput(){
	SDL_PollEvent(&event);
	if(event.type == SDL_QUIT)
		Game::state = QUIT;
}
