dev:
	g++ src/main.cpp libraries/glad/src/glad.c -Ilibraries/glad/include -lglfw -lGL -o build/app
run:
	g++ src/main.cpp libraries/glad/src/glad.c -Ilibraries/glad/include -lglfw -lGL -o build/app
	./build/app