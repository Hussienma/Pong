#pragma once

#include <SDL2/SDL.h>

class GraphicsComponent;
class InputComponent;

class GameObject {
	public:
	int velocityX = 0, velocityY = 0;
	GraphicsComponent* graphics = nullptr;
	InputComponent* input = nullptr;
	SDL_Rect position;
	GameObject(){}
	GameObject(SDL_Rect position, GraphicsComponent* graphics): position(position), graphics(graphics){}
	GameObject(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): position(position), graphics(graphics), input(input){}
	virtual void update();
	bool checkCollision(int x, int y, int w, int h){
		return (position.x + position.w > x && position.x < x + w && position.y + position.h > y && position.y < y + h);
	}
	// virtual void update(float deltaTime);
};

class Player: public GameObject{
	public:
	int score = 0;
	Player(){}
	Player(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): GameObject(position, graphics, input){}
	// void update(float deltaTime) override;
};

class Ball: public GameObject{
	int dx = 5, dy= 0;
	int maxSpeed = 5;
	float timer = 0.0f;
	public:
	Ball(){}
	Ball(SDL_Rect position, GraphicsComponent* graphics): GameObject(position, graphics){}
	void update() override;
	void reset();
};
