CXX          ?= clang++
INCLUDE_PATH := -I/usr/include/SDL -Isrc/game/ -Isrc/main_sdl -Isrc/
CXXFLAGS    += -DLINUX -DSDL_USE_OPENGL $(shell sdl-config --cflags)
CXXWARNINGS := -Wno-c++11-compat-deprecated-writable-strings -Wno-format-extra-args \
				-Wno-invalid-source-encoding -Wno-logical-op-parentheses
LDLIBS         := -lGL -lSDL -lSDL_mixer -lSDL_image
PROGRAM_NAME := heboris

all: make_directories $(PROGRAM_NAME)

$(PROGRAM_NAME):
	$(CXX) $(INCLUDE_PATH) $(CXXFLAGS) $(CXXWARNINGS) \
	src/game/*.cpp src/main_sdl/*.cpp $(LDLIBS) -o $(PROGRAM_NAME)

make_directories:
	@mkdir -p "replay" "config/data"

clean:
	rm -f $(PROGRAM_NAME)

.PHONY: clean make_directories
