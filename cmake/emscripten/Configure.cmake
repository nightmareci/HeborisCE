set(FRAMEWORK_VER "WEB-SDL2")

set(ENABLE_JOYSTICK FALSE CACHE BOOL "" FORCE)
set(ENABLE_GAME_CONTROLLER FALSE CACHE BOOL "" FORCE)
set(ALL_VIDEO_SETTINGS FALSE CACHE BOOL "" FORCE)
set(ENABLE_GAME_QUIT FALSE CACHE BOOL "" FORCE)

configure_file("${SRC}/src/main_sdl/defs.h.in" "${BIN}/src/main_sdl/defs.h" @ONLY)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS=[\"png\"]")
if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file '${SRC}/cmake/emscripten/index.html'")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s EXPORTED_RUNTIME_METHODS=ccall -s EXPORTED_FUNCTIONS=_main,_startup -s ALLOW_MEMORY_GROWTH -lidbfs.js -s FORCE_FILESYSTEM=1 -s ASYNCIFY \
--preload-file '${SRC}/res/gamecontrollerdb.txt@basepath/res/gamecontrollerdb.txt' \
--preload-file '${SRC}/res/bg/highDetail@basepath/res/bg/highDetail' \
--preload-file '${SRC}/res/bg/lowDetail@basepath/res/bg/lowDetail' \
--preload-file '${SRC}/res/bgm@basepath/res/bgm' \
--preload-file '${SRC}/res/font@basepath/res/font' \
--preload-file '${SRC}/res/graphics/highDetail@basepath/res/graphics/highDetail' \
--preload-file '${SRC}/res/graphics/lowDetail@basepath/res/graphics/lowDetail' \
--preload-file '${SRC}/res/graphics/title@basepath/res/graphics/title' \
--preload-file '${SRC}/res/se@basepath/res/se' \
--preload-file '${SRC}/config/mission@basepath/config/mission' \
--preload-file '${SRC}/config/stage@basepath/config/stage' \
")

message(STATUS "${CMAKE_EXE_LINKER_FLAGS}")

add_executable(${EXE} ${EXE_SOURCES} ${EXE_HEADERS})
set(CMAKE_EXECUTABLE_SUFFIX .html)
