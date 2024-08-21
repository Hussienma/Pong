#include "Controller.hpp"
#include "Index.hpp"
#include <chrono>
#include <cstdint>
#include <ctime>

Controller::Controller(){}

SDL_Event Controller::event;
ControllerKey Controller::input;
std::queue<bufferedInput> Controller::inputBuffer;

void Controller::handleInput(){
	SDL_PollEvent(&event);
	switch(event.type){
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_UP:
					input = UP_KEY;
					break;
				case SDLK_DOWN:
					input = DOWN_KEY;
					break;
			}
			break;
		case SDL_KEYUP:
			input = KEY_UP;
			break;
	}
	updateBuffer();

	uint32_t timestamp = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	bufferedInput keypressed;
	keypressed.key = input;
	keypressed.timestamp = timestamp;
	inputBuffer.push(keypressed);
}

// TODO: All inputs should be sent to the server
ControllerKey Controller::getInputFromBuffer(){
	ControllerKey result;
	if(!inputBuffer.empty()){
		result = inputBuffer.front().key;
		inputBuffer.pop();
	}
	else
		result = KEY_UP;


	return result;
}

void Controller::updateBuffer(){
	uint32_t currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	for(; !inputBuffer.empty() && currentTime - inputBuffer.front().timestamp > 48; inputBuffer.pop());
}
