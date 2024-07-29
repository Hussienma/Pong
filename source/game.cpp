#include "Game.hpp"
#include "Constants.h"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include <SDL2/SDL_surface.h>
#include <string>

GameState Game::state = STARTED;
RenderWindow* Game::window;
float Game::deltaTime;
std::map<std::string, Player> Game::players;
Text Game::scoreText;

Game::Game(RenderWindow& w){
	window = &w;

	players["player1"] = Player({0, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new PlayerInputComponent());
	players["player2"] = Player({WINDOW_WIDTH-20, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new InputComponent());
	ball = Ball({40, CENTER_VER-15, 15, 15}, new GraphicsComponent());
	scoreText = Text({CENTER_HOR, 30, 10, 10}, new GraphicsComponent());
	scoreText.updateText(getScoreString());
}

void Game::update(){
	players["player1"].update();
	players["player2"].update();
	ball.update();
	scoreText.update();
}

std::string Game::getScoreString(){
	return (std::to_string(players["player1"].score) + " - " + std::to_string(players["player2"].score));
}
