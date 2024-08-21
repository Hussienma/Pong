#pragma once

#include <SDL2/SDL_surface.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>

class RenderWindow {
	public:
	SDL_Window* window;
	SDL_Renderer* renderer;
	RenderWindow(int width, int height){
		window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
		if(!window)
			std::cerr<<"Faild to initialize window: "<<SDL_GetError()<<std::endl;
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if(!renderer)
			std::cerr<<"Faild to initialize renderer: "<<SDL_GetError()<<std::endl;
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}
	~RenderWindow(){
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
	}

	SDL_Texture* createTextureFromSurface(SDL_Surface* surface){
		return SDL_CreateTextureFromSurface(renderer, surface);
	}

	int getWindowRefreshrate(){
		int displayIndex = SDL_GetWindowDisplayIndex(window);

		SDL_DisplayMode mode;

		SDL_GetDisplayMode(displayIndex, 0, &mode);

		return mode.refresh_rate;
	}

	void clear(){
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
	}

	void render(SDL_Texture* texture, SDL_Rect position){
		SDL_RenderCopy(renderer, texture, NULL, &position);
	}

	void render(SDL_Rect rect){
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(renderer, &rect);
	}

	void display(){
		SDL_RenderPresent(renderer);
	}

	void destroyTexture(SDL_Texture* texture){
		SDL_DestroyTexture(texture);
	}

	void destroySurface(SDL_Surface* surface){
		SDL_FreeSurface(surface);
	}
};
