#pragma once

#include "GameObject.hpp"
#include "RenderWindow.hpp"
#include "UI.hpp"
#include <map>

enum GameState { STARTED, IN_PROGRESS, PAUSED, FINISHED, QUIT };

class Game {
	public:
	static Player* players[2];
	static Ball ball;
	static Text scoreText;
	static float deltaTime;
	static GameState state;
	static RenderWindow* window;
	Game(){}
	Game(RenderWindow& window){}
	~Game(){
		delete players[0];
		delete players[1];
	}
	virtual void initializeGame(){}
	virtual void update(){
		std::cout<<"Updating..\n";
	}
	static std::string getScoreString();

	float goalCooldown = 0.0f;
	void checkForGoals();
	void resetBall();
};

class OfflineGame: public Game {
	public:
	OfflineGame();
	OfflineGame(RenderWindow& window);
	void initializeGame() override;
	virtual void update() override;
};
