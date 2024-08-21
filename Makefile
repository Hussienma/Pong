all:
	g++ -I./include ./source/*.cpp -o Pong -lSDL2main -lSDL2_ttf -lSDL2
	./Pong
debug:
	g++ -I./include ./source/*.cpp -g -o debug -lSDL2main -lSDL2_ttf -lSDL2
	./debug
debugleaks:
	g++ -I./include ./source/*.cpp -fsanitize=address -g -o Pong -lSDL2main -lSDL2_ttf -lSDL2
	./Pong
gameserver:
	g++ -std=c++11 -I./include ./server/server.cpp -o pong_server
	./pong_server 4040
debugserver:
	gcc -dD -E -DLINUX -I./include ./server/server.cpp
