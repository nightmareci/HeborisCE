INCLUDE_PATH := -Isrc/game/ -Isrc/main_sdl -Isrc/
CXXFLAGS     += -O2 $(shell sdl2-config --cflags)
CXXWARNINGS  := -Wno-c++11-compat-deprecated-writable-strings -Wno-format-extra-args \
				-Wno-invalid-source-encoding -Wno-logical-op-parentheses \
				-Wno-write-strings
LDLIBS       := $(shell sdl2-config --libs) -lSDL2_mixer -lSDL2_image
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
