#include "InputComponent.hpp"
#include "GameObject.hpp"

void PlayerInputComponent::update(GameObject& obj){
	switch(Controller::event.type){
		case SDL_KEYDOWN:
			switch(Controller::event.key.keysym.sym){
				case SDLK_UP:
					obj.velocityY = -500;
					break;
				case SDLK_DOWN:
					obj.velocityY = 500;
					break;
			}
			break;
		case SDL_KEYUP:
			obj.velocityY = 0;
			break;
	}
}
