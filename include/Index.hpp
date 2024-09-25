#pragma once

#include <SDL2/SDL_rect.h>
#include <cstdint>
#include <string>
enum ControllerKey { KEY_UP = 0, UP_KEY=1, DOWN_KEY=2, PAUSE_KEY=3, QUIT_KEY=4 };

enum PacketType { HANDSHAKE, UPDATE };

struct vector2 {
	int x = 0;
	int y = 0;
	vector2(): x(0), y(0){}
	vector2(int x, int y): x(x), y(y){}
	inline bool operator==(vector2 other){
		return (x==other.x && y==other.y);
	}
	inline bool operator==(SDL_Rect other){
		return (x==other.x && y==other.y);
	}
	inline bool operator!=(vector2 other){
		return !(*this==other);
	}
	inline bool operator!=(SDL_Rect other){
		return (*this==other);
	}

	std::string to_string(){
		return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
	}
};

struct bufferedInput {
	ControllerKey key = KEY_UP; 
	uint32_t timestamp = 0;
};
