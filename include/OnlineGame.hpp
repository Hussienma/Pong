#pragma once
#include "Game.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Constants.h"
#include "NetworkHost.hpp"

class OnlineGame: public Game {
	public:
	std::map<int, ServerPacket*> packets;
	Text pingText;
	Address serverAddress;
	Client* client;
	const static float fixedDeltaTime;
	OnlineGame();
	OnlineGame(RenderWindow& window);
	~OnlineGame();
	void initializeGame() override;
	void sendInput();
	void listen();
	void applyPacket();
	virtual void update() override;
};
