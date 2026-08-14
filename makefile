CXX = g++
CXXFLAGS = -O2 -Ilibraries/glad/include -Ilibraries/stb
LDFLAGS = -lglfw -lGL

SRC = $(wildcard src/*.cpp) libraries/glad/src/glad.c
RELEASE = Minecrap

.PHONY: run release clean

build/app: $(SRC)
	mkdir -p build
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $@

run: clean
	$(MAKE) build/app
	./build/app

release: $(RELEASE)

$(RELEASE): $(SRC)
	$(CXX) $(SRC) $(CXXFLAGS) $(LDFLAGS) -o $@

clean:
	rm -f build/app $(RELEASE)
