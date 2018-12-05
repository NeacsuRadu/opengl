INCLUDE_DIR = -I. -I../include -I../include/src
LIBS 		= -ldl -lGL -l:libglfw.so.3 -L../lib -lreactphysics3d

all: rotating_cube lighting

main: main.cpp ShaderProgram.o Callbacks.o glad.c stb_image.o
	g++ -std=c++14 -o $(INCLUDE_DIR) $(LIBS)  main.cpp stb_image.cpp glad.c ../output/main

rotating_cube: rotating_cube.cpp ShaderProgram.cpp glad.c stb_image.cpp
	g++ -std=c++14 rotating_cube.cpp stb_image.cpp ShaderProgram.cpp glad.c $(INCLUDE_DIR) $(LIBS) -o ../output/rotating_cube

lighting: lighting.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp
	g++ -std=c++14 lighting.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp $(INCLUDE_DIR) $(LIBS) -o ../output/lighting

physics: physics.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Object.o Helpers.cpp
	g++ -std=c++14 physics.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Object.o Helpers.cpp $(INCLUDE_DIR) $(LIBS) -o ../output/physics

shadow: shadow.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp vertex_buffer.o index_buffer.o vertex_array.o
	g++ -std=c++14 shadow.cpp ShaderProgram.o glad.c stb_image.o Callbacks.o Camera.o Helpers.cpp vertex_buffer.o index_buffer.o vertex_array.o $(INCLUDE_DIR) $(LIBS) -o ../output/shadow

test: tests.cpp
	g++ -std=c++14  $(INCLUDE_DIR) tests.cpp  -o ../output/test

react_tests: react_tests.cpp
	g++ $(INCLUDE_DIR) react_tests.cpp $(LIBS) -o ../output/reactt

ShaderProgram.o: ShaderProgram.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) ShaderProgram.cpp 

Callbacks.o: Callbacks.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) Callbacks.cpp

Camera.o: Camera.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) Camera.cpp

stb_image.o: stb_image.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) stb_image.cpp

Object.o: Object.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) Object.cpp

vertex_buffer.o: vertex_buffer.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) vertex_buffer.cpp
	
index_buffer.o: index_buffer.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) index_buffer.cpp

vertex_buffer_layout.o: vertex_buffer_layout.h
	g++ -std=c++14 -c $(INCLUDE_DIR) vertex_buffer_layout.h

vertex_array.o: vertex_array.cpp
	g++ -std=c++14 -c $(INCLUDE_DIR) vertex_array.cpp

clean:
	rm -f ./output/ *.o