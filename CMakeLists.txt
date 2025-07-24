cmake_minimum_required(VERSION 3.18)
project(HeborisCE
	VERSION 1.1.1
	LANGUAGES C
)

set(APP_PROJECT_VERSION "${CMAKE_PROJECT_VERSION}")
set(APP_PROJECT_CREATOR "nightmareci")
set(APP_PROJECT_NAME "${CMAKE_PROJECT_NAME}")

# NOTE: Never, ever change APP_FILESYSTEM_ORG and APP_FILESYSTEM_APP, unless you're
# making a hard fork such that you're effectively making a new, distinct game,
# that needs its own separate save data, in which case you'd still stick with
# the new org/app names once the initially decided upon names are used in your
# first distributed release. So, carefully choose org/app to be highly unique,
# to increase likelihood no other program will ever clash with your choice;
# something based on a web domain is a reasonable choice. These variables
# together constitute the save data identifier for the game, so as long as the
# save data should maintain continuity through new release versions, these
# shouldn't be changed, even if the user-facing naming/branding changes.
#
# For maximum safety/portability, just stick to the subset of ASCII only
# including the English alphabet, numbers, space, hyphen, and never change
# alphabetic case once decided upon, while also not choosing names that differ
# from another application's names only in alphabetic case, which accommodates
# both case insensitive and sensitive filesystems.
#
# Heboris C.E. was originally called HeborisC7EX-SDL2, and was rebranded later
# to better indicate the direction of the project, so the project is stuck with
# the originally-chosen "HeborisC7EX SDL2" filesystem application name.
set(APP_FILESYSTEM_ORG "${APP_PROJECT_CREATOR}")
set(APP_FILESYSTEM_APP "HeborisC7EX SDL2")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(APP_BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita.")

message(STATUS "Build target: ${APP_BUILD_TARGET}")

# NOTE: All strings containing these must be quoted; if not quoted, and the
# path(s) happen to contain spaces, the build will fail.
set(APP_SRC_DIR "${CMAKE_SOURCE_DIR}")
set(APP_BIN_DIR "${CMAKE_BINARY_DIR}")

if(APP_BUILD_TARGET STREQUAL Emscripten)
	set(APP_EXE index)
else()
	set(APP_EXE ${CMAKE_PROJECT_NAME})
endif()

set(APP_SOURCES
	"${APP_SRC_DIR}/src/app/APP_main.c"
	"${APP_SRC_DIR}/src/app/APP.c"
	"${APP_SRC_DIR}/src/app/APP_bdf.c"
	"${APP_SRC_DIR}/src/app/APP_filesystem.c"
	"${APP_SRC_DIR}/src/app/nanotime.c"
	"${APP_SRC_DIR}/src/app/hashmap.c"

	"${APP_SRC_DIR}/src/game/gamestart.c"
	"${APP_SRC_DIR}/src/game/init.c"
	"${APP_SRC_DIR}/src/game/speed.c"
	"${APP_SRC_DIR}/src/game/mission_info.c"
	"${APP_SRC_DIR}/src/game/flexdraw.c"
	"${APP_SRC_DIR}/src/game/config.c"
	"${APP_SRC_DIR}/src/game/world.c"
	"${APP_SRC_DIR}/src/game/classic.c"
	"${APP_SRC_DIR}/src/game/ars.c"
	"${APP_SRC_DIR}/src/game/classic_D.c"
	"${APP_SRC_DIR}/src/game/effect.c"
	"${APP_SRC_DIR}/src/game/sound.c"
	"${APP_SRC_DIR}/src/game/practice.c"
	"${APP_SRC_DIR}/src/game/tomoyo.c"
	"${APP_SRC_DIR}/src/game/ranking.c"
	"${APP_SRC_DIR}/src/game/setdef.c"
	"${APP_SRC_DIR}/src/game/replay.c"
	"${APP_SRC_DIR}/src/game/staffroll.c"
	"${APP_SRC_DIR}/src/game/cpu.c"
	"${APP_SRC_DIR}/src/game/sectime.c"
	"${APP_SRC_DIR}/src/game/mission.c"
	"${APP_SRC_DIR}/src/game/view.c"
	"${APP_SRC_DIR}/src/game/grade.c"
	"${APP_SRC_DIR}/src/game/ranking2.c"
	"${APP_SRC_DIR}/src/game/ranking3.c"
)

set(APP_HEADERS
	"${APP_SRC_DIR}/src/app/APP.h"
	"${APP_SRC_DIR}/src/app/APP_common.h"
	"${APP_SRC_DIR}/src/app/APP_filesystem.h"
	"${APP_SRC_DIR}/src/app/APP_bdf.h"
	"${APP_SRC_DIR}/src/app/nanotime.h"
	"${APP_SRC_DIR}/src/app/hashmap.h"

	"${APP_SRC_DIR}/src/game/gamestart.h"
	"${APP_SRC_DIR}/src/game/init.h"
	"${APP_SRC_DIR}/src/game/speed.h"
	"${APP_SRC_DIR}/src/game/ars.h"
	"${APP_SRC_DIR}/src/game/classic_D.h"
	"${APP_SRC_DIR}/src/game/classic.h"
	"${APP_SRC_DIR}/src/game/config.h"
	"${APP_SRC_DIR}/src/game/cpu.h"
	"${APP_SRC_DIR}/src/game/effect.h"
	"${APP_SRC_DIR}/src/game/flexdraw.h"
	"${APP_SRC_DIR}/src/game/grade.h"
	"${APP_SRC_DIR}/src/game/common.h"
	"${APP_SRC_DIR}/src/game/mission.h"
	"${APP_SRC_DIR}/src/game/mission_info.h"
	"${APP_SRC_DIR}/src/game/practice.h"
	"${APP_SRC_DIR}/src/game/ranking2.h"
	"${APP_SRC_DIR}/src/game/ranking3.h"
	"${APP_SRC_DIR}/src/game/ranking.h"
	"${APP_SRC_DIR}/src/game/replay.h"
	"${APP_SRC_DIR}/src/game/sectime.h"
	"${APP_SRC_DIR}/src/game/setdef.h"
	"${APP_SRC_DIR}/src/game/sound.h"
	"${APP_SRC_DIR}/src/game/staffroll.h"
	"${APP_SRC_DIR}/src/game/tomoyo.h"
	"${APP_SRC_DIR}/src/game/view.h"
	"${APP_SRC_DIR}/src/game/world.h"
)

option(APP_VENDORED "Use vendor libraries (the version of libraries bundled with the code)" OFF)

# Override these defaults for platforms as required. These defaults work on a
# lot of platforms, but some platforms, like game consoles, have to completely
# override these settings.
option(APP_ENABLE_KEYBOARD "Enable input via keyboard" TRUE)
option(APP_ENABLE_GAME_CONTROLLER "Enable input via game controllers (not the same as classic joystick input)" TRUE)
option(APP_APP_ENABLE_GAME_CONTROLLER_DB "Enable loading gamecontrollerdb.txt for supporting more game controllers" TRUE)
option(APP_ENABLE_JOYSTICK "Enable input via any joystick device" TRUE)
option(APP_ENABLE_ALL_VIDEO_SETTINGS "Enable all video settings; disabling this option hides some \"unsafe\" options" TRUE)
option(APP_ENABLE_GAME_QUIT "Enables quitting the game by pressing escape at any time or selecting the \"QUIT\" option in the main menu" TRUE)

set(APP_DEFAULT_JOYKEY_ASSIGN "{ 0 }")
set(APP_SCREEN_SUBPIXEL_OFFSET "APP_GetScreenSubpixelOffset()")
set(APP_DEFAULT_SCREEN_MODE "(APP_SCREENMODE_WINDOW | APP_SCREENMODE_DETAILLEVEL)")

set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_WORKINGDIR)
set(APP_BASE_PATH_APPEND "")

if(${APP_BUILD_TARGET} STREQUAL Vita)
	include("${APP_SRC_DIR}/cmake/vita/Configure.cmake")
elseif(${APP_BUILD_TARGET} STREQUAL Windows)
	set(APP_PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	if(MINGW)
		include("${APP_SRC_DIR}/cmake/unix/Configure.cmake")
	elseif(WIN32 AND MSVC)
		include("${APP_SRC_DIR}/cmake/windows/Configure.cmake")
	else()
		message(FATAL_ERROR "Current Windows build setup is unsupported; only MinGW or MSVC are supported.")
	endif()
elseif(${APP_BUILD_TARGET} STREQUAL Darwin)
	add_compile_definitions(_POSIX_VERSION=200112L)
	set(APP_PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", \"Portable\", \"Installable Mac App\", or \"Portable Mac App\"")
	if(${APP_PACKAGE_TYPE} MATCHES Mac)
		include("${APP_SRC_DIR}/cmake/macos/Configure.cmake")
	else()
		include("${APP_SRC_DIR}/cmake/unix/Configure.cmake")
	endif()
elseif(${APP_BUILD_TARGET} STREQUAL Linux)
	add_compile_definitions(_POSIX_VERSION=200112L)
	set(APP_PACKAGE_TYPE WorkingDir CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	include("${APP_SRC_DIR}/cmake/unix/Configure.cmake")
elseif(${APP_BUILD_TARGET} STREQUAL Emscripten)
	include("${APP_SRC_DIR}/cmake/emscripten/Configure.cmake")
else()
	message(FATAL_ERROR "Requested build target \"${APP_BUILD_TARGET}\" is unsupported.")
endif()

target_include_directories(${APP_EXE} PRIVATE "${APP_SRC_DIR}/src" "${APP_BIN_DIR}/src")
