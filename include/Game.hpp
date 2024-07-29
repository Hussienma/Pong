#pragma once

#include "GameObject.hpp"
#include "RenderWindow.hpp"
#include "UI.hpp"
#include <map>

enum GameState { STARTED, IN_PROGRESS, PAUSED, FINISHED, QUIT };

class Game {
	private:
	Ball ball;
	public:
	static std::map<std::string, Player> players;
	static Text scoreText;
	static float deltaTime;
	static GameState state;
	static RenderWindow* window;
	Game(RenderWindow& window);
	void update();
	static std::string getScoreString();
};
