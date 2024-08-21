#include "Constants.h"
#include "UI.hpp"
#include "Game.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"

#include <SDL2/SDL_ttf.h>
#include <iostream>

void UI::update(){
	if(input)
		input->update(*this);
	if(graphics)
		graphics->update(*this);
}

void Text::updateText(std::string text){
	SDL_Surface* surface = TTF_RenderText_Solid(font, text.data(), color);
	if(!surface){
		std::cerr<<"Failed to create text surface:"<<SDL_GetError()<<std::endl;
	}

	SDL_Texture* texture = Game::window->createTextureFromSurface(surface);
	Game::window->destroySurface(surface);
	graphics->setTexture(texture);
	TTF_SizeText(font, text.data(), &position.w, &position.h);

	position.x = CENTER_HOR - position.w/2;
	position.y = 30 - position.h/2;

	this->text = text;
}
