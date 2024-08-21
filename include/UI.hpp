#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

#include "GameObject.hpp"

class UI: public GameObject{
	public:
	UI(){}
	UI(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): GameObject(position, graphics, input){}
	UI(SDL_Rect position, GraphicsComponent* graphics): GameObject(position, graphics){}
	void update() override;
};

class Text: public UI {
	public:
	TTF_Font* font;
	SDL_Color color = { 255, 255, 255 };
	std::string text;
	Text(){
		loadFont();
	}
	Text(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): UI(position, graphics, input){
		loadFont();
	}
	Text(SDL_Rect position, GraphicsComponent* graphics): UI(position, graphics){
		loadFont();
	}

	void loadFont(){
		font = TTF_OpenFont("./fonts/Pixel Millennium.ttf", 84);
		if(!font){
			std::cerr<<"Error loading font "<<SDL_GetError()<<std::endl;
		}
	}

	void updateText(std::string text);
};
