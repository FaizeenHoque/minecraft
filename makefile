CXX = g++
CXXFLAGS = -Ilibraries/glad/include
LDFLAGS = -lglfw -lGL

SRC = $(wildcard src/*.cpp) libraries/glad/src/glad.c

build/app: $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $@

run: build/app
	./build/app

clean:
	rm -f build/app