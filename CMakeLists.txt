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

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(APP_EXE index)
else()
	set(APP_EXE ${CMAKE_PROJECT_NAME})
endif()

if(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(APP_VENDORED TRUE CACHE BOOL "Override" FORCE)
else()
	option(APP_VENDORED "Use vendor libraries (the version of libraries bundled with the code)" OFF)
endif()

# Override these defaults for platforms as required. These defaults work on a
# lot of platforms, but some platforms, like game consoles, have to completely
# override these settings.
option(APP_ENABLE_KEYBOARD_INPUT "Enable input via keyboard" TRUE)
option(APP_ENABLE_GAME_CONTROLLER_INPUT "Enable input via game controllers (not the same as classic joystick input)" TRUE)
option(APP_ENABLE_GAME_CONTROLLER_DB_FILE "Enable loading gamecontrollerdb.txt for supporting more game controllers" TRUE)
option(APP_ENABLE_JOYSTICK_INPUT "Enable input via any joystick device" TRUE)
option(APP_ENABLE_ALL_VIDEO_SETTINGS "Enable all video settings; disabling this option hides some \"unsafe\" settings" TRUE)
option(APP_ENABLE_QUIT "Enables quitting by pressing escape at any time or selecting the \"QUIT\" main menu entry" TRUE)
set(APP_DEFAULT_JOYKEY_ASSIGN "{ { 0 } }")
set(APP_GET_SCREEN_SUBPIXEL_OFFSET "APP_GetScreenSubpixelOffset()")
set(APP_DEFAULT_SCREEN_MODE "(APP_SCREEN_MODE_FULLSCREEN_DESKTOP | APP_SCREEN_MODE_DETAIL_LEVEL | APP_SCREEN_MODE_RENDER_LEVEL)")
set(APP_BASE_PATH_APPEND "")

if(VITA OR CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	set(APP_PACKAGE_TYPE "Installable" CACHE STRING "Package type" FORCE)
elseif(WIN32 OR LINUX)
	set(APP_PACKAGE_TYPE "Current Directory" CACHE STRING "Package type; must be \"Current Directory\", \"Installable\", or \"Portable\"")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	set(APP_PACKAGE_TYPE "Current Directory" CACHE STRING "Package type; must be \"Current Directory\", \"Installable\", \"Portable\", \"Installable Mac App\", or \"Portable Mac App\"")
endif()

if(APP_PACKAGE_TYPE STREQUAL "Current Directory")
	set(APP_FILESYSTEM_TYPE "APP_FILESYSTEM_CURRENT_DIRECTORY")
elseif(APP_PACKAGE_TYPE MATCHES "Installable")
	set(APP_FILESYSTEM_TYPE "APP_FILESYSTEM_INSTALLABLE")
elseif(APP_PACKAGE_TYPE MATCHES "Portable")
	set(APP_FILESYSTEM_TYPE "APP_FILESYSTEM_PORTABLE")
endif()

if(MSVC)
	if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
		# Options only for real MSVC, not immitators like ClangCL
		add_compile_options("/MP")
	endif()

	# Options for all MSVC-like compilers
	add_compile_options("/utf-8")
endif()

add_executable(${APP_EXE}
	"src/app/APP_audio.c"
	"src/app/APP_bdf.c"
	"src/app/APP_filesystem.c"
	"src/app/APP_global.c"
	"src/app/APP_input.c"
	"src/app/APP_main.c"
	"src/app/APP_mp3.c"
	"src/app/APP_ogg.c"
	"src/app/APP_string.c"

	"src/app/APP_video.c"
	"src/app/APP_wav.c"

	"src/app/hashmap.c"

	"src/game/gamestart.c"
	"src/game/init.c"
	"src/game/speed.c"
	"src/game/mission_info.c"
	"src/game/flexdraw.c"
	"src/game/config.c"
	"src/game/world.c"
	"src/game/classic.c"
	"src/game/ars.c"
	"src/game/classic_D.c"
	"src/game/effect.c"
	"src/game/sound.c"
	"src/game/practice.c"
	"src/game/tomoyo.c"
	"src/game/ranking.c"
	"src/game/setdef.c"
	"src/game/replay.c"
	"src/game/staffroll.c"
	"src/game/cpu.c"
	"src/game/sectime.c"
	"src/game/mission.c"
	"src/game/view.c"
	"src/game/grade.c"
	"src/game/ranking2.c"
	"src/game/ranking3.c"

	"src/app/APP.h"
	"src/app/APP_audio.h"
	"src/app/APP_audio_private.h"
	"src/app/APP_bdf.h"
	"src/app/APP_stdinc.h"
	"src/app/APP_error.h"
	"src/app/APP_filesystem.h"
	"src/app/APP_global.h"
	"src/app/APP_input.h"
	"src/app/APP_main.h"
	"src/app/APP_mp3.c"
	"src/app/APP_ogg.h"
	"src/app/APP_stdinc.h"
	"src/app/APP_string.h"

	"src/app/APP_video.h"
	"src/app/APP_wav.h"

	"src/app/dr_mp3.h"
	"src/app/dr_wav.h"
	"src/app/hashmap.h"
	"src/app/stb_vorbis.h"

	"src/game/gamestart.h"
	"src/game/init.h"
	"src/game/speed.h"
	"src/game/ars.h"
	"src/game/classic_D.h"
	"src/game/classic.h"
	"src/game/config.h"
	"src/game/cpu.h"
	"src/game/effect.h"
	"src/game/flexdraw.h"
	"src/game/grade.h"
	"src/game/common.h"
	"src/game/mission.h"
	"src/game/mission_info.h"
	"src/game/practice.h"
	"src/game/ranking2.h"
	"src/game/ranking3.h"
	"src/game/ranking.h"
	"src/game/replay.h"
	"src/game/sectime.h"
	"src/game/setdef.h"
	"src/game/sound.h"
	"src/game/staffroll.h"
	"src/game/tomoyo.h"
	"src/game/view.h"
	"src/game/world.h"
	"src/game/plane.h"
)

set_target_properties(${APP_EXE}
	PROPERTIES
		C_STANDARD 99
		C_STANDARD_REQUIRED TRUE
)

if(LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR MINGW)
	add_compile_definitions(_POSIX_VERSION=200112L)
endif()

if(VITA)
	set(APP_FRAMEWORK_TYPE "VITA SDL3")

	if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VITASDK)
		message(FATAL_ERROR "Vita SDK CMake toolchain file must be provided to the CMake configure command.")
	else()
		include("${VITASDK}/share/vita.cmake" REQUIRED)
	endif()

	target_sources(${APP_EXE} PRIVATE "src/app/APP_vita.c")

	# Adding to CMAKE_PREFIX_PATH like this fixes find_package commands in Vita SDK
	# builds.
	list(APPEND CMAKE_PREFIX_PATH "${VITASDK}/arm-vita-eabi/lib/cmake")

	# Vita SDK will fail linking the executable if position independent code is
	# used in the vendor libraries.
	set(CMAKE_POSITION_INDEPENDENT_CODE FALSE CACHE BOOL "Override" FORCE)

	set(APP_ENABLE_KEYBOARD_INPUT FALSE CACHE BOOL "" FORCE)
	set(APP_ENABLE_JOYSTICK_INPUT FALSE CACHE BOOL "" FORCE)
	set(APP_ONLY_INPUT_TYPE APP_INPUT_PLAYSTATION CACHE STRING "" FORCE)
	set(APP_ONLY_SDL_CONTROLLER_TYPE SDL_GAMEPAD_TYPE_PS4 CACHE STRING "" FORCE)
	set(APP_ENABLE_GAME_CONTROLLER_DB_FILE FALSE CACHE BOOL "" FORCE)
	set(APP_ENABLE_ALL_VIDEO_SETTINGS FALSE CACHE BOOL "" FORCE)
	set(APP_DEFAULT_SCREEN_MODE "(APP_SCREEN_MODE_FULLSCREEN | APP_SCREEN_MODE_DETAIL_LEVEL | APP_SCREEN_MODE_RENDER_LEVEL | APP_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)

	set(VITA_APP_NAME ${APP_EXE})
	set(VITA_TITLEID "NMCI00000")
	set(VITA_VERSION "01.00")
	set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")


	if(APP_VENDORED)
		set(BUILD_SHARED_LIBS OFF)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)
		#set(SDLIMAGE_BACKEND_STB OFF) # TODO: Fix vendored zlib in SDL_image
		set(SDLIMAGE_DEPS_SHARED OFF)
		set(SDLIMAGE_VENDORED ON)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)
		find_package(Threads REQUIRED)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			Threads::Threads
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		find_package(Threads REQUIRED)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
			Threads::Threads
		)
	endif()

	vita_create_self(${APP_EXE}.self ${APP_EXE})
	vita_create_vpk(${APP_EXE}.vpk ${VITA_TITLEID} ${APP_EXE}.self
		VERSION ${VITA_VERSION}
		NAME "${VITA_APP_NAME}"

		# Game assets.
		FILE "res" "res"
		FILE "config/mission" "config/mission"
		FILE "config/stage" "config/stage"

		# Files for LiveArea and boot.
		FILE "pkg/vita/sce_sys/icon0.png" sce_sys/icon0.png
		FILE "pkg/vita/sce_sys/pic0.png" sce_sys/pic0.png
		FILE "pkg/vita/sce_sys/livearea/contents/bg0.png" sce_sys/livearea/contents/bg0.png
		FILE "pkg/vita/sce_sys/livearea/contents/template.xml" sce_sys/livearea/contents/template.xml
	)
elseif(CMAKE_SYSTEM_NAME MATCHES "Emscripten")
	# TODO: Add option to not include --emrun in linker flags for actual
	# releases, so they're suitable for hosting on a real web server.
	# TODO: Add "favicon.ico".

	if(APP_VENDORED)
		set(BUILD_SHARED_LIBS OFF)

		set(SDL_PTHREADS ON CACHE BOOL "" FORCE)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)

		set(SDLIMAGE_AVIF OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_BMP OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_GIF OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_JPG OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_JXL OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_LBM OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_PCX OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_PNM OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_QOI OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_SVG OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_TGA OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_TIF OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_WEBP OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_XCF OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_XPM OFF CACHE BOOL "" FORCE)
		set(SDLIMAGE_XV OFF CACHE BOOL "" FORCE)

		set(SDLIMAGE_PNG ON CACHE BOOL "" FORCE)

		set(SDLIMAGE_DEPS_SHARED OFF)
		set(SDLIMAGE_VENDORED ON)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)

		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
		)

		# TODO: Support non-vendored, when possible. emsdk doesn't provide SDL3_image yet, so only vendored works
		# However, if the emsdk SDL3 package doesn't have SDL_PTHREADS, then vendored will always be required
	endif()

	set(APP_FRAMEWORK_TYPE "WEB SDL3")

	set(APP_ENABLE_JOYSTICK_INPUT FALSE CACHE BOOL "" FORCE)
	set(APP_ENABLE_GAME_CONTROLLER_INPUT TRUE CACHE BOOL "" FORCE)
	set(APP_ENABLE_GAME_CONTROLLER_DB_FILE FALSE CACHE BOOL "" FORCE)
	set(APP_ENABLE_ALL_VIDEO_SETTINGS FALSE CACHE BOOL "" FORCE)
	set(APP_DEFAULT_SCREEN_MODE "(APP_SCREEN_MODE_WINDOW | APP_SCREEN_MODE_DETAIL_LEVEL | APP_SCREEN_MODE_RENDER_LEVEL | APP_SCREEN_MODE_VSYNC)" CACHE STRING "" FORCE)
	set(APP_ENABLE_QUIT FALSE CACHE BOOL "" FORCE)
	set(APP_BASE_PATH_APPEND "basepath/")

	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")
	if(CMAKE_BUILD_TYPE MATCHES "Release")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O3")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --shell-file '${CMAKE_SOURCE_DIR}/pkg/emscripten/index.html'")
	elseif(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s ASSERTIONS")
	endif()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --emrun -pthread -lidbfs.js -s PTHREAD_POOL_SIZE=30 -s ALLOW_MEMORY_GROWTH=1 -s FORCE_FILESYSTEM=1 \
		--preload-file '${CMAKE_SOURCE_DIR}/res/bg/highDetail@basepath/res/bg/highDetail' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/bg/lowDetail@basepath/res/bg/lowDetail' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/bgm@basepath/res/bgm' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/font@basepath/res/font' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/graphics/highDetail@basepath/res/graphics/highDetail' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/graphics/lowDetail@basepath/res/graphics/lowDetail' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/graphics/title@basepath/res/graphics/title' \
		--preload-file '${CMAKE_SOURCE_DIR}/res/se@basepath/res/se' \
		--preload-file '${CMAKE_SOURCE_DIR}/config/mission@basepath/config/mission' \
		--preload-file '${CMAKE_SOURCE_DIR}/config/stage@basepath/config/stage' \
	")

	set_property(TARGET ${APP_EXE} PROPERTY SUFFIX ".html")
elseif(WIN32 AND MSVC)
	set(APP_FRAMEWORK_TYPE "WINDOWS SDL3")

	if(CMAKE_BUILD_TYPE MATCHES "Debug")
		set(WIN32_EXECUTABLE_DEFAULT OFF)
	else()
		set(WIN32_EXECUTABLE_DEFAULT ON)
	endif()
	option(CMAKE_WIN32_EXECUTABLE "Hide the Windows console." ${WIN32_EXECUTABLE_DEFAULT})

	target_sources(${APP_EXE} PRIVATE "pkg/windows/icon.rc")

	if(APP_VENDORED)
		set(BUILD_SHARED_LIBS OFF)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)
		#set(SDLIMAGE_BACKEND_STB OFF) # TODO: Fix vendored zlib in SDL_image
		set(SDLIMAGE_DEPS_SHARED OFF)
		set(SDLIMAGE_VENDORED ON)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
		)
	endif()

	if(APP_PACKAGE_TYPE STREQUAL "Current Directory")
		message(STATUS "Configuring current directory version; CMake installation is not supported")
	elseif(
		APP_PACKAGE_TYPE STREQUAL "Portable" OR
		APP_PACKAGE_TYPE STREQUAL "Installable"
	)
		if(APP_PACKAGE_TYPE STREQUAL "Portable")
			message(STATUS "Configuring portable package")
		elseif(APP_PACKAGE_TYPE STREQUAL "Installable")
			message(STATUS "Configuring installable package")
		else()
			message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported; must be \"Current Directory\", \"Portable\", or \"Installable\"")
		endif()

		install(TARGETS ${APP_EXE} DESTINATION ".")
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "config")
		install(DIRECTORY "res" DESTINATION ".")
		install(FILES "changelog.txt" "heboris.txt" "LICENSE-hashmap.txt" "README.md" DESTINATION ".")
	else()
		message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported when building for Windows with MSVC")
	endif()
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin" AND APP_PACKAGE_TYPE MATCHES "Mac App")
	set(APP_FRAMEWORK_TYPE "MACOSX SDL3")

	# Builds Mac application bundle, packaged into a DMG with CPack by default.

	target_sources(${APP_EXE} PRIVATE
		"src/app/physfsrwops.c"
		"src/app/physfsrwops.h"
		"pkg/macos/${APP_EXE}.icns"
	)

	set_property(TARGET ${APP_EXE} PROPERTY MACOSX_BUNDLE TRUE)
	set_source_files_properties("pkg/macos/${APP_EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
	set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_EXE})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER "${APP_PROJECT_NAME}")
	set(MACOSX_BUNDLE_ICON_FILE ${APP_EXE})
	if(APP_PACKAGE_TYPE MATCHES "Portable")
		set(SDL_FILESYSTEM_BASE_DIR_TYPE "parent")
		set(APP_INSTALL_DIR ${APP_EXE})
		install(TARGETS ${APP_EXE} BUNDLE DESTINATION "${APP_EXE}")
		install(FILES "changelog.txt" "heboris.txt" "LICENSE-hashmap.txt" "README.md" DESTINATION "${APP_EXE}")
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "${APP_EXE}/config")
		install(DIRECTORY "res" DESTINATION "${APP_EXE}")
	elseif(APP_PACKAGE_TYPE MATCHES "Installable")
		set(SDL_FILESYSTEM_BASE_DIR_TYPE "resource")
		set(APP_INSTALL_DIR .)
		install(TARGETS ${APP_EXE} BUNDLE DESTINATION .)
		install(FILES "changelog.txt" "heboris.txt" "LICENSE-hashmap.txt" "README.md" DESTINATION .)
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "${APP_EXE}.app/Contents/Resources/config")
		install(DIRECTORY "res" DESTINATION "${APP_EXE}.app/Contents/Resources")
	else()
		message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported when building a macOS app")
	endif()

	set_target_properties(${APP_EXE} PROPERTIES
		MACOSX_BUNDLE_INFO_PLIST "cmake/MacOSXBundleInfo.plist.cmake"
		INSTALL_RPATH "@executable_path"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)

	if(APP_VENDORED)
		include("cmake/AddLibrariesVendor.cmake" REQUIRED)
		AddLibrariesVendor(${APP_EXE} TRUE)
	else()
		include("cmake/AddLibrariesPkgConfig.cmake" REQUIRED)
		AddLibrariesPkgConfig(${APP_EXE})
	endif()

	set(APP_APPLE_CERT_NAME - CACHE STRING "The name of the Apple supplied code signing certificate to use. Defaults to adhoc signing (\"-\")")
	include(CMakeFindBinUtils)
	install(CODE "
		set(APP_POST_EXCLUDES
		\"^/usr/lib\"
		\"^/System/Library/Frameworks\"
		)

		file(GET_RUNTIME_DEPENDENCIES
			EXECUTABLES \$<TARGET_FILE:${APP_EXE}>
			POST_EXCLUDE_REGEXES
				\${APP_POST_EXCLUDES}
			RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPENDENCIES
			UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPENDENCIES
		)

		file(INSTALL
			DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs\"
			TYPE SHARED_LIBRARY
			FOLLOW_SYMLINK_CHAIN
			FILES \${RESOLVED_DEPENDENCIES}
		)

		set(APP_CHANGES \"\")
		set(APP_LIBS \"\")
		foreach(RESOLVED_DEPENDENCY \${RESOLVED_DEPENDENCIES})
			string(REGEX REPLACE \"^.*/([^/]+)\" \"\\\\1\" LIB \"\${RESOLVED_DEPENDENCY}\")
			list(APPEND APP_CHANGES -change \"\${RESOLVED_DEPENDENCY}\" \"@rpath/../libs/\${LIB}\")
			list(APPEND APP_LIBS \"\${LIB}\")
		endforeach()

		if(\"${APP_APPLE_CERT_NAME}\" STREQUAL -)
			set(APP_ENTITLEMENTS adhoc)
		else()
			set(APP_ENTITLEMENTS identity-required)
		endif()

		foreach(APP_LIB \${APP_LIBS})
			execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${APP_CHANGES} -id \"@rpath/../libs/\${APP_LIB}\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
			execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
			execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APP_APPLE_CERT_NAME}\" --entitlements \"${CMAKE_SOURCE_DIR}/pkg/macos/entitlements-\${APP_ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
		endforeach()
		execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${APP_CHANGES} \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/MacOS/${APP_EXE}\")
		execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app\")
		execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APP_APPLE_CERT_NAME}\" --entitlements \"${CMAKE_SOURCE_DIR}/pkg/macos/entitlements-\${APP_ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app\")
	")

	set(CPACK_DMG_VOLUME_NAME ${APP_EXE})
	set(CPACK_PACKAGE_VENDOR "${APP_PROJECT_CREATOR}")
	if(APP_PACKAGE_TYPE MATCHES "Portable")
		set(CPACK_PACKAGE_FILE_NAME "${APP_EXE}-Portable-App-macOS")
	elseif(${APP_PACKAGE_TYPE} MATCHES "Installable")
		set(CPACK_PACKAGE_FILE_NAME "${APP_EXE}-Installable-App-macOS")
	endif()
	set(CPACK_GENERATOR "DragNDrop")
	include(CPack)
elseif(LINUX OR CMAKE_SYSTEM_NAME STREQUAL "Darwin" OR MINGW OR UNIX)
	if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
		set(APP_FRAMEWORK_TYPE "MACOSX SDL3")
	else()
		# It's probably a fair assumption that system names are always ASCII,
		# so it's likely no conversion to ASCII is needed here; the framework
		# version is displayed using the game font, not the Unicode font, so it
		# must be ASCII.
		string(TOUPPER "${CMAKE_SYSTEM_NAME}" APP_FRAMEWORK_TARGET)
		set(APP_FRAMEWORK_TYPE "${APP_FRAMEWORK_TARGET} SDL3")
	endif()

	# Builds commandline program for some Unix-type platforms. The "Portable" package type supports creating distributable packages with CPack.

	if(LINUX)
		option(APP_ENABLE_LINUX_GPIO_INPUT "Enable input via Linux GPIO" OFF)
	else()
		set(APP_ENABLE_LINUX_GPIO_INPUT OFF CACHE BOOL "Enable input via Linux GPIO" FORCE)
	endif()

	if(APP_PACKAGE_TYPE STREQUAL "Current Directory")
	elseif(APP_PACKAGE_TYPE STREQUAL "Portable")
		install(TARGETS ${APP_EXE} DESTINATION ${APP_EXE})
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "${APP_EXE}/config")
		install(DIRECTORY "res" DESTINATION ${APP_EXE})
		install(FILES "changelog.txt" "heboris.txt" "LICENSE-hashmap.txt" "README.md" DESTINATION ${APP_EXE})
	elseif(APP_PACKAGE_TYPE STREQUAL "Installable")
		message(STATUS "Configuring installable package")
		include(GNUInstallDirs REQUIRED)
		set(APP_BASE_PATH_APPEND "../${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}/")
		install(TARGETS ${APP_EXE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}/config")
		install(DIRECTORY "res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}")
		install(FILES "README.md" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}")
		if(LINUX)
			set(APP_LINUX_DESKTOP_ICON ${APP_EXE} CACHE STRING "The file name of the icon to put into the Linux desktop file, without an extension")
			set(APP_COMMENT "Cross-platform Expansion and continuation of Heboris U.E.")
			configure_file("cmake/entry.desktop.cmake" "${CMAKE_BINARY_DIR}/${APP_EXE}.desktop" @ONLY)
			install(FILES "${CMAKE_BINARY_DIR}/${APP_EXE}.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
			install(FILES "pkg/linux/icon.png" RENAME "${APP_EXE}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")
		endif()
	else()
		message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported; must be \"Current Directory\", \"Portable\", or \"Installable\"")
	endif()

	set_target_properties(${APP_EXE}
		PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
	)

	if(APP_VENDORED)
		set(BUILD_SHARED_LIBS OFF)
		add_subdirectory(dep/SDL EXCLUDE_FROM_ALL)
		#set(SDLIMAGE_BACKEND_STB OFF) # TODO: Fix vendored zlib in SDL_image
		set(SDLIMAGE_DEPS_SHARED OFF)
		set(SDLIMAGE_VENDORED ON)
		add_subdirectory(dep/SDL_image EXCLUDE_FROM_ALL)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
		)
	else()
		find_package(SDL3 REQUIRED)
		find_package(SDL3_image REQUIRED)
		target_link_libraries(${APP_EXE} PRIVATE
			SDL3::SDL3
			SDL3_image::SDL3_image
		)
	endif()
else()
	message(FATAL_ERROR "The current build target is not supported")
endif()

configure_file("src/app/APP_build_config.h.cmake" "src/app/APP_build_config.h" @ONLY)
target_sources(${APP_EXE} PRIVATE "${CMAKE_BINARY_DIR}/src/app/APP_build_config.h")
target_include_directories(${APP_EXE} PRIVATE "src" "${CMAKE_BINARY_DIR}/src" "${CMAKE_BINARY_DIR}/src/app")
