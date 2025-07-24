set(APP_FRAMEWORK_TYPE "VITA SDL2")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VITASDK)
	message(FATAL_ERROR "Vita SDK CMake toolchain file must be provided to the CMake configure command.")
else()
	include("${VITASDK}/share/vita.cmake" REQUIRED)
endif()

# Adding to CMAKE_PREFIX_PATH like this fixes find_package commands in Vita SDK
# builds.
list(APPEND CMAKE_PREFIX_PATH "${VITASDK}/arm-vita-eabi/lib/cmake")

# Vita SDK will fail linking the executable if position independent code is
# used in the vendor libraries.
set(CMAKE_POSITION_INDEPENDENT_CODE FALSE CACHE BOOL "Override" FORCE)

list(APPEND APP_SOURCES "${APP_SRC_DIR}/src/app/physfsrwops.c")
list(APPEND APP_HEADERS "${APP_SRC_DIR}/src/app/physfsrwops.h")

add_executable(${APP_EXE} ${APP_SOURCES} ${APP_HEADERS})

set(APP_ENABLE_KEYBOARD FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_JOYSTICK FALSE CACHE BOOL "" FORCE)
set(APP_ONLY_INPUT_TYPE APP_INPUT_PLAYSTATION CACHE STRING "" FORCE)
set(APP_ONLY_SDL_CONTROLLER_TYPE SDL_CONTROLLER_TYPE_PS4 CACHE STRING "" FORCE)
set(APP_ENABLE_GAME_CONTROLLER_DB FALSE CACHE BOOL "" FORCE)
set(APP_ENABLE_ALL_VIDEO_SETTINGS FALSE CACHE BOOL "" FORCE)
set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_PHYSFS CACHE STRING "" FORCE)
set(APP_DEFAULT_SCREEN_MODE "(APP_SCREENMODE_FULLSCREEN | APP_SCREENMODE_DETAILLEVEL | APP_SCREENMODE_VSYNC)" CACHE STRING "" FORCE)
set(APP_SCREEN_SUBPIXEL_OFFSET "0.0f" CACHE STRING "" FORCE)

configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "src/app/APP_build_config.h" @ONLY)

# The upstream PhysFS doesn't have Vita SDK support, so we need to use Vita
# SDK's included PhysFS, not the vendor version, thus the need for FALSE passed
# to AddLibrariesVendor and finding the PhysFS package.
#
# Also, as of last attempting Vita SDK builds, SDL2_mixer was broken with
# find_package, as it tries to link to libxmp-lite, which was missing in the
# Vita SDK packages; using the vendor version works fine, though.
include("${APP_SRC_DIR}/cmake/lib/AddLibrariesVendor.cmake")
AddLibrariesVendor(${APP_EXE} FALSE)
find_package(PhysFS REQUIRED)
target_link_libraries(${APP_EXE} PUBLIC ${PHYSFS_LIBRARY})
target_include_directories(${APP_EXE} PUBLIC ${PHYSFS_INCLUDE_DIR})

file(ARCHIVE_CREATE OUTPUT "${APP_BIN_DIR}/assets.zip"
	PATHS
		"${APP_SRC_DIR}/res/bg"
		"${APP_SRC_DIR}/res/bgm"
		"${APP_SRC_DIR}/res/font"
		"${APP_SRC_DIR}/res/graphics"
		"${APP_SRC_DIR}/res/se"
		"${APP_SRC_DIR}/config/mission"
		"${APP_SRC_DIR}/config/stage"
	FORMAT zip
)

set(VITA_APP_NAME ${APP_EXE})
set(VITA_TITLEID "NMCI00000")
set(VITA_VERSION "01.00")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

vita_create_self(${APP_EXE}.self ${APP_EXE})
vita_create_vpk(${APP_EXE}.vpk ${VITA_TITLEID} ${APP_EXE}.self
	VERSION ${VITA_VERSION}
	NAME "${VITA_APP_NAME}"

	# Game assets.
	FILE "${APP_BIN_DIR}/assets.zip" assets.zip

	# Files for LiveArea and boot.
	FILE "${APP_SRC_DIR}/pkg/vita/sce_sys/icon0.png" sce_sys/icon0.png
	FILE "${APP_SRC_DIR}/pkg/vita/sce_sys/pic0.png" sce_sys/pic0.png
	FILE "${APP_SRC_DIR}/pkg/vita/sce_sys/livearea/contents/bg0.png" sce_sys/livearea/contents/bg0.png
	FILE "${APP_SRC_DIR}/pkg/vita/sce_sys/livearea/contents/template.xml" sce_sys/livearea/contents/template.xml
)
