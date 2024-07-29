all:
	g++ -I./include ./source/*.cpp -o Pong -lSDL2main -lSDL2_ttf -lSDL2
	./Pong
