CXX          ?= clang++
INCLUDE_PATH := -I/usr/include/SDL -Isrc/game/ -Isrc/main_sdl -Isrc/
CXX_FLAGS    += -O2 -march=native -Wno-c++11-compat-deprecated-writable-strings
LIBS         := -lGL -lSDL -lSDL_mixer -lSDL_image

all:
	$(CXX) $(INCLUDE_PATH) $(CXX_FLAGS) src/game/*.cpp src/main_sdl/*.cpp $(LIBS)
