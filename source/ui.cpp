#include "Constants.h"
#include "UI.hpp"
#include "Game.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include <iostream>

void UI::update(){
	if(input)
		input->update(*this);
	if(graphics)
		graphics->update(*this);
}

void Text::updateText(std::string text){
	SDL_Color white = { 255, 255, 255 };
	surface = TTF_RenderText_Solid(font, text.c_str(), white);
	if(!surface){
		std::cerr<<"Failed to create text surface:"<<SDL_GetError()<<std::endl;
	}


	graphics->texture = Game::window->createTextureFromSurface(surface);
	TTF_SizeText(font, text.c_str(), &position.w, &position.h);

	position.x = CENTER_HOR - position.w/2;
	position.y = 30 - position.h/2;

	this->text = text;
}
