cmake_minimum_required(VERSION 3.16)
project(HeborisC7EX-SDL2
	VERSION 1.0.0
	LANGUAGES C
)
set(HEBORISC7EXSDL2_VERSION "${CMAKE_PROJECT_VERSION}")
add_compile_definitions(HEBORISC7EXSDL2_VERSION="${HEBORISC7EXSDL2_VERSION}")

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)

if(CMAKE_SYSTEM_NAME STREQUAL Windows)
	add_compile_definitions(WIN32)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
	add_compile_definitions(MACOSX)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
	add_compile_definitions(LINUX)
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

if(WIN32 AND MSVC AND VCPKG_TOOLCHAIN)
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

	target_link_options(${EXE}
		PRIVATE
			$<$<NOT:$<CONFIG:Debug>>:
				/ENTRY:mainCRTStartup
				/SUBSYSTEM:WINDOWS
			>
	)

	if(${PACKAGE_TYPE} STREQUAL "WorkingDir")
		message(STATUS "Configuring working directory version; CMake installation is not supported")
		set(BASE_PATH "SDL_strdup(\"./\")")
		set(BASE_PATH_APPEND "\"\"")
		set(PREF_PATH "SDL_strdup(\"./\")")
	else()
		if(${PACKAGE_TYPE} STREQUAL "Portable")
			message(STATUS "Configuring portable package")
			set(BASE_PATH "SDL_GetBasePath()")
			set(BASE_PATH_APPEND "\"\"")
			set(PREF_PATH "SDL_GetBasePath()")
		elseif(${PACKAGE_TYPE} STREQUAL "Installable")
			message(STATUS "Configuring installable package")
			set(BASE_PATH "SDL_GetBasePath()")
			set(BASE_PATH_APPEND "\"\"")
			set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
		else()
			message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported")
		endif()
		install(TARGETS ${EXE} DESTINATION ".")
		install(DIRECTORY "config/mission" "config/stage" DESTINATION "config")
		install(DIRECTORY "res" DESTINATION ".")
		install(FILES "changelog.txt" "heboris.ini" "heboris.txt" "README.md" DESTINATION ".")
		include(pkg/windows/InstallRuntimeDependencies.cmake)
	endif()
	configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
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
			message(STATUS "Configuring working directory version")
			add_executable(${EXE} ${EXE_SOURCES})
			set(BASE_PATH "SDL_strdup(\"./\")")
			set(BASE_PATH_APPEND "\"\"")
			set(PREF_PATH "SDL_strdup(\"./\")")
			configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
		# TODO: For macOS packaging, since CMake itself can't do everything: add_custom_command(TARGET ${EXE} POST_BUILD COMMAND ... DEPENDS ... VERBATIM)
		elseif(${PACKAGE_TYPE} MATCHES "Portable")
			if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin AND ${PACKAGE_TYPE} MATCHES "Mac App")
				message(STATUS "Configuring portable Mac app package")
				set(CMAKE_BUILD_RPATH @executable_path)
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
				message(STATUS "Configuring portable package")
				add_executable(${EXE} ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(BASE_PATH_APPEND "\"\"")
				set(PREF_PATH "SDL_GetBasePath()")
				configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
				install(TARGETS ${EXE} DESTINATION ".")
				install(DIRECTORY "config/mission" "config/stage" DESTINATION "config")
				install(DIRECTORY "res" DESTINATION ".")
				install(FILES "changelog.txt" "heboris.ini" "heboris.txt" "README.md" DESTINATION ".")
			endif()
		elseif(${PACKAGE_TYPE} MATCHES "Installable")
			if(${CMAKE_SYSTEM_NAME} STREQUAL Darwin AND ${PACKAGE_TYPE} MATCHES "Mac App")
				message(STATUS "Configuring installable Mac app package")
				set(CMAKE_BUILD_RPATH @executable_path)
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
				message(STATUS "Configuring installable package")
				add_executable(${EXE} ${EXE_SOURCES})
				set(BASE_PATH "SDL_GetBasePath()")
				set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
				if(UNIX)
					include(GNUInstallDirs)
					set(BASE_PATH_APPEND "\"../${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}\"")
					configure_file("src/main_sdl/paths.h.in" "src/main_sdl/paths.h" @ONLY)
					install(TARGETS ${EXE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
					install(DIRECTORY "config/mission" "config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/config")
					install(DIRECTORY "res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
					install(FILES "heboris.ini" "README.md" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}")
					if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
						set(LINUX_DESKTOP_ICON "HeborisC7EX-SDL2" CACHE STRING "The file name of the icon to put into the Linux desktop file, without an extension.")
						configure_file("pkg/linux/HeborisC7EX-SDL2.desktop.in" "HeborisC7EX-SDL2.desktop" @ONLY)
						install(FILES "${CMAKE_CURRENT_BINARY_DIR}/HeborisC7EX-SDL2.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
						install(FILES "pkg/linux/HeborisC7EX-SDL2.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")
					endif()
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
		)
		foreach(DEPENDENCY ${DEPENDENCIES})
			target_pkg_config_import(${EXE} ${DEPENDENCY})
		endforeach()

		# Some old systems even lack pkg-config support for PhysicsFS,
		# but a manually-installed up-to-date version of CMake (not
		# hard to do) can always provide PhysicsFS this way.
		find_package(PhysFS REQUIRED)
		target_link_libraries(${EXE} PRIVATE ${PHYSFS_LIBRARY})
		target_include_directories(${EXE} SYSTEM PRIVATE ${PHYSFS_INCLUDE_DIR})
	else()
		message(FATAL_ERROR "pkg-config not found")
	endif()
endif()

target_include_directories(${EXE} PRIVATE src src/game src/main_sdl "${CMAKE_CURRENT_BINARY_DIR}/src" "${CMAKE_CURRENT_BINARY_DIR}/src/game" "${CMAKE_CURRENT_BINARY_DIR}/src/main_sdl")
