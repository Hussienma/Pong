#pragma once
#include <string>

class Address {
	private:
	public:
	uint32_t address;
	unsigned short a, b, c, d;
	unsigned short port;
	Address(){}
	Address(unsigned int a, unsigned int b, unsigned int c, unsigned int d, unsigned short port)
	: a(a), b(b), c(c), d(d), port(port){
		address = (a << 24) | (b << 16) | (c << 8) | d;
	}

	Address(uint32_t address, unsigned short port): address(address), port(port){
	}

	bool operator==(const Address& other){
		return (this->address == other.address && this->port == other.port);
	}

	bool operator!=(const Address& other){
		return !(*this==other);
	}

	std::string to_string(){
		uint8_t octet1 = (address >> 24) & 0xFF;
		uint8_t octet2 = (address >> 16) & 0xFF;
		uint8_t octet3 = (address >> 8) & 0xFF;
		uint8_t octet4 = address & 0xFF;

		return std::to_string(static_cast<int>(octet1)) + "." +std::to_string(static_cast<int>(octet2)) + "." + std::to_string(static_cast<int>(octet3)) + "." + std::to_string(static_cast<int>(octet4)) + ":" + std::to_string(port);
	}
};
