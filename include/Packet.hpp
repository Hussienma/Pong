#pragma once
#include <cassert>
#include <cstdint>
#include <iostream>
#include <queue>
#include <string>

#include "Address.hpp"
#include "Index.hpp"

class Packet {
	public:
	PacketType type;
	Address* sender;
	std::string protocolId;
	uint32_t sequenceNumber=0;
	uint32_t timestamp;
	std::string content;
	bool ack[32] = { false };

	Packet(){}
	Packet(Address* sender, std::string protocolId, uint32_t sequenceNumber, uint32_t timestamp, std::string content): sender(sender), protocolId(protocolId), sequenceNumber(sequenceNumber), timestamp(timestamp), content(content){
		parseContent(content);
	}

	virtual void parseContent(std::string content){
		if(content.length() <= 32)
			return;

		std::string ackStr = content.substr(0, 32);
		for(int i=0; i<32; ++i)
			ack[i] = ackStr[i] == '0' ? false : true;
	}

	virtual std::string to_string(){
		std::string result;
		result += protocolId;
		
		for(int i=3; i>=0; --i){
			result += static_cast<unsigned char>(sequenceNumber>>(8*i));
		}

		for(int i=0; i<32; ++i)
			result += ack[i] ? '1' : '0';

		result += content;

		return result;
	}
};

class ClientPacket: public Packet {
	public:
	std::queue<ControllerKey> input;
	ClientPacket(){}
	ClientPacket(Packet* packet): Packet(*packet){
		 parseContent(packet->content);
	}
	ClientPacket(Address* sender, std::string protocolId, uint32_t sequenceNumber, uint32_t timestamp, std::string content): Packet(sender, protocolId, sequenceNumber, timestamp, content){
		parseContent(content);
	}

	private:
	void parseContent(std::string content) override {
		if(content.length() <= 32)
			return;

		std::string packetInput = content.substr(32);
		int start = packetInput.find_first_not_of('0', 0);

		for(unsigned long i=start; i<packetInput.size(); ++i){
			switch(packetInput[i]){
				case '1': input.push(UP_KEY); break;
				case '2': input.push(DOWN_KEY); break;
				default: input.push(KEY_UP); break;
			}
		}

		// std::cout<<"Input in package is: "<<packetInput<<std::endl;
		
	}
};

class ServerPacket: public Packet {
	public:
	vector2 ballPosition, ballSpeed;
	int positions[2];
	int score[2];
	ServerPacket(){}

	ServerPacket(Packet* packet): Packet(*packet){
		parseContent(packet->content);
	}

	ServerPacket(Address* sender, std::string protocolId, uint32_t sequenceNumber, uint32_t timestamp, std::string content): Packet(sender, protocolId, sequenceNumber, timestamp, content){
		parseContent(content);
	}

	private:
	void parseContent(std::string content) override {
		if(content.length() < 60)
			return;

		ballPosition.x = atoi(content.substr(32, 4).c_str());
		ballPosition.y = atoi(content.substr(36, 4).c_str());
		
		ballSpeed.x = atoi(content.substr(40, 4).c_str());
		ballSpeed.y = atoi(content.substr(44, 4).c_str());

		std::cout<<"("<<ballSpeed.x<<", "<<ballSpeed.y<<")\n";

		positions[0] = atoi(content.substr(48, 4).c_str());
		positions[1] = atoi(content.substr(52, 4).c_str());

		score[0] = atoi(content.substr(56, 2).c_str());
		score[1] = atoi(content.substr(58, 2).c_str());
	}
};
