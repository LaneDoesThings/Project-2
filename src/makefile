all: project.bin

project.bin: Project2.o

%.o: %.cpp
	g++ $< -o ../bin/project.bin -Wall -lGL -lGLU -lglut -lGLEW

start: project.bin
	../bin/project.bin