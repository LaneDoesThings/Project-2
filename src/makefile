all: project

project: Project2.o

%.o: %.cpp
	g++ $< -o ../bin/project -Wall -lGL -lGLU -lglut -lGLEW

start: project
	../bin/project