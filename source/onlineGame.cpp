#include "OnlineGame.hpp"
#include "Controller.hpp"
#include "Index.hpp"
#include "Packet.hpp"
#include "RenderWindow.hpp"
#include <chrono>
#include <stack>
#include <string>
#include <thread>

std::stack<ServerPacket*> OnlineGame::packets;

OnlineGame::OnlineGame(){
	initializeGame();
}

OnlineGame::OnlineGame(RenderWindow& w): serverAddress(127, 0, 0, 1, 4040){
	window = &w;

	initializeGame();
}

void OnlineGame::initializeGame(){
	client = new Client(serverAddress, 4041);

	std::cout<<"Here\n";
	ServerPacket* packet;
	while((packet = client->receive()) == nullptr || packet->content.substr(32) != "GameInit"){
		client->send("HELLO");
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}

	std::cout<<packet->content<<std::endl;

	players[0] = new OnlinePlayer({0, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new PlayerInputComponent());
	players[1] = new OnlinePlayer({WINDOW_WIDTH-20, CENTER_VER-40, 20, 80}, new GraphicsComponent(), new OnlineInputComponent());
	ball = Ball({40, CENTER_VER-15, 15, 15}, {200, 100}, new GraphicsComponent());
	scoreText = Text({CENTER_HOR, 30, 10, 10}, new GraphicsComponent());
	scoreText.updateText(getScoreString());

	std::thread listeningThread(&OnlineGame::listen, this);
	listeningThread.detach();
	std::thread inputThread(&OnlineGame::sendInput, this);
	inputThread.detach();
}

void OnlineGame::sendInput(){
	while(true){
		std::string input = Controller::getInputServer();
		client->send(input);
		Controller::confirmInput(input.size());

		int waitForMilliseconds = CLIENT_TIMESTEP_SECONDS * 1000;
		std::this_thread::sleep_for(std::chrono::milliseconds(waitForMilliseconds));
	}
}

void OnlineGame::listen(){
	ServerPacket* packet = client->receive();

	while(true){
		if(packet != nullptr){
			std::cout<<"Recieved packet: "<<packet->content.substr(32)<<std::endl;

			packets.push(packet);

			std::cout<<"Ping: "<<client->getPing()<<"ms\n";
		}

		packet = client->receive();
		std::this_thread::sleep_for(std::chrono::milliseconds(20));
	}
}

void OnlineGame::applyPacket(){
	ServerPacket packet = packets.top();
	ball.position.x = packet.ballPosition.x;
	ball.position.y = packet.ballPosition.y;

        players[0]->position.y = packet.positions[0];
        players[1]->position.y = packet.positions[1];

        ball.velocityX = packet.ballSpeed.x;
        ball.velocityY = packet.ballSpeed.y;

        players[0]->score = packet.score[0];
        players[1]->score = packet.score[1];
}

void OnlineGame::update(){
	applyPacket();
	players[0]->update();
	players[1]->update();
	ball.update();
	checkForGoals();
	scoreText.updateText(getScoreString());
	scoreText.update();
}
