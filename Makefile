CXX          ?= clang++
INCLUDE_PATH := -I/usr/include/SDL -Isrc/game/ -Isrc/main_sdl -Isrc/
CXX_FLAGS    += -DLINUX -DSDL_USE_OPENGL $(shell sdl-config --cflags)
CXX_WARNINGS := -Wno-c++11-compat-deprecated-writable-strings -Wno-format-extra-args \
				-Wno-invalid-source-encoding -Wno-logical-op-parentheses
LIBS         := -lGL -lSDL -lSDL_mixer -lSDL_image
PROGRAM_NAME := heboris

all: $(PROGRAM_NAME)
	$(CXX) $(INCLUDE_PATH) $(CXX_FLAGS) $(CXX_WARNINGS) \
	src/game/*.cpp src/main_sdl/*.cpp $(LIBS) -o $(PROGRAM_NAME)
	@mkdir -p REPLAY

clean:
	rm -f $(PROGRAM_NAME)

.PHONY: clean
