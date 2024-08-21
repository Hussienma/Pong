#pragma once
#include "Game.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Constants.h"
#include "NetworkHost.hpp"

class OnlineGame: public Game {
	public:
	Text pingText;
	Address serverAddress;
	Client* client;
	OnlineGame();
	OnlineGame(RenderWindow& window);
	void initializeGame() override;
	void listen();
	virtual void update() override;
};
