#include "Controller.hpp"
#include "Index.hpp"
#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>

Controller::Controller(){}

SDL_Event Controller::event;
ControllerKey Controller::input;
std::queue<bufferedInput> Controller::serverBuffer;
std::queue<bufferedInput> Controller::clientBuffer;

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
	serverBuffer.push(keypressed);
	clientBuffer.push(keypressed);
}

// TODO: All inputs should be sent to the server
std::string Controller::getInputServer(){
	std::queue<bufferedInput>* temp = new std::queue<bufferedInput>(serverBuffer);

	std::string result;
	for(int i=0; i < 5 && !temp->empty(); ++i, temp->pop()){
		result += std::to_string(temp->front().key);
	}

	delete temp;

	return result;
}

ControllerKey Controller::getInputClient(){
	ControllerKey result;
	if(!clientBuffer.empty()){
		result = clientBuffer.front().key;
	}
	else
		result = KEY_UP;

	if(!clientBuffer.empty())
		clientBuffer.pop();

	return result;
}

void Controller::confirmInput(int number){
	for(int i = 0; i<number && !serverBuffer.empty(); ++i, serverBuffer.pop());
}

void Controller::updateBuffer(){
	uint32_t currentTime = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	for(; !serverBuffer.empty() && (serverBuffer.front().key == KEY_UP && currentTime - serverBuffer.front().timestamp > 60); serverBuffer.pop());
	for(; !clientBuffer.empty() && (clientBuffer.front().key == KEY_UP || currentTime - clientBuffer.front().timestamp > 60); clientBuffer.pop());
}
