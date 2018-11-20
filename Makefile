INCLUDE_DIR = -I. -Iinclude
LIBS 		= -ldl -lGL -l:libglfw.so.3

all: main rotating_cube lighting

main: main.cpp ShaderProgram.cpp glad.c stb_image.cpp
	g++ -std=c++14 main.cpp stb_image.cpp ShaderProgram.cpp glad.c $(INCLUDE_DIR) $(LIBS) -o output/main

rotating_cube: rotating_cube.cpp ShaderProgram.cpp glad.c stb_image.cpp
	g++ -std=c++14 rotating_cube.cpp stb_image.cpp ShaderProgram.cpp glad.c $(INCLUDE_DIR) $(LIBS) -o output/rotating_cube

lighting: lighting.cpp ShaderProgram.cpp glad.c stb_image.cpp
	g++ -std=c++14 lighting.cpp stb_image.cpp ShaderProgram.cpp glad.c $(INCLUDE_DIR) $(LIBS) -o output/lighting

test: tests.cpp
	g++ -std=c++14 tests.cpp $(INCLUDE_DIR) -o output/test

clean:
	rm output/main