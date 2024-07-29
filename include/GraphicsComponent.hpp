#pragma once

#include <SDL2/SDL.h>

#include "GameObject.hpp"

class GraphicsComponent {
	public:
	SDL_Texture* texture = nullptr;
	GraphicsComponent(){}
	GraphicsComponent(SDL_Texture* texture): texture(texture){}
	virtual void update(GameObject& obj);
};
