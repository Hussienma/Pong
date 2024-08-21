#pragma once

#include <cstdint>
enum ControllerKey { KEY_UP = 0, UP_KEY=1, DOWN_KEY=2, PAUSE_KEY=3, QUIT_KEY=4 };

enum PacketType { HANDSHAKE, UPDATE };

struct vector2 {
	int x = 0;
	int y = 0;
	vector2(): x(0), y(0){}
	vector2(int x, int y): x(x), y(y){}
};

struct bufferedInput {
	ControllerKey key = KEY_UP; 
	uint32_t timestamp = 0;
};
