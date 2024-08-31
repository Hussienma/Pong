#include <SDL2/SDL.h>
#include <iostream>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_timer.h>
#include <thread>

#include "Constants.h"
#include "OnlineGame.hpp"
#include "RenderWindow.hpp"
#include "Game.hpp"
#include "Controller.hpp"

enum ApplicationState { MAIN_MENU, OFFLINE_GAME, ONLINE_GAME };

// TODO: create a main menu

int main(){
	if(SDL_Init(SDL_INIT_EVERYTHING) == -1){
		std::cerr<<"Faild to initialize SDL: "<<SDL_GetError()<<std::endl;
		exit(1);
	}

	if(TTF_Init() < 0){
		std::cerr<<"Faild to initialize SDL TTF: "<<SDL_GetError()<<std::endl;
		exit(1);
	}

	RenderWindow window(WINDOW_WIDTH, WINDOW_HEIGHT);
	ApplicationState state = ONLINE_GAME;
	Controller controller;
	// OfflineGame game(window);
	OnlineGame onlineGame(window);

	const float timestep = 0.01f;
	float accumulator = 0.0f;
	int currentTime = SDL_GetTicks(); 
	int lastFrameTime = SDL_GetTicks();
	int numberOfFrames = 0;
	
	while(Controller::event.type != SDL_QUIT){
		int newTime = SDL_GetTicks();

		while(accumulator > timestep){
			window.clear();
			switch(state){
				case MAIN_MENU:
					// TODO: Main menu
					break;
				// case OFFLINE_GAME:
				// 	game.update();
				// 	break;
				case ONLINE_GAME:
					onlineGame.update();
					break;
				default:
					break;
			}

			window.display();

			lastFrameTime = SDL_GetTicks();
			numberOfFrames++;
			accumulator -= timestep;
		}

		int frameTime = newTime - currentTime;

		currentTime = newTime;
		
		Game::deltaTime = (SDL_GetTicks() - lastFrameTime)*0.001;
		accumulator += frameTime*0.001;

		Controller::handleInput();
		if(frameTime < (float)1000/window.getWindowRefreshrate())
			SDL_Delay((float)1000/window.getWindowRefreshrate()-frameTime);
	}

	window.cleanUp();

	TTF_Quit();
	SDL_Quit();

	return 0;
}
