#include "Game.hpp"
#include "GameObject.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Constants.h"
#include <cstdlib>

void GameObject::update(){
	if(input)
		input->update(*this);

	position.x += velocityX * Game::deltaTime;
	position.y += velocityY * Game::deltaTime;

	if(position.y < 0)
		position.y = 0;
	else if(position.y + position.h > WINDOW_HEIGHT)
		position.y = WINDOW_HEIGHT-position.h;

	if(graphics)
		graphics->update(*this);
}

void OnlinePlayer::update(){
	if(input)
		input->update(*this);

	position.y += velocityY * Game::deltaTime;

	if(graphics)
		graphics->update(*this);
}

void Ball::update(){
	if(abs(velocityY) > maxSpeed)
		velocityY = abs(maxSpeed*velocityY)/velocityY;

	position.x += velocityX * Game::deltaTime;
	position.y += velocityY * Game::deltaTime;

	if(position.y + position.h > WINDOW_HEIGHT || position.y < 0)
		velocityY = -velocityY;

	for(GameObject* player: Game::players){
		SDL_Rect playerPos = player->position;

		if(checkCollision(playerPos.x, playerPos.y, playerPos.w, playerPos.h)){
			if(player->velocityY > 0)
				velocityY -= 2;
			else if(player->velocityY < 0)
				velocityY += 2;
			velocityX = -velocityX;
		}
	}

	graphics->update(*this);
}

void Ball::reset(){
	velocityX = -velocityX;
	if(velocityY != 0)
		velocityY = 2;

	position.x = CENTER_HOR;
	position.y = CENTER_VER;
}
