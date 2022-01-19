cmake_minimum_required(VERSION 3.16)
project(HeborisC7EX-SDL2
	VERSION 1.0.1
	LANGUAGES C
)

set(BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita with VitaSDK.")

set(HEBORISC7EXSDL2_VERSION "${CMAKE_PROJECT_VERSION}")
add_compile_definitions(HEBORISC7EXSDL2_VERSION="${HEBORISC7EXSDL2_VERSION}")

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

set(SRC ${CMAKE_CURRENT_SOURCE_DIR})
set(BIN ${CMAKE_CURRENT_BINARY_DIR})

set(EXE HeborisC7EX-SDL2)
set(EXE_SOURCES
	${SRC}/src/game/prototypes.h
	${SRC}/src/game/gamedef.h
	${SRC}/src/game/gamestart.h
	${SRC}/src/game/init.h
	${SRC}/src/game/speed.h

	${SRC}/src/game/init.inc

	${SRC}/src/game/gamestart.c
	${SRC}/src/game/speed.c

	${SRC}/src/main_sdl/include.h
	${SRC}/src/main_sdl/ygs2kfunc.h
	${SRC}/src/main_sdl/SDL_kanji.h
	${SRC}/src/main_sdl/physfsrwops.h

	${SRC}/src/main_sdl/main.c
	${SRC}/src/main_sdl/ygs2kfunc.c
	${SRC}/src/main_sdl/SDL_kanji.c
	${SRC}/src/main_sdl/physfsrwops.c

	${SRC}/src/script/include.h

	${SRC}/src/script/mission_info.h
	${SRC}/src/script/flexdraw.h
	${SRC}/src/script/config.h
	${SRC}/src/script/world.h
	${SRC}/src/script/classic.h
	${SRC}/src/script/ars.h
	${SRC}/src/script/classic_D.h
	${SRC}/src/script/effect.h
	${SRC}/src/script/sound.h
	${SRC}/src/script/practice.h
	${SRC}/src/script/tomoyo.h
	${SRC}/src/script/ranking.h
	${SRC}/src/script/setdef.h
	${SRC}/src/script/replay.h
	${SRC}/src/script/staffroll.h
	${SRC}/src/script/cpu.h
	${SRC}/src/script/sectime.h
	${SRC}/src/script/mission.h
	${SRC}/src/script/view.h
	${SRC}/src/script/grade.h
	${SRC}/src/script/ranking2.h
	${SRC}/src/script/ranking3.h

	${SRC}/src/script/mission_info.c
	${SRC}/src/script/flexdraw.c
	${SRC}/src/script/config.c
	${SRC}/src/script/world.c
	${SRC}/src/script/classic.c
	${SRC}/src/script/ars.c
	${SRC}/src/script/classic_D.c
	${SRC}/src/script/effect.c
	${SRC}/src/script/sound.c
	${SRC}/src/script/practice.c
	${SRC}/src/script/tomoyo.c
	${SRC}/src/script/ranking.c
	${SRC}/src/script/setdef.c
	${SRC}/src/script/replay.c
	${SRC}/src/script/staffroll.c
	${SRC}/src/script/cpu.c
	${SRC}/src/script/sectime.c
	${SRC}/src/script/mission.c
	${SRC}/src/script/view.c
	${SRC}/src/script/grade.c
	${SRC}/src/script/ranking2.c
	${SRC}/src/script/ranking3.c
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-source-encoding -Wno-logical-op-parentheses")
endif()

set(BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita.")
message(STATUS "Build target: ${BUILD_TARGET}")

if(${BUILD_TARGET} STREQUAL Vita)
	add_compile_definitions(VITA)
	include(${SRC}/cmake/vita/Configure.cmake)
elseif(${BUILD_TARGET} STREQUAL Windows)
	add_compile_definitions(WIN32)
	set(PACKAGE_TYPE "WorkingDir" CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	if(MINGW)
		include(${SRC}/cmake/unix/Configure.cmake)
	elseif(WIN32 AND MSVC AND VCPKG_TOOLCHAIN)
		include(${SRC}/cmake/windows/Configure.cmake)
	else()
		message(FATAL_ERROR "Current Windows build setup is unsupported; MinGW with pkg-config or MSVC with VCPKG are supported.")
	endif()
elseif(${BUILD_TARGET} STREQUAL Darwin)
	add_compile_definitions(MACOSX)
	set(PACKAGE_TYPE "WorkingDir" CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", \"Portable\", \"Installable Mac App\", or \"Portable Mac App\"")
	include(${SRC}/cmake/unix/Configure.cmake)
elseif(${BUILD_TARGET} STREQUAL Linux)
	add_compile_definitions(LINUX)
	set(PACKAGE_TYPE "WorkingDir" CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")
	include(${SRC}/cmake/unix/Configure.cmake)
else()
	message(FATAL_ERROR "Requested build target \"${BUILD_TARGET}\" is unsupported.")
endif()

target_include_directories(${EXE} PRIVATE ${SRC}/src ${SRC}/src/game ${SRC}/src/main_sdl "${BIN}/src" "${BIN}/src/game" "${BIN}/src/main_sdl")

# TODO: Finish this for installation of DLLs of MinGW builds.
#install(CODE "
#message(STATUS \"Target file: $<TARGET_FILE:${EXE}>\")
#execute_process(COMMAND sh -c \"ldd $<TARGET_FILE:${EXE}> | sed -ne 's#^[ \\t][ \\t]*[^\\\\/][^\\\\/]*\\\\.[dD][lL][lL] => \\\\(..*\\\\.[dD][lL][lL]\\\\) (0x[0-9a-f][0-9a-f]*)$#\\\\1#p'\" OUTPUT_VARIABLE LIBRARIES_LINES)
#string(REPLACE \"\\n\" \";\" DEPENDENCIES \${LIBRARIES_LINES})
#foreach(DEPENDENCY IN LISTS DEPENDENCIES)
#	message(STATUS \${DEPENDENCY})
#endforeach()
#
#set_property(GLOBAL PROPERTY DEPTH 0)
#function(test_recurse)
#	get_property(DEPTH_LAST GLOBAL PROPERTY DEPTH)
#	math(EXPR DEPTH_CURRENT \"\${DEPTH_LAST} + 1\")
#	set_property(GLOBAL PROPERTY DEPTH \${DEPTH_CURRENT})
#	message(STATUS \"Depth at entry: \${DEPTH_CURRENT}\")
#	if(\${DEPTH_CURRENT} EQUAL 3)
#		return()
#	else()
#		test_recurse()
#		get_property(DEPTH_AFTER_RECURSE GLOBAL PROPERTY DEPTH)
#		message(STATUS \"Depth after recurse: \${DEPTH_AFTER_RECURSE}\")
#	endif()
#endfunction()
#
#test_recurse()
#message(STATUS \"Done\")
#")
