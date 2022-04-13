cmake_minimum_required(VERSION 3.18)
project(HeborisC7EX-SDL2
	VERSION 1.0.1
	LANGUAGES C
)

set(PROJECT_VER "${CMAKE_PROJECT_VERSION}")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

# NOTE: All strings containing these must be quoted; if not quoted, and the
# path(s) happen to contain spaces, the build will fail.
set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN ${CMAKE_CURRENT_BINARY_DIR})

set(EXE HeborisC7EX-SDL2)
set(EXE_SOURCES
	"${SRC}/src/game/gamestart.c"
	"${SRC}/src/game/speed.c"

	"${SRC}/src/main_sdl/main.c"
	"${SRC}/src/main_sdl/ygs2kfunc.c"
	"${SRC}/src/main_sdl/SDL_kanji.c"
	"${SRC}/src/main_sdl/physfsrwops.c"

	"${SRC}/src/script/mission_info.c"
	"${SRC}/src/script/flexdraw.c"
	"${SRC}/src/script/config.c"
	"${SRC}/src/script/world.c"
	"${SRC}/src/script/classic.c"
	"${SRC}/src/script/ars.c"
	"${SRC}/src/script/classic_D.c"
	"${SRC}/src/script/effect.c"
	"${SRC}/src/script/sound.c"
	"${SRC}/src/script/practice.c"
	"${SRC}/src/script/tomoyo.c"
	"${SRC}/src/script/ranking.c"
	"${SRC}/src/script/setdef.c"
	"${SRC}/src/script/replay.c"
	"${SRC}/src/script/staffroll.c"
	"${SRC}/src/script/cpu.c"
	"${SRC}/src/script/sectime.c"
	"${SRC}/src/script/mission.c"
	"${SRC}/src/script/view.c"
	"${SRC}/src/script/grade.c"
	"${SRC}/src/script/ranking2.c"
	"${SRC}/src/script/ranking3.c"
)

if(CMAKE_C_COMPILER_ID MATCHES Clang)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-source-encoding -Wno-logical-op-parentheses")
endif()

set(BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita.")

message(STATUS "Build target: ${BUILD_TARGET}")

# Override these defaults for platforms as required. These defaults are
# intended for "desktop" platforms, where keyboard input is guaranteed, with
# the game created in a window, in the high detail 640x480 screen mode.
set(DEFAULT_JOYKEY_ASSIGN "{ 0 }")
set(DEFAULT_SCREEN_MODE "(SCREENMODE_WINDOW | SCREENMODE_DETAILLEVEL)")
set(SCREEN_SUBPIXEL_OFFSET "GetScreenSubpixelOffset()")

option(USE_PHYSFS_SETSANECONFIG "If TRUE, PHYSFS_setSaneConfig is used for paths setup; FALSE uses SDL_filesystem APIs instead. Some platforms always set this TRUE for technical reasons." FALSE)

if(${BUILD_TARGET} STREQUAL Vita)
	include("${SRC}/cmake/vita/Configure.cmake")
elseif(${BUILD_TARGET} STREQUAL Windows)
	set(PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	if(MINGW)
		include("${SRC}/cmake/unix/Configure.cmake")
	elseif(WIN32 AND MSVC AND VCPKG_TOOLCHAIN)
		include("${SRC}/cmake/windows/Configure.cmake")
	else()
		message(FATAL_ERROR "Current Windows build setup is unsupported; MinGW with pkg-config or MSVC with VCPKG are supported.")
	endif()
elseif(${BUILD_TARGET} STREQUAL Darwin)
	set(PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", \"Portable\", \"Installable Mac App\", or \"Portable Mac App\"")
	if(${PACKAGE_TYPE} MATCHES Mac)
		include("${SRC}/cmake/macos/Configure.cmake")
	else()
		include("${SRC}/cmake/unix/Configure.cmake")
	endif()
elseif(${BUILD_TARGET} STREQUAL Linux)
	set(PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	include("${SRC}/cmake/unix/Configure.cmake")
else()
	message(FATAL_ERROR "Requested build target \"${BUILD_TARGET}\" is unsupported.")
endif()

target_include_directories(${EXE} PRIVATE
	"${SRC}/src" "${SRC}/src/game" "${SRC}/src/main_sdl"
	"${BIN}/src" "${BIN}/src/game" "${BIN}/src/main_sdl"
)
