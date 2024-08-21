#include "OnlineGame.hpp"
#include "Controller.hpp"
#include "Index.hpp"
#include "RenderWindow.hpp"
#include <thread>

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
}

// TODO: Send all inputs in the buffer and confirm ack from server
void OnlineGame::listen(){
	ServerPacket* packet = client->receive();

	while(true){
		// Controller::handleInput();
		client->send(std::to_string(Controller::getInputFromBuffer()));

		if(packet != nullptr){
			std::cout<<"Recieved packet: "<<packet->content.substr(32)<<std::endl;

			ball.position.x = packet->ballPosition.x;
			ball.position.y = packet->ballPosition.y;

			players[0]->position.y = packet->positions[0];
			players[1]->position.y = packet->positions[1];

			ball.velocityX = packet->ballSpeed.x;
			ball.velocityY = packet->ballSpeed.y;
			
			players[0]->score = packet->score[0];
			players[1]->score = packet->score[1];

			std::cout<<"Ping: "<<client->getPing()<<"ms\n";
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		packet = client->receive();
	}
}

void OnlineGame::update(){
	players[0]->update();
	players[1]->update();
	ball.update();
	checkForGoals();
	scoreText.updateText(getScoreString());
	scoreText.update();
}
