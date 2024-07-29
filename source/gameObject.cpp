#include "Game.hpp"
#include "GameObject.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Constants.h"
#include <cstdlib>
#include <iostream>
#include <map>

void GameObject::update(){
	if(input)
		input->update(*this);

	position.x += velocityX * Game::deltaTime;
	position.y += velocityY * Game::deltaTime;

	if(graphics)
		graphics->update(*this);
}

void Ball::update(){
	// Timer for the ball to respawn after scoring
	if(timer != 0.0f){
		if((timer+=Game::deltaTime*1000) < 3000)
			return;
		else
			timer = 0.0f;
	}

	if(abs(dy) > maxSpeed)
		dy = abs(maxSpeed*dy)/dy;

	position.x += dx;
	position.y += dy;

	if(position.x + position.w > WINDOW_WIDTH){
		Game::players["player1"].score += 1;
		reset();
		return;
	}
	else if(position.x < 0){
		Game::players["player2"].score += 1;
		reset();
		return;
	}

	if(position.y + position.h > WINDOW_HEIGHT || position.y < 0)
		dy = -dy;

	std::map<std::string, Player>::iterator itr;
	for(itr = Game::players.begin(); itr != Game::players.end(); ++itr){
		SDL_Rect obj = itr->second.position;

		if(checkCollision(obj.x, obj.y, obj.w, obj.h)){
			if(itr->second.velocityY > 0)
				dy -= 2;
			else if(itr->second.velocityY < 0)
				dy += 2;
			dx = -dx;
		}
	}

	graphics->update(*this);
}

void Ball::reset(){
	timer += Game::deltaTime;
	Game::scoreText.updateText(Game::getScoreString());
	dx = -dx;
	position.x = CENTER_HOR;
	position.y = CENTER_VER;
}
