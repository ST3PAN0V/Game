MyGame: main.o game.o
	g++ main.o game.o -lglut -lGL -o MyGame
	rm *.o
main.o: main.cpp
	g++ -c main.cpp -o main.o
game.o: game.cpp
	g++ -c game.cpp -o game.o

