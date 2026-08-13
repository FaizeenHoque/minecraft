dev:
	g++ main.cpp glad/src/glad.c -Iglad/include -lglfw -lGL -o build/app
run:
	g++ main.cpp glad/src/glad.c -Iglad/include -lglfw -lGL -o build/app
	./build/app