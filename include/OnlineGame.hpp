#pragma once
#include "Game.hpp"
#include "GraphicsComponent.hpp"
#include "InputComponent.hpp"
#include "Constants.h"
#include "NetworkHost.hpp"
#include <stack>

class OnlineGame: public Game {
	public:
	static std::stack<ServerPacket*> packets;
	Text pingText;
	Address serverAddress;
	Client* client;
	OnlineGame();
	OnlineGame(RenderWindow& window);
	void initializeGame() override;
	void sendInput();
	void listen();
	static void applyPacket();
	virtual void update() override;
};
