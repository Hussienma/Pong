#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>
#include <vector>

#include "NetworkHost.hpp"
#include "ServerGame.hpp"
#include "Constants.h"

// TODO: Create and manage rooms with multithreading (for later)

struct Room {
	Address* host;
	Address* guest;
};

void update(ServerGame* game){
	if(game){
		using namespace std::chrono;
		auto startTime = high_resolution_clock::now();
		float accumulator = 0.0f;
		int numberOfUpdates = 0;
		float timeInSeconds = 0.0f;
		while(true){
			auto currentTime = high_resolution_clock::now();
			while(accumulator >= SERVER_UPDATE_RATE_SECONDS){
				game->update();
				accumulator = 0;
				numberOfUpdates++;
			}

			duration<double> timePassed = currentTime - startTime;
			startTime = currentTime;
			accumulator += timePassed.count();
			timeInSeconds += timePassed.count();

			// std::cout<<"Accumlator: "<<accumulator<<"s.\n";
			if(accumulator < SERVER_UPDATE_RATE_SECONDS){
				// (long)((game->fixedDeltaTime-accumulator)*1000)
				duration<double> sleepFor = milliseconds((long)((SERVER_UPDATE_RATE_SECONDS-accumulator)*1000));
				std::this_thread::sleep_for(sleepFor);
				// accumulator = game->fixedDeltaTime;
			}

			std::cout<<"Update rate (per second): "<<numberOfUpdates/timeInSeconds<<std::endl;
		}
	}
}

void handleClient(ServerGame* game, ClientPacket* packet, Server* server){
	game->updateState(packet);
}

void sendState(ServerGame* game, Server* server){
	while(true){
		server->send(game->gameState());
		
		std::chrono::duration<double> sleepFor = std::chrono::milliseconds((long)(SERVER_TIMESTEP_SECONDS * 1000));
		std::this_thread::sleep_for(sleepFor);
	}
}

int main(int argc, char* argv[]){
	if(argc < 2){
		std::cerr<<"Expected port.\n";
		exit(1);
	}

	const int port = atoi(argv[1]);

	Address clientAddress = Address(127, 0, 0, 1, 4041);
	Server server(clientAddress, port);

	float accumulator = 0.0f;
	
	std::vector<Address*> connections;
	ServerGame* game;

	auto start = std::chrono::system_clock::now();

	while(true){
		auto currTime = std::chrono::system_clock::now();
		std::chrono::duration<double> timePassed = currTime - start;
		start = currTime;

		accumulator += timePassed.count() * 1000;

		while(accumulator > SERVER_TIMESTEP_SECONDS * 1000){
			ClientPacket* packet = server.receive();

			if(packet && packet->content.substr(32) == "HELLO"){
				if(connections.size() > 0 && connections.back()->address == packet->sender->address){
					server.send("GameInit");
					server.remoteSequenceNumber = 0;
				} else {
					connections.push_back(packet->sender);
					std::cout<<"Initializing game!\n";
					game = new ServerGame();
					std::thread gameThread(update, game);
					gameThread.detach();
					server.send("GameInit");
					std::thread serveThread(sendState, game, &server);
					serveThread.detach();
				}
			}
			else if(packet){
				if(connections.size() > 0 && connections.back()->address == packet->sender->address){
					std::thread networkThread(handleClient, game, packet, &server);
					networkThread.detach();
				}
			}

			accumulator -= SERVER_TIMESTEP_SECONDS * 1000;
		}

		std::chrono::duration<double> sleepFor = std::chrono::milliseconds((long)(((SERVER_TIMESTEP_SECONDS-accumulator)*1000)));
		if(accumulator < SERVER_TIMESTEP_SECONDS * 1000)
			std::this_thread::sleep_for(sleepFor);
	}
	return 0;
}
