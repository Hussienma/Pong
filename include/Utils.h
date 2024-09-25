#pragma once

#include <SDL2/SDL.h>
#include <algorithm>
#include <cstdint>
#include <string>

namespace utils {
	inline float hireTimeInSeconds(){
		float t = SDL_GetTicks();
		t *= 1000;
		return t;
	}

	inline char* substr(char* arr, int begin, int len){
		char* res = new char[len+1];
		for(int i=0; i<len; ++i)
			*(res+i) = *(arr+begin+i);
		res[len] = '\0';
		return res;
	}

	inline unsigned char* int32ToChar(uint32_t n){
		unsigned char* result = new unsigned char[4];
		for(int i=3; i>=0; --i){
			result[i] = static_cast<unsigned char>((n>>(8*i)));
		}

		return result;
	}

	inline uint32_t charToInt32(unsigned char* c){
		uint32_t result = 0;
		for(int i=3; i>=0; --i){
			result += static_cast<int>((c[i]<<(8*i)));
		}

		return result;
	}

	inline unsigned char* floatToChar(float n){
		unsigned char* result = new unsigned char[sizeof(float)];
		memcpy(result, &n, sizeof(float));

		return result;
	}

	inline float charToFloat(unsigned char* c){
		float result;
		memcpy(&result, c, sizeof(float));

		return result;
	}

	inline std::string toFixedChar(int value, int size=2){
		std::string result;

		int n = value;
		if(value<0)
			n = -n;
		while(size-- > 0){
			result += ('0' + n%10);
			n /= 10;
		}

		std::reverse(result.begin(), result.end());

		if(value < 0)
			result[0] = '-';

		return result;
	}
}
