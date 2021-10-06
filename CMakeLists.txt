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

set(PACKAGE_TYPE "WorkingDir" CACHE STRING "Package type; must be \"WorkingDir\", \"Installable\", or \"Portable\"")

set(EXE HeborisC7EX-SDL2)
set(EXE_SOURCES
	src/game/prototypes.h
	src/game/gamedef.h
	src/game/gamestart.h
	src/game/heboris_ini.h
	src/game/speed.h

	src/game/inireader_heboris_ini.inc
	src/game/heboris.ini
	src/game/heboris_user.ini

	src/game/gamestart.c
	src/game/inireader.c
	src/game/readini.c
	src/game/speed.c

	src/main_sdl/include.h
	src/main_sdl/ygs2kfunc.h
	src/main_sdl/SDL_kanji.h
	src/main_sdl/physfsrwops.h

	src/main_sdl/main.c
	src/main_sdl/ygs2kfunc.c
	src/main_sdl/SDL_kanji.c
	src/main_sdl/physfsrwops.c

	src/script/include.h

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

	src/script/mission_info.c
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

if(CMAKE_C_COMPILER_ID MATCHES "Clang")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-invalid-source-encoding -Wno-logical-op-parentheses")
endif()

if(VCPKG_TOOLCHAIN)
	find_package(SDL2 REQUIRED)
	find_package(sdl2-image REQUIRED)
	find_package(sdl2-mixer REQUIRED)
	find_package(PhysFS REQUIRED)
	add_executable(${EXE} ${EXE_SOURCES})
	target_link_libraries(${EXE}
		PUBLIC
			SDL2::SDL2
			SDL2::SDL2main
			SDL2::SDL2_image
			SDL2::SDL2_mixer
			${PHYSFS_LIBRARY}
	)
	target_include_directories(${EXE} SYSTEM PRIVATE ${PHYSFS_INCLUDE_DIR})
else()
	find_package(PkgConfig)
	if(PKG_CONFIG_FOUND)
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
	
		if(${PACKAGE_TYPE} STREQUAL "WorkingDir")
			add_executable(${EXE} ${EXE_SOURCES})
			set(BASE_PATH "SDL_strdup(\"./\")")
			set(BASE_PATH_APPEND "\"\"")
			set(PREF_PATH "SDL_strdup(\"./\")")
			configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
		# TODO: For macOS packaging, since CMake itself can't do everything: add_custom_command(TARGET ${EXE} POST_BUILD COMMAND ... DEPENDS ... VERBATIM)
		elseif(${PACKAGE_TYPE} STREQUAL "Portable")
			if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
				add_executable(${EXE} MACOSX_BUNDLE "pkg/macos/${EXE}.icns" ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(BASE_PATH_APPEND "\"\"")
				set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
				configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
				set_source_files_properties("pkg/macos/${EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
				set(MACOSX_BUNDLE_BUNDLE_NAME ${EXE})
				set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
				set(MACOSX_BUNDLE_ICON_FILE ${EXE})
				set(SDL_FILESYSTEM_BASE_DIR_TYPE parent)
				set_target_properties(${EXE} PROPERTIES
					BUNDLE True
					MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/pkg/macos/MacOSXBundleInfo.plist.in"
				)
			else()
				add_executable(${EXE} ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(BASE_PATH_APPEND "\"\"")
				set(PREF_PATH "SDL_GetBasePath()")
				configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
			endif()
		elseif(${PACKAGE_TYPE} STREQUAL "Installable")
			if(CMAKE_SYSTEM_NAME STREQUAL Darwin)
				add_executable(${EXE} MACOSX_BUNDLE "pkg/macos/${EXE}.icns" ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(BASE_PATH_APPEND "\"\"")
				set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
				configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
				set_source_files_properties("pkg/macos/${EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
				set(MACOSX_BUNDLE_BUNDLE_NAME ${EXE})
				set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
				set(MACOSX_BUNDLE_ICON_FILE ${EXE})
				set(SDL_FILESYSTEM_BASE_DIR_TYPE resource)
				set_target_properties(${EXE} PROPERTIES
					BUNDLE True
					MACOSX_BUNDLE_INFO_PLIST "${CMAKE_SOURCE_DIR}/pkg/macos/MacOSXBundleInfo.plist.in"
				)
			else()
				add_executable(${EXE} ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
				if(UNIX)
					include(GNUInstallDirs)
					install(TARGETS ${EXE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
					install(DIRECTORY "config/mission" "config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/config")
					install(DIRECTORY "res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
					install(FILES "heboris.ini" DESTINATION "share/${PROJECT_NAME}")
					set(BASE_PATH_APPEND "\"../${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}\"")
					configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
				else()
					set(BASE_PATH_APPEND "\"\"")
					configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
				endif()
			endif()
		else()
			message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported")
		endif()
	
		target_link_libraries(${EXE} PRIVATE
			m
			${CMAKE_DL_LIBS}
		)
		set(DEPENDENCIES
			sdl2
			SDL2_image
			SDL2_mixer
			physfs
		)
		foreach(DEPENDENCY ${DEPENDENCIES})
			target_pkg_config_import(${EXE} ${DEPENDENCY})
		endforeach()
	else()
		message(FATAL_ERROR "pkg-config not found")
	endif()
endif()

target_include_directories(${EXE} PRIVATE src src/game src/main_sdl ${CMAKE_BINARY_DIR}/src ${CMAKE_BINARY_DIR}/src/game ${CMAKE_BINARY_DIR}/src/main_sdl)
