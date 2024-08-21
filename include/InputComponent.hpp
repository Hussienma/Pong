#pragma once

#include <SDL2/SDL.h>

#include "GameObject.hpp"
#include "Controller.hpp"

class InputComponent {
	public:
	InputComponent(){}
	virtual void update(GameObject& obj){}
};

class PlayerInputComponent: public InputComponent {
	public:
	PlayerInputComponent(){}
	void update(GameObject& obj) override;
};

class OnlineInputComponent: public InputComponent {
	public:
	OnlineInputComponent(){}
	void update(GameObject& obj) override;
};

class AiInputComponent: public InputComponent {
	public:
	AiInputComponent(){}
	void update(GameObject& obj) override;
};
