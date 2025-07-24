# TODO: Add option to not include --emrun in linker flags for actual releases,
# so they're suitable for hosting on a real web server.
# TODO: Add "favicon.ico".

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(BUILD_SHARED_LIBS FALSE)

# Emscripten needs vendor libraries to be handled differently than native
# builds, so special handling is done here, instead of in AddLibrariesVendor.

if(APP_VENDORED)
	set(SDL_SHARED FALSE)
	set(SDL_STATIC TRUE)
	set(SDL_TEST FALSE)
	set(SDL2_DISABLE_UNINSTALL ON)
	add_subdirectory("dep/SDL" "${APP_BIN_DIR}/dep/SDL")

	set(SDL2IMAGE_VENDORED ON)
	set(SDL2IMAGE_DEPS_SHARED OFF)
	add_subdirectory("dep/SDL_image" "${APP_BIN_DIR}/dep/SDL_image")

	set(SDL2MIXER_VENDORED ON)
	set(SDL2MIXER_DEPS_SHARED OFF)
	set(SDL2MIXER_FLAC OFF)
	set(SDL2MIXER_GME OFF)
	set(SDL2MIXER_MOD OFF)
	set(SDL2MIXER_MP3 OFF)
	set(SDL2MIXER_MIDI OFF)
	set(SDL2MIXER_OPUS OFF)
	set(SDL2MIXER_VORBIS VORBISFILE)
	set(SDL2MIXER_WAVE ON)
	set(SDL2MIXER_WAVPACK OFF)
	add_subdirectory("dep/SDL_mixer" "${APP_BIN_DIR}/dep/SDL_mixer")
else()
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s USE_SDL_MIXER=2 -s SDL2_IMAGE_FORMATS=[\"png\"]")
endif()

set(APP_FRAMEWORK_TYPE "WEB SDL2")

set(APP_ENABLE_JOYSTICK FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_GAME_CONTROLLER FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_GAME_CONTROLLER_DB FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_ALL_VIDEO_SETTINGS FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_GAME_QUIT FALSE CACHE BOOL "" FORCE)

configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)

if(CMAKE_BUILD_TYPE MATCHES Release)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file '${APP_SRC_DIR}/cmake/emscripten/index.html'")
elseif(CMAKE_BUILD_TYPE MATCHES Debug)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS")
endif()

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -s EXPORTED_RUNTIME_METHODS=ccall -s EXPORTED_FUNCTIONS=_main,_startup -s ALLOW_MEMORY_GROWTH -lidbfs.js -s FORCE_FILESYSTEM=1 \
--preload-file '${APP_SRC_DIR}/res/bg/highDetail@basepath/res/bg/highDetail' \
--preload-file '${APP_SRC_DIR}/res/bg/lowDetail@basepath/res/bg/lowDetail' \
--preload-file '${APP_SRC_DIR}/res/bgm@basepath/res/bgm' \
--preload-file '${APP_SRC_DIR}/res/font@basepath/res/font' \
--preload-file '${APP_SRC_DIR}/res/graphics/highDetail@basepath/res/graphics/highDetail' \
--preload-file '${APP_SRC_DIR}/res/graphics/lowDetail@basepath/res/graphics/lowDetail' \
--preload-file '${APP_SRC_DIR}/res/graphics/title@basepath/res/graphics/title' \
--preload-file '${APP_SRC_DIR}/res/se@basepath/res/se' \
--preload-file '${APP_SRC_DIR}/config/mission@basepath/config/mission' \
--preload-file '${APP_SRC_DIR}/config/stage@basepath/config/stage' \
")

add_executable(${APP_EXE} ${APP_SOURCES} ${APP_HEADERS})
set(CMAKE_EXECUTABLE_SUFFIX .html)

if(APP_VENDORED)
	if(TARGET SDL2::SDL2main)
		target_link_libraries("${APP_EXE}" PUBLIC SDL2::SDL2main)
	endif()
	target_link_libraries("${APP_EXE}" PUBLIC
		SDL2::SDL2-static
		SDL2_image::SDL2_image-static
		SDL2_mixer::SDL2_mixer-static
	)
endif()
