#pragma once

#include <SDL2/SDL.h>

namespace utils{
	inline float hireTimeInSeconds(){
		float t = SDL_GetTicks();
		t *= 1000;
		return t;
	}
}
