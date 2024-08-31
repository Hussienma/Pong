#pragma once

#include "Socket.hpp"
#include "Packet.hpp"
#include "Utils.h"
#include "Constants.h"

#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <deque>


class NetworkHost {
	public:
	Address* address;
	Socket socket;
	std::deque<Packet*> ack;
	uint32_t localSequenceNumber = 0;
	uint32_t remoteSequenceNumber = 0;
	int RTTinMilliseconds = 0;

	NetworkHost(){
		initSocket(4040);
	}

	NetworkHost(short port){
		initSocket(port);
	}

	NetworkHost(Address& address, short port){
		initSocket(port);
		this->address = &address;
	}

	~NetworkHost(){
		delete address;
	}

	void initSocket(short port){
		socket.open(port);
		if(!socket.isOpen())
			std::cerr<<"Problem creating socket!\n";
		else
			std::cout<<"Socket bound on port: "<<port<<std::endl;
	}

	// NOTE: keep updated please
	// [protocol ID]	4 bytes
	// [sequence number]	4 bytes
	// [timestamp]		4 bytes
	// [ack bitfield]	32 bytes
	// [content]		? bytes
	virtual std::string serializePayload(std::string content){
		std::string payload;
		payload += PROTOCOLID;

		unsigned char* sequenceNumber = utils::int32ToChar(localSequenceNumber);

		using namespace std::chrono;
		uint32_t time = duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

		unsigned char* timestamp = utils::int32ToChar(time);

		for(int i=0; i<4; ++i)
			payload += sequenceNumber[i];

		for(int i=0; i<4; ++i)
			payload += timestamp[i];

		for(int i=0; i<32; ++i){
			if(i < ack.size())
				payload += ack[i]->sequenceNumber == remoteSequenceNumber-(ack.size()-1-i) ? '1' : '0';
			else
				payload += '0';
		}

		payload += content;

		delete [] sequenceNumber;
		delete [] timestamp;
		
		return payload;
	}

	virtual std::string serializePayload(Packet& packet){
		std::string payload;
		payload += packet.protocolId;

		unsigned char* sequenceNumber = utils::int32ToChar(localSequenceNumber);
		packet.sequenceNumber = localSequenceNumber;

		unsigned char* timestamp = utils::int32ToChar(packet.timestamp);

		for(int i=0; i<4; ++i)
			payload += sequenceNumber[i];

		for(int i=0; i<4; ++i)
			payload += timestamp[i];

		for(int i=0; i<32; ++i){
			if(i < ack.size()){
				bool acknowledged = ack[i]->sequenceNumber == remoteSequenceNumber-(ack.size()-1-i);
				payload += acknowledged ? '1' : '0';
				packet.ack[i] = acknowledged;
			}
			else{
				payload += '0';
				packet.ack[i] = false;
			}
		}

		payload += packet.content;

		delete [] sequenceNumber;
		delete [] timestamp;
		
		return payload;
	}

	virtual bool send(std::string content){
		std::string payload = serializePayload(content);
		localSequenceNumber++;

		std::cout<<"Packet sent: "<<content<<std::endl;

		return socket.send(*address, payload.c_str(), payload.size());
	}

	virtual bool send(Packet& packet){
		std::string payload = serializePayload(packet);
		localSequenceNumber++;

		return socket.send(*address, payload.c_str(), payload.size());
	}

	virtual Packet* parsePayload(Address* sender, char* payload, int size){
		char* protocolId = utils::substr(payload, 0, 4);
		uint32_t sequenceNumber = 0;
		uint32_t timestamp = 0;

		unsigned char sequenceNumberChar[4];
		unsigned char timestampChar[4];
		for(int i=0; i<4; ++i){
			sequenceNumberChar[i] = payload[i+4];
			timestampChar[i] = payload[i+8];
		}

		sequenceNumber = utils::charToInt32(sequenceNumberChar);
		timestamp = utils::charToInt32(timestampChar);

		char* content = utils::substr(payload, 12, size-12);

		Packet* packet = new Packet(sender, protocolId, sequenceNumber, timestamp, content);

		delete [] content;
		delete [] protocolId;

		return packet;
	}

	virtual Packet* receive(){
		int bufferSize = 256;
		char buffer[bufferSize];

		Address sender;
		int receivedBytes = socket.receive(sender, buffer, bufferSize);

		// TODO: confirm the sender's address
		if(receivedBytes <= 0)
			return nullptr; 

		Address* senderPtr = new Address(sender);

		Packet* packet = parsePayload(senderPtr, buffer, receivedBytes);

		if(remoteSequenceNumber <= packet->sequenceNumber)
			remoteSequenceNumber = packet->sequenceNumber;
		else if(packet->content.substr(32) == "HELLO"){

		}
		else
			return nullptr;

		// Handling acknowledgments
		if(ack.size() >= 32){
			delete ack.front();
			ack.pop_front();
		}

		ack.push_back(packet);

		// TODO: Fix the this (RTT should be the time between sending the packet and receiving the ack for that packet)
		using namespace std::chrono;
		uint32_t currentTime = duration_cast<milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
		// if(RTTinMilliseconds > 0)
		// 	RTTinMilliseconds = 0.1*(RTTinMilliseconds + (currentTime - packet->timestamp));
		// else
			RTTinMilliseconds = currentTime - packet->timestamp;

		std::cout<<"Recieved packet's sequence number: "<<packet->sequenceNumber<<std::endl;
		return packet;
	}

	virtual int getPing(){
		return RTTinMilliseconds;
	}
};

class Client: public NetworkHost {
	public:
	Client(){}
	Client(short port): NetworkHost(port){}
	Client(Address& serverAddress, short port): NetworkHost(serverAddress, port){}
	
	ServerPacket* receive() override {
		Packet* packet = NetworkHost::receive();
		ServerPacket* serverPacket = nullptr;
		if(packet != nullptr)
			serverPacket = new ServerPacket(packet);

		return serverPacket;
	}
};

class Server: public NetworkHost {
	public:
	Server(): NetworkHost(){}
	Server(short port): NetworkHost(port){}
	Server(Address& address, short port): NetworkHost(address, port){}

	std::string serializePayload(std::string content) override {
		std::string basicHeaders = NetworkHost::serializePayload(content);
		return basicHeaders;
	}

	// bool send(ServerPacket& packet){
	// 	return socket.send(*address, packet.to_string().c_str(), 44);
	// }

	ClientPacket* receive() override {
		Packet* packet = NetworkHost::receive();
		ClientPacket* clientPacket = nullptr;

		if(packet != nullptr){
			clientPacket = new ClientPacket(packet);
		}

		return clientPacket;
	}
};
