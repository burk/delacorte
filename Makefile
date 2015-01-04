all:
	g++ -O3 -std=c++0x -o delacorte delacorte.cpp
debug:
	g++ -std=c++0x -g -o delacorte delacorte.cpp
