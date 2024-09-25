#pragma once

#include "Address.hpp"
#include "Packet.hpp"
#include "Index.hpp"
#include "Utils.h"
#include "Constants.h"

#include <string>
#include <queue>

class ServerGame {
	public:
	const double fixedDeltaTime = SERVER_UPDATE_RATE_SECONDS;
	std::string clients[2];
	vector2 playersPosition[2];
	int playersSpeed[2] = {0};
	std::queue<ControllerKey> inputBuffers[2];
	vector2 playerScale;
	int score[2];
	vector2 ballPosition, ballSpeed, ballScale;
	int maxBallSpeed = 500;
	ServerGame(){
		initializeGame();
	}

	ServerGame(Address* client1, Address* client2){
		clients[0] = client1->to_string();
		clients[1] = client2->to_string();
	}

	void initializeGame(){
		playersPosition[0] = vector2(0, 200);
		playersPosition[1] = vector2(620, 200);
		ballPosition = vector2(40, 225);
		ballScale = vector2(15, 15);
		playerScale = vector2(20, 80);
		score[0] = 10;
		score[1] = 4;
		ballSpeed = vector2(150, 100);
	}

	void updateState(ClientPacket* packet){
		if(packet){
			for(; !packet->input.empty(); packet->input.pop())
				inputBuffers[0].push(packet->input.front());
		}
	}

	void update(){
		std::cout<<"Delta time: "<<fixedDeltaTime<<std::endl;
		playersSpeed[0] = 0;
		if(!inputBuffers[0].empty()){
			switch(inputBuffers[0].front()){
				case UP_KEY:
					playersSpeed[0] = -500;
					break;
				case DOWN_KEY:
					playersSpeed[0] = 500;
					break;
				default:
					break;
			}

			inputBuffers[0].pop();
		}

		playersPosition[0].y += playersSpeed[0] * fixedDeltaTime;

		if(abs(ballSpeed.y) > maxBallSpeed)
			ballSpeed.y = abs(maxBallSpeed*ballSpeed.y)/ballSpeed.y;

		ballPosition.x += ballSpeed.x * fixedDeltaTime;
		ballPosition.y += ballSpeed.y * fixedDeltaTime;

		if(ballPosition.y + ballScale.y > 480 || ballPosition.y < 0)
			ballSpeed.y = -ballSpeed.y;

		if(ballPosition.x + ballScale.x > 640 || ballPosition.x < 0)
			ballSpeed.x = -ballSpeed.x;

		// for(auto player: playersPosition){
		// 	if(checkCollision(player.x, player.y, playerScale.x, playerScale.y)){
		// 		// if(player.velocityY > 0)
		// 		// 	ballSpeed.y -= 2;
		// 		// else if(player.velocityY < 0)
		// 		// 	ballSpeed.y += 2;
		// 		ballSpeed.x = -ballSpeed.x;
		// 	}
		// }
	}

	bool checkCollision(int x, int y, int w, int h){
		return (ballPosition.x + ballScale.x > x && ballPosition.x < x + w && ballPosition.y + ballScale.y > y && ballPosition.y < y + h);
	}

	std::string gameState(){
		std::string state;
		state+= utils::toFixedChar(ballPosition.x, 4);
		state+= utils::toFixedChar(ballPosition.y, 4);
		state+= utils::toFixedChar(ballSpeed.x, 4);
		state+= utils::toFixedChar(ballSpeed.y, 4);
		state+= utils::toFixedChar(playersPosition[0].y, 4);
		state+= utils::toFixedChar(playersPosition[1].y, 4);
		state+= utils::toFixedChar(score[0]);
		state+= utils::toFixedChar(score[1]);

		return state;
	}
};
