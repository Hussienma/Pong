#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>

#include "Constants.h"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "Controller.hpp"

int main(){
	if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
		std::cerr<<"Faild to initialize SDL: "<<SDL_GetError()<<std::endl;

	if(TTF_Init() < 0)
		std::cerr<<"Faild to initialize SDL TTF: "<<SDL_GetError()<<std::endl;

	RenderWindow window(WINDOW_WIDTH, WINDOW_HEIGHT);
	Controller controller;
	Game game(window);

	const float timestep = 0.01f;
	float accumulator = 0.0f;
	int currentTime = SDL_GetTicks(); 
	
	while(Game::state != QUIT){
		int newTime = SDL_GetTicks();
		int frameTime = newTime - currentTime;

		if(frameTime < (float)1000/window.getWindowRefreshrate())
			SDL_Delay((float)1000/window.getWindowRefreshrate()-frameTime);

		currentTime = newTime;
		float deltaTime = frameTime*0.001;
		Game::deltaTime = deltaTime;
		accumulator += deltaTime;

		while(accumulator > timestep){
			window.clear();
			controller.handleInput();
			game.update();

			window.display();

			accumulator -= timestep;
		}
	}

	TTF_Quit();
	SDL_Quit();

	return 0;
}
