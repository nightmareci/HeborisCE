set(FRAMEWORK_VER "WINDOWS-SDL2")

find_package(SDL2 REQUIRED)
find_package(sdl2-image REQUIRED)
find_package(sdl2-mixer REQUIRED)
find_package(PhysFS REQUIRED)

add_executable(${EXE}
	${EXE_SOURCES}
	"${SRC}/pkg/windows/icon.rc"
)
target_link_libraries(${EXE}
	PUBLIC
		SDL2::SDL2
		SDL2::SDL2main
		SDL2::SDL2_image
		SDL2::SDL2_mixer
		${PHYSFS_LIBRARY}
)
target_include_directories(${EXE} SYSTEM PRIVATE
	"${PHYSFS_INCLUDE_DIR}"
)

option(HIDE_WINDOWS_CONSOLE "Hide the Windows console.")
target_link_options(${EXE}
	PRIVATE
		$<$<BOOL:${HIDE_WINDOWS_CONSOLE}>:
			/ENTRY:mainCRTStartup
			/SUBSYSTEM:WINDOWS
		>
)

if(${PACKAGE_TYPE} STREQUAL "WorkingDir")
	message(STATUS "Configuring working directory version; CMake installation is not supported")
else()
	if(${PACKAGE_TYPE} STREQUAL "Portable")
		message(STATUS "Configuring portable package")
		set(FILESYSTEM_TYPE FILESYSTEM_PORTABLE)
	elseif(${PACKAGE_TYPE} STREQUAL "Installable")
		message(STATUS "Configuring installable package")
		set(FILESYSTEM_TYPE FILESYSTEM_INSTALLABLE)
	else()
		message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported; must be \"WorkingDir\", \"Portable\", or \"Installable\"")
	endif()

	install(TARGETS ${EXE} DESTINATION ".")
	install(DIRECTORY "${SRC}/config/mission" "${SRC}/config/stage" DESTINATION "config")
	install(DIRECTORY "${SRC}/res" DESTINATION ".")
	install(FILES "${SRC}/changelog.txt" "${SRC}/heboris.txt" "${SRC}/README.md" DESTINATION ".")
	include("${SRC}/cmake/windows/InstallRuntimeDependenciesMSVC.cmake" REQUIRED)
endif()
configure_file("${SRC}/src/main_sdl/defs.h.in" "src/main_sdl/defs.h" @ONLY)
