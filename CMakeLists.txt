cmake_minimum_required(VERSION 3.18)
project(HeborisC7EX-SDL2
	VERSION 1.0.1
	LANGUAGES C
)

set(PROJECT_VER "${CMAKE_PROJECT_VERSION}")
set(PROJECT_ORG "nightmareci")
set(PROJECT_APP "HeborisC7EX SDL2")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita.")

message(STATUS "Build target: ${BUILD_TARGET}")

# NOTE: All strings containing these must be quoted; if not quoted, and the
# path(s) happen to contain spaces, the build will fail.
set(SRC "${CMAKE_SOURCE_DIR}")
set(BIN "${CMAKE_BINARY_DIR}")

if(BUILD_TARGET STREQUAL Emscripten)
	set(EXE index)
else()
	set(EXE ${CMAKE_PROJECT_NAME})
endif()

set(EXE_SOURCES
	"${SRC}/src/game/gamestart.c"
	"${SRC}/src/game/init.c"
	"${SRC}/src/game/speed.c"

	"${SRC}/src/main_sdl/delay.c"
	"${SRC}/src/main_sdl/filesystem.c"
	"${SRC}/src/main_sdl/main.c"
	"${SRC}/src/main_sdl/SDL_kanji.c"
	"${SRC}/src/main_sdl/ygs2kfunc.c"
	"${SRC}/src/main_sdl/ygs2kinput.c"
	"${SRC}/src/main_sdl/ygs2kprivate.c"

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

set(EXE_HEADERS
	"${SRC}/src/game/button.h"
	"${SRC}/src/game/gamestart.h"
	"${SRC}/src/game/init.h"
	"${SRC}/src/game/speed.h"

	"${SRC}/src/main_sdl/delay.h"
	"${SRC}/src/main_sdl/gamecontroller.h"
	"${SRC}/src/main_sdl/include.h"
	"${SRC}/src/main_sdl/main.h"
	"${SRC}/src/main_sdl/filesystem.h"
	"${SRC}/src/main_sdl/SDL_kanji.h"
	"${SRC}/src/main_sdl/ygs2kfunc.h"
	"${SRC}/src/main_sdl/ygs2kinput.h"
	"${SRC}/src/main_sdl/ygs2kprivate.h"

	"${SRC}/src/script/ars.h"
	"${SRC}/src/script/classic_D.h"
	"${SRC}/src/script/classic.h"
	"${SRC}/src/script/config.h"
	"${SRC}/src/script/cpu.h"
	"${SRC}/src/script/effect.h"
	"${SRC}/src/script/flexdraw.h"
	"${SRC}/src/script/grade.h"
	"${SRC}/src/script/include.h"
	"${SRC}/src/script/mission.h"
	"${SRC}/src/script/mission_info.h"
	"${SRC}/src/script/practice.h"
	"${SRC}/src/script/ranking2.h"
	"${SRC}/src/script/ranking3.h"
	"${SRC}/src/script/ranking.h"
	"${SRC}/src/script/replay.h"
	"${SRC}/src/script/sectime.h"
	"${SRC}/src/script/setdef.h"
	"${SRC}/src/script/sound.h"
	"${SRC}/src/script/staffroll.h"
	"${SRC}/src/script/tomoyo.h"
	"${SRC}/src/script/view.h"
	"${SRC}/src/script/world.h"
)

set(DEFAULT_INPUT Desktop CACHE STRING "The default input type; this should be set to the type of input guaranteed available on a platform. Some platforms are hard set to override this option, so this setting can't be changed for them. Can be set to \"Desktop\" or \"Game Controller\" on configurable platforms.")

# Override these defaults for platforms as required. These defaults work on a
# lot of platforms, but some platforms, like game consoles, have to completely
# override these settings.
option(ENABLE_KEYBOARD "Enable input via keyboard" TRUE)
option(ENABLE_GAME_CONTROLLER "Enable input via game controllers (not the same as classic joystick input)" TRUE)
option(ENABLE_JOYSTICK "Enable input via any joystick device" TRUE)
option(ALL_VIDEO_SETTINGS "Enable all video settings; disabling this option hides some \"unsafe\" options" TRUE)
option(ENABLE_GAME_QUIT "Enables quitting the game by pressing escape at any time or selecting the \"QUIT\" option in the main menu" TRUE)

set(DEFAULT_JOYKEY_ASSIGN "{ 0 }")
set(SCREEN_SUBPIXEL_OFFSET "YGS2kGetScreenSubpixelOffset()")
set(DEFAULT_SCREEN_MODE "(YGS_SCREENMODE_WINDOW | YGS_SCREENMODE_DETAILLEVEL)")

set(FILESYSTEM_TYPE FILESYSTEM_WORKINGDIR)
set(BASE_PATH_APPEND "")

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
elseif(${BUILD_TARGET} STREQUAL Emscripten)
	include("${SRC}/cmake/emscripten/Configure.cmake")
else()
	message(FATAL_ERROR "Requested build target \"${BUILD_TARGET}\" is unsupported.")
endif()

target_include_directories(${EXE} PRIVATE
	"${SRC}/src" "${SRC}/src/game" "${SRC}/src/main_sdl"
	"${BIN}/src" "${BIN}/src/game" "${BIN}/src/main_sdl"
)

target_precompile_headers(${EXE} PRIVATE ${EXE_HEADERS})
