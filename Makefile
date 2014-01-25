

main: main.cpp initShaders.h
	g++ main.cpp -o main -lGL -lGLEW -lglut -lGLU 
