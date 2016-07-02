all: *.cpp
	g++ -std=c++11 *.cpp -lGL -lglfw -lGraphicsMagick++ -I/usr/include/GraphicsMagick

run: all
	./a.out 1

