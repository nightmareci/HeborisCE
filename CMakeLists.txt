cmake_minimum_required(VERSION 3.10)
project(HeborisC7EX
	LANGUAGES CXX
)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(EXE heboris)
add_executable(${EXE})

target_include_directories(${EXE} PRIVATE src src/game src/main_sdl)
target_sources(${EXE}
	PRIVATE
		src/game/prototypes.h
		src/game/gamedef.h
		src/game/gamestart.h
		src/game/gamestart.cpp
		src/game/inireader_heboris_ini.inc
		src/game/inireader.cpp
		src/game/readini.c
		src/game/heboris.ini
		src/game/heboris_user.ini

		src/main_sdl/include.h
		src/main_sdl/main.cpp
		src/main_sdl/ygs2kfunc.h
		src/main_sdl/ygs2kfunc.cpp
		src/main_sdl/SDL_kanji.h
		src/main_sdl/SDL_kanji.cpp

		src/script/mission_info.def
		src/script/flexdraw.c
		src/script/config.c
		src/script/world.c
		src/script/classic.c
		src/script/ars.c
		src/script/classic_D.c
		src/script/effect.c
		src/script/sound.c
		src/script/practice.c
		src/script/tomoyo.c
		src/script/ranking.c
		src/script/readdef.c
		src/script/replay.c
		src/script/staffroll.c
		src/script/cpu.c
		src/script/sectime.c
		src/script/mission.c
		src/script/view.c
		src/script/grade.c
		src/script/ranking2.c
		src/script/ranking3.c
)

file(MAKE_DIRECTORY replay config/data)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-invalid-source-encoding -Wno-logical-op-parentheses")

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
