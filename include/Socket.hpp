#pragma once
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>

#include "Address.hpp"

class Socket {
	public:
	Socket(){

	}

	~Socket(){
		close();
	}

	bool open(unsigned short port){
		handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

		if(handle <= 0){
			std::cerr<<"Failed to initialize socket.\n";
			close();
			return false;
		}

		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons((unsigned short)port);

		if(bind(handle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0){
			std::cerr<<"Error binding the socket.\n";
			close();
			return false;
		}

		int nonBlocking = 1;
		if(fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1){
			std::cerr<<"Failed to set nonblocking!\n";
			close();
			return false;
		}

		return true;
	}

	void close(){
		::close(handle);
	}

	bool isOpen() const{
		return handle;
	}

	bool send(const Address& destination, const void* data, int size){
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr = htonl(destination.address);
		addr.sin_port = htons((unsigned short)destination.port);

		int sentBytes = sendto(handle, (const char*)data, size, 0, (sockaddr*)&addr, sizeof(sockaddr_in));

		if(sentBytes != size){
			std::cerr<<"Couldn't send package.\n";
			return false;
		}

		// std::cout<<"Packet Sent!\n";
		return true;
	}

	int receive(Address& sender, void* data, int size){
		sockaddr_in addr;
		socklen_t senderLength = sizeof(addr);

		int receivedBytes = recvfrom(handle, (char*)data, size, 0, (sockaddr*)&addr, &senderLength);

		if(receivedBytes >= 0)
			sender = Address(ntohl(addr.sin_addr.s_addr), ntohs(addr.sin_port));

		return receivedBytes;
	}

	private:
	int handle;
};
