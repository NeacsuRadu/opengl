INCLUDE_DIR = -I. -I../include
LIBS 		= -ldl -lGL -l:libglfw.so.3

all: rotating_cube lighting

main: main.cpp ShaderProgram.o Callbacks.o glad.c stb_image.o
	g++ -std=c++14 -o $(INCLUDE_DIR) $(LIBS)  main.cpp stb_image.cpp glad.c ../output/main

rotating_cube: rotating_cube.cpp ShaderProgram.cpp glad.c stb_image.cpp
	g++ -std=c++14 rotating_cube.cpp stb_image.cpp ShaderProgram.cpp glad.c $(INCLUDE_DIR) $(LIBS) -o ../output/rotating_cube

lighting: lighting.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp
	g++ -std=c++14 lighting.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp $(INCLUDE_DIR) $(LIBS) -o ../output/lighting

test: tests.cpp
	g++ -std=c++14 tests.cpp -c

ShaderProgram.o: ShaderProgram.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) ShaderProgram.cpp 

Callbacks.o: Callbacks.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) Callbacks.cpp

Camera.o: Camera.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) Camera.cpp

stb_image.o: stb_image.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) stb_image.cpp

clean:
	rm -f ./output/ *.o