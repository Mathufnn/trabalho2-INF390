main.out: trabalho2.o
	g++ trabalho2.o -lglut -lGLU -lGL -o main.out

trabalho2.o:
	g++ -c trabalho2.cpp
