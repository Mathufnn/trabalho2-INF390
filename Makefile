main.out: trabalho3.o
	g++ trabalho3.o -lglut -lGLU -lGL -o main.out

trabalho2.o:
	g++ -c trabalho3.cpp
