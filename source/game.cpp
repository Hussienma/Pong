#include "Game.hpp"
#include "Constants.h"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include <SDL2/SDL_surface.h>
#include <string>

GameState Game::state = STARTED;
RenderWindow* Game::window;
float Game::deltaTime;
Player* Game::players[2];
Ball Game::ball;
Text Game::scoreText;

OfflineGame::OfflineGame(){
	initializeGame();
}

OfflineGame::OfflineGame(RenderWindow& w){
	window = &w;

	initializeGame();
}

void OfflineGame::initializeGame(){
	std::cout<<"Offline game init\n";
	
	players[0] = new Player({0, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new PlayerInputComponent());
	players[1] = new Player({WINDOW_WIDTH-20, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new AiInputComponent());
	ball = Ball({40, CENTER_VER-15, 15, 15}, {240, 120} , new GraphicsComponent());
	scoreText = Text({CENTER_HOR, 30, 10, 10}, new GraphicsComponent());
	scoreText.updateText("0 - 0");
}

void OfflineGame::update(){
	players[0]->update();
	players[1]->update();
	ball.update();
	checkForGoals();
	scoreText.update();
}

std::string Game::getScoreString(){
	std::string score = std::to_string(players[0]->score) + " - " + std::to_string(players[1]->score);
	return score;
}

void Game::checkForGoals(){
	if(ball.position.x + ball.position.w > WINDOW_WIDTH || ball.position.x < 0)
		resetBall();
}

void Game::resetBall(){
	// Timer for the ball to respawn after scoring
	if(goalCooldown != 0.0f){
		if((goalCooldown+=deltaTime) < 3)
			return;
	}
	else {
		if(ball.position.x + ball.position.w > WINDOW_WIDTH){
			players[0]->score += 1;
		}

		else if(ball.position.x < 0){
			players[1]->score += 1;
		}

		scoreText.updateText(getScoreString());
		goalCooldown += deltaTime;
		return;
	}
	
	ball.reset();
	goalCooldown = 0.0f;
}
