#include "Game.hpp"
#include "InputComponent.hpp"
#include "Controller.hpp"
#include "GameObject.hpp"
#include "Index.hpp"
#include <cstdlib>

void PlayerInputComponent::update(GameObject& obj){
	switch(Controller::event.type){
		case SDL_KEYDOWN:
			switch(Controller::event.key.keysym.sym){
				case SDLK_UP:
					obj.velocityY = -180;
					break;
				case SDLK_DOWN:
					obj.velocityY = 200;
					break;
			}
			break;
		case SDL_KEYUP:
			obj.velocityY = 0;
			break;
	}
}

void OnlineInputComponent::update(GameObject& obj){
	// TODO: Make client side predictions for the trajectory
	
	switch(Controller::getInputClient()){
		case UP_KEY:
			obj.velocityY = -500;
			break;
		case DOWN_KEY:
			obj.velocityY = 500;
			break;
		case KEY_UP:
			obj.velocityY = 0;
			break;
		default:
			break;
	}
}

void AiInputComponent::update(GameObject& obj){
	SDL_Rect ball = Game::ball.position;
	if(obj.position.y + obj.position.h/2 > ball.y)
		obj.velocityY = -400;
	else if(obj.position.y + obj.position.h/2 < ball.y)
		obj.velocityY = 400;
	else
		obj.velocityY = 20;
}
