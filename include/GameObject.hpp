#pragma once

#include "Index.hpp"
#include "NetworkHost.hpp"
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
	virtual void update() override {
		GameObject::update();
	}
};

class OfflinePlayer: public Player {
	public:
	OfflinePlayer(){}
	OfflinePlayer(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): Player(position, graphics, input){}
};

class OnlinePlayer: public Player{
	public:
	OnlinePlayer(){}
	OnlinePlayer(SDL_Rect position, GraphicsComponent* graphics, InputComponent* input): Player(position, graphics, input){}
	virtual void update() override;
};

class Ball: public GameObject{
	public:
	int maxSpeed = 500;
	Ball(){}
	Ball(SDL_Rect position, vector2 velocity, GraphicsComponent* graphics): GameObject(position, graphics){
		velocityX = velocity.x;
		velocityY = velocity.y;
	}
	void update() override;
	void reset();
};
