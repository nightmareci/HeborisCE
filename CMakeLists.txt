cmake_minimum_required(VERSION 3.26)
project(HeborisCE
	VERSION 1.1.1
	LANGUAGES C
)

set(PROJECT_TITLE "Heboris C.E.")
set(PROJECT_IDENTIFIER "com.nightmareci.HeborisCE")
set(PROJECT_CREATOR "nightmareci")

# NOTE: Never, ever change FILESYSTEM_ORG and FILESYSTEM_APP, unless you're
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
set(FILESYSTEM_ORG "${PROJECT_CREATOR}")
set(FILESYSTEM_APP "HeborisC7EX SDL2")

get_cmake_property(GENERATOR_IS_MULTI_CONFIG GENERATOR_IS_MULTI_CONFIG)
if(VITA)
	# VitaSDK doesn't handle CMake output directories properly, so the
	# output directory *must* be the binary directory
	# TODO: Maybe an issue for supporting CMake output directory settings
	# should be submitted to the VitaSDK project
	set(OUTPUT_DIR "${CMAKE_BINARY_DIR}")
elseif(GENERATOR_IS_MULTI_CONFIG STREQUAL "NOTFOUND" OR NOT GENERATOR_IS_MULTI_CONFIG)
	string(LENGTH "${CMAKE_BUILD_TYPE}" BUILD_TYPE_LENGTH)
	if(BUILD_TYPE_LENGTH EQUAL 0)
		set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "CMake build type" FORCE)
	endif()
	set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}")
else()
	set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/$<CONFIG>")
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(EXECUTABLE index)
else()
	set(EXECUTABLE ${PROJECT_NAME})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(BUILD_VENDORED TRUE CACHE BOOL "Override" FORCE)
else()
	option(BUILD_VENDORED "Use vendor libraries (the version of libraries bundled with the code)" OFF)
endif()

option(FILESYSTEM_USING_RESOURCE_DAT "Use an uncompressed DAT archive for game resources" OFF)

# Override these defaults for platforms as required. These defaults work on a
# lot of platforms, but some platforms, like game consoles, have to completely
# override these settings.
option(INPUT_ENABLE_KEYBOARD "Enable input via keyboard" TRUE)
option(INPUT_ENABLE_GAME_CONTROLLER "Enable input via game controllers (not the same as classic joystick input)" TRUE)
option(INPUT_ENABLE_GAME_CONTROLLER_DB "Enable loading gamecontrollerdb.txt for supporting more game controllers" TRUE)
option(INPUT_ENABLE_JOYSTICK "Enable input via any joystick device" TRUE)
option(VIDEO_ENABLE_ALL_SETTINGS "Enable all video settings; disabling this option hides some \"unsafe\" settings" TRUE)
option(GAME_ENABLE_QUIT "Enables quitting by pressing escape at any time or selecting the \"QUIT\" main menu entry" TRUE)
if(VITA OR CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(PACKAGE_TYPE "Installable" CACHE STRING "Package type" FORCE)
elseif(WIN32 OR LINUX)
	set(PACKAGE_TYPE "Current Directory" CACHE STRING "Package type; must be \"Current Directory\", \"Installable\", or \"Portable\"")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(PACKAGE_TYPE "Current Directory" CACHE STRING "Package type; must be \"Current Directory\", \"Installable\", \"Portable\", \"Installable Mac App\", or \"Portable Mac App\"")
else()
	set(PACKAGE_TYPE "Installable" CACHE STRING "Package type")
endif()

if(VITA OR PACKAGE_TYPE STREQUAL "Installable Mac App")
	set(VIDEO_ENABLE_WINDOW_ICON_FILE FALSE CACHE BOOL "Enables setting the window icon dynamically at runtime" FORCE)
else()
	option(VIDEO_ENABLE_WINDOW_ICON_FILE "Enables setting the window icon dynamically at runtime" TRUE)
endif()
option(WORKER_MULTITHREADED "Use multiple threads for the worker API" TRUE)
set(INPUT_JOY_KEY_ASSIGN_DEFAULT "{ { 0 } }")
set(VIDEO_SCREEN_MODE_BASE "0")
set(VIDEO_SCREEN_MODE_DEFAULT "(VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP | VIDEO_SCREEN_MODE_DETAIL_LEVEL | VIDEO_SCREEN_MODE_RENDER_LEVEL)")
set(FILESYSTEM_BASE_PATH_APPEND "")

if(PACKAGE_TYPE STREQUAL "Current Directory")
	set(FILESYSTEM_TYPE "FILESYSTEM_CURRENT_DIRECTORY")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
	if(NOT WIN32 AND (LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR UNIX))
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
	endif()
elseif(PACKAGE_TYPE MATCHES "Installable")
	set(FILESYSTEM_TYPE "FILESYSTEM_INSTALLABLE")
	if(PACKAGE_TYPE STREQUAL "Installable Mac App")
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<TARGET_BUNDLE_CONTENT_DIR:${EXECUTABLE}>/Frameworks")
	elseif(LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR UNIX OR MSYS)
		include(GNUInstallDirs REQUIRED)
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${CMAKE_INSTALL_BINDIR}")
		if(MSYS)
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${CMAKE_INSTALL_BINDIR}")
		else()
			set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/${CMAKE_INSTALL_LIBDIR}")
		endif()
	else()
		set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
	endif()
elseif(PACKAGE_TYPE MATCHES "Portable")
	set(FILESYSTEM_TYPE "FILESYSTEM_PORTABLE")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
	if(PACKAGE_TYPE STREQUAL "Portable Mac App")
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "$<TARGET_BUNDLE_CONTENT_DIR:${EXECUTABLE}>/Frameworks")
	elseif(NOT WIN32 AND (LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR UNIX))
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
	else()
		set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}")
	endif()
endif()

add_compile_options(
	# Force MSVC to read sources in UTF-8 encoding
	"$<$<C_COMPILER_ID:MSVC>:/utf-8>"
	"$<$<CXX_COMPILER_ID:MSVC>:/utf-8>"
)

add_executable(${EXECUTABLE}
	"src/sys/src/AUDIO.c"
	"src/sys/src/BDF.c"
	"src/sys/src/BDF.h"
	"src/sys/src/ERROR.c"
	"src/sys/src/FILESYSTEM.c"
	"src/sys/src/GLOBAL.c"
	"src/sys/src/GLOBAL.h"
	"src/sys/src/HASHTABLE.c"
	"src/sys/src/HASHTABLE.h"
	"src/sys/src/INPUT.c"
	"src/sys/src/MAIN.c"
	"src/sys/src/VIDEO.c"

	"src/sys/include/AUDIO.h"
	"src/sys/include/ERROR.h"
	"src/sys/include/FILESYSTEM.h"
	"src/sys/include/INPUT.h"
	"src/sys/include/MAIN.h"
	"src/sys/include/STD.h"
	"src/sys/include/SYS.h"
	"src/sys/include/VIDEO.h"
	"src/sys/include/WORKER.h"

	"src/game/src/game.c"
	"src/game/src/init.c"
	"src/game/src/speed.c"
	"src/game/src/mission_info.c"
	"src/game/src/flexdraw.c"
	"src/game/src/config.c"
	"src/game/src/world.c"
	"src/game/src/classic.c"
	"src/game/src/ars.c"
	"src/game/src/classic_D.c"
	"src/game/src/effect.c"
	"src/game/src/sound.c"
	"src/game/src/practice.c"
	"src/game/src/tomoyo.c"
	"src/game/src/ranking.c"
	"src/game/src/setdef.c"
	"src/game/src/replay.c"
	"src/game/src/staffroll.c"
	"src/game/src/cpu.c"
	"src/game/src/sectime.c"
	"src/game/src/mission.c"
	"src/game/src/view.c"
	"src/game/src/grade.c"
	"src/game/src/ranking2.c"
	"src/game/src/ranking3.c"

	"src/game/include/game.h"
	"src/game/include/init.h"
	"src/game/include/speed.h"
	"src/game/include/ars.h"
	"src/game/include/classic_D.h"
	"src/game/include/classic.h"
	"src/game/include/config.h"
	"src/game/include/cpu.h"
	"src/game/include/effect.h"
	"src/game/include/flexdraw.h"
	"src/game/include/grade.h"
	"src/game/include/common.h"
	"src/game/include/mission.h"
	"src/game/include/mission_info.h"
	"src/game/include/practice.h"
	"src/game/include/ranking2.h"
	"src/game/include/ranking3.h"
	"src/game/include/ranking.h"
	"src/game/include/replay.h"
	"src/game/include/sectime.h"
	"src/game/include/setdef.h"
	"src/game/include/sound.h"
	"src/game/include/staffroll.h"
	"src/game/include/tomoyo.h"
	"src/game/include/view.h"
	"src/game/include/world.h"
	"src/game/include/plane.h"
)

set_target_properties(${EXECUTABLE}
	PROPERTIES
		C_STANDARD 99
		C_STANDARD_REQUIRED TRUE
)

if(LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR MINGW OR UNIX)
	add_compile_definitions(_POSIX_VERSION=200112L)
endif()

if(WORKER_MULTITHREADED)
	target_sources(${EXECUTABLE} PRIVATE "src/sys/src/WORKER_multithreaded.c")
else()
	target_sources(${EXECUTABLE} PRIVATE "src/sys/src/WORKER_synchronous.c")
endif()

# // The on-disk format is unheadered and uses file extension .dat
# // At the start of the .dat, there's an int64_t for how many files there are (the array length of FILESYSTEM_FilesArray)
# // After the file count, there's a list of FILESYSTEM_FileData entries
# // This isn't actual code, just a description of the on-disk format
# typedef struct FILESYSTEM_FileData
# {
# 	char filename[]; // UTF-8 string
# 	int64_t fileSize; // int64_t to correspond to file sizes in SDL3
# 	uint8_t file[fileSize];
# } FILESYSTEM_FileData;
#
# typedef struct FILESYSTEM_File
# {
# 	int64_t start; // Starting offset into the .dat where FILESYSTEM_FileData::file is located
# 	int64_t size; // FILESYSTEM_FileData::fileSize
# } FILESYSTEM_File;
#
# // All the FILESYSTEM_File objects are represented as one contiguous array, simplifies memory management
# FILESYSTEM_File* FILESYSTEM_FilesArray;
#
# // FILESYSTEM_FilesTable is indexed by the filename (FILESYSTEM_FileData::filename), e.g.
# // "res/graphics/highDetail/title.png"
# //
# // Each entry in FILESYSTEM_FilesTable stores a pointer to an
# // FILESYSTEM_File. When a file is requested to be opened, a custom-interface
# // SDL_IOStream is created, which internally handles bounds checking etc., so
# // the stream can be used as if it only represents a single file with
# // ordinary stream functions, not a normal interface stream of the .dat
# SDL_PropertiesID FILESYSTEM_FilesTable;
function(GenerateResourceDAT DAT_FILE)
	set(RES_FILES)
	foreach(FILE_TYPE "png" "ogg" "bdf" "wav")
		file(GLOB_RECURSE FILES RELATIVE "${CMAKE_SOURCE_DIR}" "res/*.${FILE_TYPE}")
		list(APPEND RES_FILES ${FILES})
	endforeach()
	file(GLOB MISSION_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/mission/*.sav")
	file(GLOB STAGE_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/stage/*.sav")
	set(ALL_FILES ${RES_FILES} ${MISSION_FILES} ${STAGE_FILES})
	if(VIDEO_ENABLE_WINDOW_ICON_FILE)
		list(APPEND ALL_FILES "${PROJECT_NAME}.png")
	endif()

	add_custom_command(
		OUTPUT "${DAT_FILE}"
		DEPENDS ${ALL_FILES}
		COMMAND python3 "${CMAKE_SOURCE_DIR}/pkg/script/gen_res_dat.py" "${DAT_FILE}" "${CMAKE_SOURCE_DIR}/" ${ALL_FILES}
		VERBATIM
	)
	add_custom_target("${PROJECT_NAME}_dat"
		DEPENDS "${DAT_FILE}"
		VERBATIM
	)
	add_dependencies(${EXECUTABLE} "${PROJECT_NAME}_dat")
endfunction()

function(CopyResourcesToDestination DESTINATION_DIR)
	if(VIDEO_ENABLE_WINDOW_ICON_FILE)
		add_custom_command(
			OUTPUT "${DESTINATION_DIR}/${PROJECT_NAME}.png"
			DEPENDS "${PROJECT_NAME}.png"
			COMMAND "${CMAKE_COMMAND}" -E copy_if_different "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.png" "${DESTINATION_DIR}"
			VERBATIM
		)
		add_custom_target("${PROJECT_NAME}_output_icon_file"
			DEPENDS "${DESTINATION_DIR}/${PROJECT_NAME}.png"
			VERBATIM
		)
		add_dependencies(${EXECUTABLE} "${PROJECT_NAME}_output_icon_file")
	endif()

	file(GLOB_RECURSE RES_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "res/*")
	file(GLOB MISSION_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/mission/*")
	file(GLOB STAGE_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/stage/*")
	set(DEPENDS_FILES ${RES_FILES} ${MISSION_FILES} ${STAGE_FILES})
	set(OUTPUT_FILES)
	foreach(FILE ${DEPENDS_FILES})
		list(APPEND OUTPUT_FILES "${DESTINATION_DIR}/${FILE}")
	endforeach()
	add_custom_command(
		OUTPUT ${OUTPUT_FILES}
		DEPENDS ${DEPENDS_FILES}
		COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/res" "${DESTINATION_DIR}/res"
		COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/config/mission" "${DESTINATION_DIR}/config/mission"
		COMMAND "${CMAKE_COMMAND}" -E copy_directory_if_different "${CMAKE_SOURCE_DIR}/config/stage" "${DESTINATION_DIR}/config/stage"
		VERBATIM
	)
	add_custom_target("${PROJECT_NAME}_output_files"
		DEPENDS ${OUTPUT_FILES}
		VERBATIM
	)
	add_dependencies(${EXECUTABLE} "${PROJECT_NAME}_output_files")
endfunction()

if(VITA)
	set(SYS_PLATFORM "VITA SDL3")

	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VITASDK)
		message(FATAL_ERROR "Vita SDK CMake toolchain file must be provided to the CMake configure command.")
	else()
		include("${VITASDK}/share/vita.cmake" REQUIRED)
	endif()

	target_sources(${EXECUTABLE} PRIVATE "src/sys/src/VITA.c")

	# Adding to CMAKE_PREFIX_PATH like this fixes find_package commands in Vita SDK
	# builds.
	list(APPEND CMAKE_PREFIX_PATH "${VITASDK}/arm-vita-eabi/lib/cmake")

	# Vita SDK will fail linking the executable if position independent code is
	# used in the vendor libraries.
	set(CMAKE_POSITION_INDEPENDENT_CODE FALSE CACHE BOOL "Override" FORCE)

	set(INPUT_ENABLE_KEYBOARD FALSE CACHE BOOL "" FORCE)
	set(INPUT_ENABLE_JOYSTICK FALSE CACHE BOOL "" FORCE)
	set(INPUT_ONLY_DEVICE_TYPE INPUT_DEVICE_TYPE_PLAYSTATION CACHE STRING "" FORCE)
	set(INPUT_ONLY_SDL_CONTROLLER_TYPE SDL_GAMEPAD_TYPE_PS4 CACHE STRING "" FORCE)
	set(INPUT_ENABLE_GAME_CONTROLLER_DB FALSE CACHE BOOL "" FORCE)
	set(VIDEO_ENABLE_ALL_SETTINGS FALSE CACHE BOOL "" FORCE)
	set(VIDEO_SCREEN_MODE_BASE "(VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP | VIDEO_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)
	set(VIDEO_SCREEN_MODE_DEFAULT "(VIDEO_SCREEN_MODE_FULLSCREEN_DESKTOP | VIDEO_SCREEN_MODE_DETAIL_LEVEL | VIDEO_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)

	set(VITA_APP_NAME ${PROJECT_NAME})
	set(VITA_TITLEID "NMCI00000")
	set(VITA_VERSION "01.00")
	set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

	if(BUILD_VENDORED)
		set(BUILD_SHARED_LIBS OFF)

		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_DEPS_SHARED OFF)
 		# TODO: Use libpng instead of stb once vendored zlib builds for all platforms
		set(SDLIMAGE_PNG_LIBPNG OFF)
		set(SDLIMAGE_BACKEND_STB ON)
		# TODO: Enable TIFF once vendored zlib builds for all platforms
		set(SDLIMAGE_TIF OFF)
		# TODO: Enable AVIF when it builds for all platforms
		set(SDLIMAGE_AVIF OFF)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		set(SDLMIXER_VENDORED ON)
		set(SDLMIXER_AIFF OFF)
		set(SDLMIXER_WAVE ON)
		set(SDLMIXER_VOC OFF)
		set(SDLMIXER_AU OFF)
		set(SDLMIXER_FLAC OFF)
		set(SDLMIXER_GME OFF)
		set(SDLMIXER_MOD OFF)
		set(SDLMIXER_MP3 OFF)
		set(SDLMIXER_MIDI OFF)
		set(SDLMIXER_OPUS OFF)
		# TODO: Use libvorbis instead of stb when libvorbis builds for all platforms
		set(SDLMIXER_VORBIS_STB ON)
		set(SDLMIXER_VORBIS_VORBISFILE OFF)
		set(SDLMIXER_WAVPACK OFF)
		add_subdirectory(dep/SDL_mixer EXCLUDE_FROM_ALL)

		find_package(Threads REQUIRED)
		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
			Threads::Threads
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		find_package(SDL3_mixer REQUIRED)
		find_package(Threads REQUIRED)
		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
			Threads::Threads
		)
	endif()

	if(FILESYSTEM_USING_RESOURCE_DAT)
		GenerateResourceDAT("${OUTPUT_DIR}/${PROJECT_NAME}.dat")
		set(ASSETS
			FILE "${OUTPUT_DIR}/${PROJECT_NAME}.dat" "${PROJECT_NAME}.dat"
		)
	else()
		set(ASSETS
			FILE "res" "res"
			FILE "config/mission" "config/mission"
			FILE "config/stage" "config/stage"
		)
	endif()
	vita_create_self(${PROJECT_NAME}.self ${EXECUTABLE})
	vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
		VERSION ${VITA_VERSION}
		NAME "${VITA_APP_NAME}"

		# Game assets.
		${ASSETS}

		# Text files.
		FILE "changelog.txt" "changelog.txt"
		FILE "heboris.txt" "heboris.txt"
		FILE "README.md" "README.md"

		# Files for LiveArea and boot.
		FILE "pkg/vita/sce_sys/icon0.png" "sce_sys/icon0.png"
		FILE "pkg/vita/sce_sys/pic0.png" "sce_sys/pic0.png"
		FILE "pkg/vita/sce_sys/livearea/contents/bg0.png" "sce_sys/livearea/contents/bg0.png"
		FILE "pkg/vita/sce_sys/livearea/contents/template.xml" "sce_sys/livearea/contents/template.xml"
	)
elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	# TODO: Add option to not include --emrun in linker flags for actual
	# releases, so they're suitable for hosting on a real web server.
	# TODO: Add "favicon.ico".

	if(BUILD_VENDORED)
		set(BUILD_SHARED_LIBS OFF)

		set(SDL_PTHREADS ON CACHE BOOL "" FORCE)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_AVIF OFF)
		set(SDLIMAGE_BMP OFF)
		set(SDLIMAGE_GIF OFF)
		set(SDLIMAGE_JPG OFF)
		set(SDLIMAGE_JXL OFF)
		set(SDLIMAGE_LBM OFF)
		set(SDLIMAGE_PCX OFF)
		# TODO: Use libpng instead of stb once vendored zlib builds for all platforms
		set(SDLIMAGE_PNG_LIBPNG OFF)
		set(SDLIMAGE_BACKEND_STB ON)
		set(SDLIMAGE_PNG ON)
		set(SDLIMAGE_PNM OFF)
		set(SDLIMAGE_QOI OFF)
		set(SDLIMAGE_SVG OFF)
		set(SDLIMAGE_TGA OFF)
		set(SDLIMAGE_TIF OFF)
		set(SDLIMAGE_WEBP OFF)
		set(SDLIMAGE_XCF OFF)
		set(SDLIMAGE_XPM OFF)
		set(SDLIMAGE_XV OFF)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		set(SDLMIXER_VENDORED ON)
		set(SDLMIXER_AIFF OFF)
		set(SDLMIXER_VOC OFF)
		set(SDLMIXER_AU OFF)
		set(SDLMIXER_FLAC OFF)
		set(SDLMIXER_GME OFF)
		set(SDLMIXER_MOD OFF)
		set(SDLMIXER_MP3 OFF)
		set(SDLMIXER_MIDI OFF)
		set(SDLMIXER_OPUS OFF)
		# TODO: Use libvorbis instead of stb when libvorbis builds for all platforms
		set(SDLMIXER_VORBIS_STB ON)
		set(SDLMIXER_VORBIS_VORBISFILE OFF)
		set(SDLMIXER_WAVE ON)
		set(SDLMIXER_WAVPACK OFF)
		add_subdirectory(dep/SDL_mixer EXCLUDE_FROM_ALL)

		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)

		# TODO: Support non-vendored, when possible. emsdk doesn't provide SDL3_image yet, so only vendored works
		# However, if the emsdk SDL3 package doesn't have SDL_PTHREADS, then vendored will always be required
	endif()

	set(SYS_PLATFORM "WEB SDL3")

	set(INPUT_ENABLE_JOYSTICK FALSE CACHE BOOL "" FORCE)
	set(INPUT_ENABLE_GAME_CONTROLLER TRUE CACHE BOOL "" FORCE)
	set(INPUT_ENABLE_GAME_CONTROLLER_DB FALSE CACHE BOOL "" FORCE)
	set(VIDEO_ENABLE_ALL_SETTINGS FALSE CACHE BOOL "" FORCE)
	set(VIDEO_SCREEN_MODE_BASE "(VIDEO_SCREEN_MODE_WINDOW | VIDEO_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)
	set(VIDEO_SCREEN_MODE_DEFAULT "(VIDEO_SCREEN_MODE_WINDOW | VIDEO_SCREEN_MODE_DETAIL_LEVEL | VIDEO_SCREEN_MODE_RENDER_LEVEL | VIDEO_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)
	set(GAME_ENABLE_QUIT FALSE CACHE BOOL "" FORCE)
	set(FILESYSTEM_BASE_PATH_APPEND "basepath/")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file '${CMAKE_SOURCE_DIR}/pkg/emscripten/index.html'")
	elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS")
	endif()
	if(FILESYSTEM_USING_RESOURCE_DAT)
		GenerateResourceDAT("${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dat")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -pthread -lidbfs.js -s PTHREAD_POOL_SIZE=30 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1 \
			--preload-file '${CMAKE_BINARY_DIR}/${PROJECT_NAME}.dat@basepath/${PROJECT_NAME}.dat' \
		")
	else()
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -pthread -lidbfs.js -s PTHREAD_POOL_SIZE=30 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1 \
			--preload-file '${CMAKE_SOURCE_DIR}/${PROJECT_NAME}.png@basepath/${PROJECT_NAME}.png' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/bg/highDetail@basepath/res/bg/highDetail' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/bg/lowDetail@basepath/res/bg/lowDetail' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/bgm@basepath/res/bgm' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/font@basepath/res/font' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/graphics/highDetail@basepath/res/graphics/highDetail' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/graphics/lowDetail@basepath/res/graphics/lowDetail' \
			--preload-file '${CMAKE_SOURCE_DIR}/res/se@basepath/res/se' \
			--preload-file '${CMAKE_SOURCE_DIR}/config/mission@basepath/config/mission' \
			--preload-file '${CMAKE_SOURCE_DIR}/config/stage@basepath/config/stage' \
		")
	endif()

	set_property(TARGET ${EXECUTABLE} PROPERTY SUFFIX ".html")
elseif(WIN32 AND NOT MSYS)
	set(SYS_PLATFORM "WINDOWS SDL3")

	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(WIN32_EXECUTABLE_DEFAULT OFF)
	else()
		set(WIN32_EXECUTABLE_DEFAULT ON)
	endif()
	option(CMAKE_WIN32_EXECUTABLE "Hide the Windows console." ${WIN32_EXECUTABLE_DEFAULT})

	target_sources(${EXECUTABLE} PRIVATE "pkg/windows/HeborisCE.rc")

	if(BUILD_VENDORED)
		set(BUILD_SHARED_LIBS ON)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_DEPS_SHARED ON)
		# TODO: Use libpng instead of stb once vendored zlib builds for all platforms
		set(SDLIMAGE_PNG_LIBPNG OFF)
		set(SDLIMAGE_BACKEND_STB ON)
		# TODO: Enable TIFF once vendored zlib builds for all platforms
		set(SDLIMAGE_TIF OFF)
		# TODO: Enable AVIF when it builds for all platforms
		set(SDLIMAGE_AVIF OFF)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		set(SDLMIXER_VENDORED ON)
		set(SDLMIXER_DEPS_SHARED ON)
		# TODO: Use libvorbis instead of stb when libvorbis builds for all platforms
		set(SDLMIXER_VORBIS_STB ON)
		set(SDLMIXER_VORBIS_VORBISFILE OFF)
		# TODO: Enable WavPack when it builds for all platforms
		set(SDLMIXER_WAVPACK OFF)
		add_subdirectory(dep/SDL_mixer EXCLUDE_FROM_ALL)

		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		find_package(SDL3_mixer REQUIRED)
		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
	endif()

	if(FILESYSTEM_USING_RESOURCE_DAT)
		GenerateResourceDAT("${OUTPUT_DIR}/${PROJECT_NAME}.dat")
	elseif(NOT PACKAGE_TYPE STREQUAL "Current Directory")
		CopyResourcesToDestination("${OUTPUT_DIR}")
	endif()
	if(
		PACKAGE_TYPE STREQUAL "Portable" OR
		PACKAGE_TYPE STREQUAL "Installable"
	)
		install(TARGETS ${EXECUTABLE} DESTINATION ".")
		if(BUILD_VENDORED)
			install(CODE "
				file(GLOB LIBS \"$<TARGET_FILE_DIR:${EXECUTABLE}>/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
				file(INSTALL
					DESTINATION \"\${CMAKE_INSTALL_PREFIX}\"
					TYPE SHARED_LIBRARY
					FOLLOW_SYMLINK_CHAIN
					FILES \${LIBS}
				)
			")
		endif()
		if(FILESYSTEM_USING_RESOURCE_DAT)
			install(FILES "${OUTPUT_DIR}/${PROJECT_NAME}.dat" DESTINATION ".")
		else()
			install(FILES "${PROJECT_NAME}.png" DESTINATION ".")
			install(DIRECTORY "config/mission" "config/stage" DESTINATION "config")
			install(DIRECTORY "res" DESTINATION ".")
		endif()
		install(FILES "changelog.txt" "heboris.txt" "README.md" DESTINATION ".")

		set(CPACK_GENERATOR "ZIP" CACHE STRING "The CPack generator to use")
		set(CPACK_PACKAGE_VENDOR "${PROJECT_CREATOR}")
		if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(AMD64|IA64|EM64T)$") # x86-64 names are a mess, but this covers all the possible values
			set(PROCESSOR "x64")
		else() # For anything else, the CMake variable probably corresponds to what we want (e.g. x86 and ARM64)
			set(PROCESSOR "${CMAKE_SYSTEM_PROCESSOR}")
		endif()
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PACKAGE_TYPE}-Windows-${PROCESSOR}")
		set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
		include(CPack)
	elseif(NOT PACKAGE_TYPE STREQUAL "Current Directory")
		message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported when building for Windows")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND PACKAGE_TYPE MATCHES "Mac App")
	set(SYS_PLATFORM "MACOSX SDL3")

	# Builds Mac application bundle, packaged into a DMG with CPack by default.

	if(NOT CMAKE_GENERATOR STREQUAL "Xcode")
		message(FATAL_ERROR "Building Mac apps is only supported by the Xcode generator")
	endif()

	set_property(TARGET ${EXECUTABLE} PROPERTY MACOSX_BUNDLE TRUE)

	if(FILESYSTEM_USING_RESOURCE_DAT)
		GenerateResourceDAT("${OUTPUT_DIR}/${PROJECT_NAME}.dat")
	endif()
	if(PACKAGE_TYPE MATCHES "Portable")
		set(SDL_FILESYSTEM_BASE_DIR_TYPE "parent")
		install(TARGETS ${EXECUTABLE} BUNDLE DESTINATION "${PROJECT_NAME}")
		install(FILES "changelog.txt" "heboris.txt" "README.md" DESTINATION "${PROJECT_NAME}")
		if(FILESYSTEM_USING_RESOURCE_DAT)
			install(FILES "${OUTPUT_DIR}/${PROJECT_NAME}.dat" DESTINATION "${PROJECT_NAME}")
		else()
			CopyResourcesToDestination("${OUTPUT_DIR}")
			install(DIRECTORY "config/mission" "config/stage" DESTINATION "${PROJECT_NAME}/config")
			install(DIRECTORY "res" DESTINATION "${PROJECT_NAME}")
		endif()
	elseif(PACKAGE_TYPE MATCHES "Installable")
		set(SDL_FILESYSTEM_BASE_DIR_TYPE "resource")
		install(TARGETS ${EXECUTABLE} BUNDLE DESTINATION .)
		install(FILES "changelog.txt" "heboris.txt" "README.md" DESTINATION .)
		if(FILESYSTEM_USING_RESOURCE_DAT)
			set_source_files_properties("${OUTPUT_DIR}/${PROJECT_NAME}.dat" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
		else()
			set(RES_FILES)
			foreach(FILE_TYPE "png" "ogg" "bdf" "wav")
				file(GLOB_RECURSE FILES RELATIVE "${CMAKE_SOURCE_DIR}" "res/*.${FILE_TYPE}")
				list(APPEND RES_FILES ${FILES})
			endforeach()
			file(GLOB MISSION_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/mission/*.sav")
			file(GLOB STAGE_FILES RELATIVE "${CMAKE_SOURCE_DIR}" "config/stage/*.sav")
			set(ALL_FILES ${RES_FILES} ${MISSION_FILES} ${STAGE_FILES})
			foreach(FILE ${ALL_FILES})
				cmake_path(GET FILE PARENT_PATH PATH)
				target_sources(${EXECUTABLE} PRIVATE "${FILE}")
				set_source_files_properties("${FILE}" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources/${PATH}")
			endforeach()
		endif()
	else()
		message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported when building a macOS app")
	endif()

	set(CPACK_GENERATOR "DragNDrop" CACHE STRING "The CPack generator to use")

	set(CPACK_BUNDLE_APPLE_CERT_APP "-" CACHE STRING "The name of the code signing certificate to use")
	if(CPACK_BUNDLE_APPLE_CERT_APP STREQUAL "-")
		set(CPACK_BUNDLE_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/pkg/macos/entitlements-adhoc.plist")
	else()
		set(CPACK_BUNDLE_ENTITLEMENTS "${CMAKE_SOURCE_DIR}/pkg/macos/entitlements-identity-required.plist")
	endif()
	set(MACOSX_BUNDLE_BUNDLE_NAME ${PROJECT_NAME})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_IDENTIFIER}")
	set(MACOSX_BUNDLE_ICON_FILE ${PROJECT_NAME})
	option(BUILD_INJECT_BASE_ENTITLEMENTS "Inject base entitlements when building Mac apps" YES)
	set_target_properties(${EXECUTABLE} PROPERTIES
		MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/cmake/MacOSXBundleInfo.plist.cmake"
		XCODE_ATTRIBUTE_BUNDLE_IDENTIFIER "${PROJECT_IDENTIFIER}"
		XCODE_ATTRIBUTE_PRODUCT_BUNDLE_IDENTIFIER "${PROJECT_IDENTIFER}"
		XCODE_ATTRIBUTE_AUTOMATIC_SIGNING YES
		XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED TRUE
		XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "${CPACK_BUNDLE_APPLE_CERT_APP}"
		XCODE_ATTRIBUTE_CODE_SIGN_ENTITLEMENTS "${CPACK_BUNDLE_ENTITLEMENTS}"
		XCODE_ATTRIBUTE_OTHER_CODE_SIGN_FLAGS "--timestamp --options runtime"
		XCODE_ATTRIBUTE_CODE_SIGN_INJECT_BASE_ENTITLEMENTS ${BUILD_INJECT_BASE_ENTITLEMENTS}
		INSTALL_RPATH "@executable_path/../Frameworks"
		BUILD_WITH_INSTALL_RPATH ON
	)
	target_sources(${EXECUTABLE} PRIVATE "pkg/macos/${PROJECT_NAME}.icns")
	set_source_files_properties("pkg/macos/${PROJECT_NAME}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

	if(BUILD_VENDORED)
		set(BUILD_SHARED_LIBS ON)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_DEPS_SHARED ON)
		# TODO: Use libpng instead of stb once vendored zlib builds for all platforms
		set(SDLIMAGE_PNG_LIBPNG OFF)
		set(SDLIMAGE_BACKEND_STB ON)
		# TODO: Enable TIFF once veondred zlib builds for all platforms
		set(SDLIMAGE_TIF OFF)
		# TODO: Enable AVIF when it builds for all platforms
		set(SDLIMAGE_AVIF OFF)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		set(SDLMIXER_VENDORED ON)
		set(SDLMIXER_DEPS_SHARED ON)
		# TODO: Use libvorbis instead of stb when libvorbis builds for all platforms
		set(SDLMIXER_VORBIS_STB ON)
		set(SDLMIXER_VORBIS_VORBISFILE OFF)
		# TODO: Enable WavPack when it builds for all platforms
		set(SDLMIXER_WAVPACK OFF)
		add_subdirectory(dep/SDL_mixer EXCLUDE_FROM_ALL)

		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
		add_custom_command(TARGET ${EXECUTABLE} POST_BUILD
			COMMAND "${CMAKE_SOURCE_DIR}/cmake/CodeSignDylibs.sh" "${PROJECT_NAME}.app/Contents/Frameworks"
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		find_package(SDL3_mixer REQUIRED)
		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
	endif()

	set(CPACK_DMG_VOLUME_NAME ${PROJECT_NAME})
	set(CPACK_PACKAGE_VENDOR "${PROJECT_CREATOR}")
	if(PACKAGE_TYPE MATCHES "Portable")
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-Portable-App-macOS")
	elseif(${PACKAGE_TYPE} MATCHES "Installable")
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-Installable-App-macOS")
	endif()
	if(CPACK_GENERATOR STREQUAL "DragNDrop")
		set(CPACK_POST_BUILD_SCRIPTS "${CMAKE_SOURCE_DIR}/cmake/CPackDragNDropCodeSignDMGs.cmake")
	endif()
	include(CPack)
elseif(LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR MSYS OR UNIX)
	if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
		set(SYS_PLATFORM "MACOSX SDL3")
	else()
		# It's probably a fair assumption that system names are always ASCII,
		# so it's likely no conversion to ASCII is needed here; the framework
		# version is displayed using the game font, not the Unicode font, so it
		# must be ASCII.
		string(TOUPPER "${CMAKE_SYSTEM_NAME}" SYS_SYSTEM_NAME)
		set(SYS_PLATFORM "${SYS_SYSTEM_NAME} SDL3")
	endif()

	# Builds commandline program for some Unix-type platforms. The "Portable" package type supports creating distributable packages with CPack.

	if(LINUX)
		option(INPUT_ENABLE_LINUX_GPIO "Enable input via Linux GPIO" OFF)
	else()
		set(INPUT_ENABLE_LINUX_GPIO OFF CACHE BOOL "Enable input via Linux GPIO" FORCE)
	endif()

	unset(RPATH_ORIGIN)
	if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
		set(RPATH_ORIGIN "@executable_path")
	elseif(NOT MSYS AND (LINUX OR UNIX))
		set(RPATH_ORIGIN "$ORIGIN")
	endif()

	if(PACKAGE_TYPE STREQUAL "Current Directory")
		set(FILESYSTEM_USING_RESOURCE_DAT_FILE "${OUTPUT_DIR}/${PROJECT_NAME}.dat")
		if(FILESYSTEM_USING_RESOURCE_DAT)
			GenerateResourceDAT("${FILESYSTEM_USING_RESOURCE_DAT_FILE}")
		endif()
		if(BUILD_VENDORED)
			if(DEFINED RPATH_ORIGIN)
				set_target_properties(${EXECUTABLE} PROPERTIES
					INSTALL_RPATH "${RPATH_ORIGIN}/lib"
					BUILD_WITH_INSTALL_RPATH ON
				)
			endif()
		endif()
	elseif(PACKAGE_TYPE STREQUAL "Portable")
		set(FILESYSTEM_USING_RESOURCE_DAT_FILE "${OUTPUT_DIR}/${PROJECT_NAME}.dat")
		if(FILESYSTEM_USING_RESOURCE_DAT)
			GenerateResourceDAT("${FILESYSTEM_USING_RESOURCE_DAT_FILE}")
		else()
			CopyResourcesToDestination("${OUTPUT_DIR}")
		endif()
		if(BUILD_VENDORED)
			if(DEFINED RPATH_ORIGIN)
				set_target_properties(${EXECUTABLE} PROPERTIES
					INSTALL_RPATH "${RPATH_ORIGIN}/lib"
					BUILD_WITH_INSTALL_RPATH ON
				)
			endif()
		endif()

		install(TARGETS ${EXECUTABLE} DESTINATION ".")
		if(BUILD_VENDORED)
			if(DEFINED ENV{DESTDIR})
				set(DESTDIR "$ENV{DESTDIR}")
			else()
				set(DESTDIR "")
			endif()
			install(CODE "
				file(GLOB LIBS \"$<TARGET_FILE_DIR:${EXECUTABLE}>/lib/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
				file(INSTALL
					DESTINATION \"${DESTDIR}\${CMAKE_INSTALL_PREFIX}/lib\"
					TYPE SHARED_LIBRARY
					FOLLOW_SYMLINK_CHAIN
					FILES \${LIBS}
				)
			")
		endif()
		if(FILESYSTEM_USING_RESOURCE_DAT)
			install(FILES "${FILESYSTEM_USING_RESOURCE_DAT_FILE}" DESTINATION ".")
		else()
			install(FILES "${PROJECT_NAME}.png" DESTINATION ".")
			install(DIRECTORY "config/mission" "config/stage" DESTINATION "config")
			install(DIRECTORY "res" DESTINATION ".")
		endif()
		install(FILES "changelog.txt" "heboris.txt" "README.md" DESTINATION ".")
	elseif(PACKAGE_TYPE STREQUAL "Installable")
		set(DATA_DIR "${OUTPUT_DIR}/${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
		set(FILESYSTEM_USING_RESOURCE_DAT_FILE "${DATA_DIR}/${PROJECT_NAME}.dat")
		if(FILESYSTEM_USING_RESOURCE_DAT)
			GenerateResourceDAT("${FILESYSTEM_USING_RESOURCE_DAT_FILE}")
		else()
			CopyResourcesToDestination("${DATA_DIR}")
		endif()
		if(BUILD_VENDORED)
			if(DEFINED RPATH_ORIGIN)
				set_target_properties(${EXECUTABLE} PROPERTIES
					INSTALL_RPATH "${RPATH_ORIGIN}/../${CMAKE_INSTALL_LIBDIR}"
					BUILD_WITH_INSTALL_RPATH ON
				)
			endif()
		endif()
		set(FILESYSTEM_BASE_PATH_APPEND "../${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/")

		install(TARGETS ${EXECUTABLE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
		if(BUILD_VENDORED)
			if(DEFINED RPATH_ORIGIN)
				set(LIBDIR "${CMAKE_INSTALL_LIBDIR}")
			else()
				set(LIBDIR "${CMAKE_INSTALL_BINDIR}")
			endif()
			if(DEFINED ENV{DESTDIR})
				set(DESTDIR "$ENV{DESTDIR}")
			else()
				set(DESTDIR "")
			endif()
			install(CODE "
				file(GLOB LIBS \"$<TARGET_FILE_DIR:${EXECUTABLE}>/../${LIBDIR}/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
				file(INSTALL
					DESTINATION \"${DESTDIR}\${CMAKE_INSTALL_PREFIX}/${LIBDIR}\"
					TYPE SHARED_LIBRARY
					FOLLOW_SYMLINK_CHAIN
					FILES \${LIBS}
				)
			")
		endif()
		if(FILESYSTEM_USING_RESOURCE_DAT)
			install(FILES "${FILESYSTEM_USING_RESOURCE_DAT_FILE}" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
		else()
			install(FILES "${PROJECT_NAME}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
			install(DIRECTORY "config/mission" "config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/config")
			install(DIRECTORY "res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
		endif()
		install(FILES "changelog.txt" "heboris.txt" "README.md" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
		if(LINUX)
			install(FILES "pkg/linux/${PROJECT_NAME}.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
			install(FILES "${PROJECT_NAME}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")
		endif()
	else()
		message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported; must be \"Current Directory\", \"Portable\", or \"Installable\"")
	endif()

	if(BUILD_VENDORED)
		set(BUILD_SHARED_LIBS ON)

		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_VENDORED ON)
		set(SDLIMAGE_DEPS_SHARED ON)
		# TODO: Use libpng instead of stb once vendored zlib builds for all platforms
		set(SDLIMAGE_PNG_LIBPNG OFF)
		set(SDLIMAGE_BACKEND_STB ON)
		# TODO: Enable TIFF once vendored zlib builds for all platforms
		set(SDLIMAGE_TIF OFF)
		# TODO: Enable AVIF when it builds for all platforms
		set(SDLIMAGE_AVIF OFF)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		set(SDLMIXER_VENDORED ON)
		set(SDLMIXER_DEPS_SHARED ON)
		# TODO: Use libvorbis instead of stb when libvorbis builds for all platforms
		set(SDLMIXER_VORBIS_STB ON)
		set(SDLMIXER_VORBIS_VORBISFILE OFF)
		# TODO: Enable WavPack when it builds for all platforms
		set(SDLMIXER_WAVPACK OFF)
		add_subdirectory(dep/SDL_mixer EXCLUDE_FROM_ALL)

		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		find_package(SDL3_mixer REQUIRED)
		target_link_libraries(${EXECUTABLE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			SDL3_mixer::SDL3_mixer
		)
	endif()

	if(NOT PACKAGE_TYPE STREQUAL "Current Directory")
		if(WIN32)
			set(DEFAULT_CPACK_GENERATOR "ZIP")
		else()
			set(DEFAULT_CPACK_GENERATOR "TGZ")
		endif()
		set(CPACK_GENERATOR "${DEFAULT_CPACK_GENERATOR}" CACHE STRING "The CPack generator to use")
		set(CPACK_PACKAGE_VENDOR "${PROJECT_CREATOR}")
		if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
			set(PLATFORM "macOS")
		else()
			set(PLATFORM "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
		endif()
		set(CPACK_PACKAGE_FILE_NAME "${PROJECT_NAME}-${PACKAGE_TYPE}-${PLATFORM}")
		include(CPack)
	endif()
else()
	message(FATAL_ERROR "The current build target is not supported")
endif()

set(BUILD_CONFIG_H "src/sys/include/SYS_build_config.h")
set(INPUT_BUILD_CONFIG "${CMAKE_SOURCE_DIR}/${BUILD_CONFIG_H}.cmake")
set(OUTPUT_BUILD_CONFIG "${OUTPUT_DIR}/${BUILD_CONFIG_H}")
add_custom_command(
	DEPENDS "${INPUT_BUILD_CONFIG}"
	OUTPUT "${OUTPUT_BUILD_CONFIG}"
	VERBATIM
	COMMAND "${CMAKE_COMMAND}"
		-DINPUT_FILE=${INPUT_BUILD_CONFIG}
		-DOUTPUT_FILE=${OUTPUT_BUILD_CONFIG}

		-DPROJECT_NAME=${PROJECT_NAME}
		-DPROJECT_VERSION=${PROJECT_VERSION}
		-DPROJECT_TITLE=${PROJECT_TITLE}
		-DPROJECT_CREATOR=${PROJECT_CREATOR}
		-DPROJECT_IDENTIFIER=${PROJECT_IDENTIFIER}
		-DFILESYSTEM_ORG=${FILESYSTEM_ORG}
		-DFILESYSTEM_APP=${FILESYSTEM_APP}
		-DFILESYSTEM_TYPE=${FILESYSTEM_TYPE}
		-DFILESYSTEM_BASE_PATH_APPEND=${FILESYSTEM_BASE_PATH_APPEND}
		-DINPUT_ENABLE_LINUX_GPIO=${INPUT_ENABLE_LINUX_GPIO}
		-DINPUT_ENABLE_KEYBOARD=${INPUT_ENABLE_KEYBOARD}
		-DINPUT_ENABLE_GAME_CONTROLLER=${INPUT_ENABLE_GAME_CONTROLLER}
		-DINPUT_ENABLE_GAME_CONTROLLER_DB=${INPUT_ENABLE_GAME_CONTROLLER_DB}
		-DINPUT_ENABLE_JOYSTICK=${INPUT_ENABLE_JOYSTICK}
		-DINPUT_ONLY_DEVICE_TYPE=${INPUT_ONLY_DEVICE_TYPE}
		-DINPUT_ONLY_GAME_CONTROLLER_TYPE=${INPUT_ONLY_GAME_CONTROLLER_TYPE}
		-DINPUT_ONLY_SDL_CONTROLLER_TYPE=${INPUT_ONLY_SDL_CONTROLLER_TYPE}
		-DINPUT_JOY_KEY_ASSIGN_DEFAULT=${INPUT_JOY_KEY_ASSIGN_DEFAULT}
		-DVIDEO_SCREEN_MODE_BASE=${VIDEO_SCREEN_MODE_BASE}
		-DVIDEO_SCREEN_MODE_DEFAULT=${VIDEO_SCREEN_MODE_DEFAULT}
		-DSYS_PLATFORM=${SYS_PLATFORM}
		-DVIDEO_ENABLE_ALL_SETTINGS=${VIDEO_ENABLE_ALL_SETTINGS}
		-DGAME_ENABLE_QUIT=${GAME_ENABLE_QUIT}
		-DFILESYSTEM_USING_RESOURCE_DAT=${FILESYSTEM_USING_RESOURCE_DAT}
		-DVIDEO_ENABLE_WINDOW_ICON_FILE=${VIDEO_ENABLE_WINDOW_ICON_FILE}

		-P "${CMAKE_SOURCE_DIR}/cmake/ConfigureFile.cmake"
)
target_sources(${EXECUTABLE} PRIVATE "${OUTPUT_BUILD_CONFIG}")
target_include_directories(${EXECUTABLE} PRIVATE
	"src/game/include"
	"src/sys/include"
	"${OUTPUT_DIR}/src/sys/include"
)
