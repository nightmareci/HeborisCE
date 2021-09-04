cmake_minimum_required(VERSION 3.10)
project(HeborisC7EX-SDL2
	LANGUAGES C
)
set(CMAKE_VERBOSE_MAKEFILE ON)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
	add_definitions(-DWIN32)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	add_definitions(-DMACOSX)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	add_definitions(-DLINUX)
endif()

set(EXE heboris)
set(EXE_SOURCES
	src/game/prototypes.h
	src/game/gamedef.h
	src/game/gamestart.h
	src/game/gamestart.c
	src/game/inireader_heboris_ini.inc
	src/game/inireader.c
	src/game/readini.c
	src/game/heboris_ini.h
	src/game/heboris.ini
	src/game/heboris_user.ini

	src/main_sdl/include.h
	src/main_sdl/main.c
	src/main_sdl/ygs2kfunc.h
	src/main_sdl/ygs2kfunc.c
	src/main_sdl/SDL_kanji.h
	src/main_sdl/SDL_kanji.c

	src/script/mission_info.h
	src/script/flexdraw.h
	src/script/config.h
	src/script/world.h
	src/script/classic.h
	src/script/ars.h
	src/script/classic_D.h
	src/script/effect.h
	src/script/sound.h
	src/script/practice.h
	src/script/tomoyo.h
	src/script/ranking.h
	src/script/readdef.h
	src/script/replay.h
	src/script/staffroll.h
	src/script/cpu.h
	src/script/sectime.h
	src/script/mission.h
	src/script/view.h
	src/script/grade.h
	src/script/ranking2.h
	src/script/ranking3.h
)
add_executable(${EXE} ${EXE_SOURCES})

target_include_directories(${EXE} PRIVATE src src/game src/main_sdl)

file(MAKE_DIRECTORY replay config/data)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-source-encoding -Wno-logical-op-parentheses")

find_package(PkgConfig)
function(target_pkg_config_import TARGET DEPENDENCY)
	string(TOUPPER ${DEPENDENCY} DEPENDENCY_VARIABLE)
	pkg_check_modules(${DEPENDENCY_VARIABLE} REQUIRED ${DEPENDENCY})
	target_compile_options(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_CFLAGS}")
	target_compile_options(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_CFLAGS_OTHER}")
	target_include_directories(${TARGET} SYSTEM PRIVATE "${${DEPENDENCY_VARIABLE}_INCLUDE_DIRS}")
	target_link_directories(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_LIBRARY_DIRS}")
	target_link_libraries(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_LIBRARIES}")
	target_link_options(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_LDFLAGS}")
	target_link_options(${TARGET} PRIVATE "${${DEPENDENCY_VARIABLE}_LDFLAGS_OTHER}")
endfunction()
if(PKG_CONFIG_FOUND)
	target_link_libraries(${EXE} PRIVATE
		m
		${CMAKE_DL_LIBS}
	)
	set(DEPENDENCIES
		sdl2
		SDL2_image
		SDL2_mixer
	)
	foreach(DEPENDENCY ${DEPENDENCIES})
		target_pkg_config_import(${EXE} ${DEPENDENCY})
	endforeach()
else()
	find_package(SDL2 REQUIRED)
	find_package(sdl2-image REQUIRED)
	find_package(sdl2-mixer REQUIRED)
	target_link_libraries(${EXE}
		PUBLIC
			SDL2::SDL2
			SDL2::SDL2main
			SDL2::SDL2_image
			SDL2::SDL2_mixer
	)
endif()
