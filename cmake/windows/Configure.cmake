set(FRAMEWORK_VER "WINDOWS-SDL2")

add_compile_options("/MP")
add_compile_options("/utf-8")

list(APPEND EXE_SOURCES "${SRC}/src/main_sdl/physfsrwops.c")
list(APPEND EXE_HEADERS "${SRC}/src/main_sdl/physfsrwops.h")

if(ENABLE_GAME_CONTROLLER)
	list(APPEND EXE_SOURCES "${SRC}/src/main_sdl/gamecontrollerdb.c")
endif()

add_executable(${EXE}
	${EXE_SOURCES} ${EXE_HEADERS}
	"${SRC}/pkg/windows/icon.rc"
)

if(USE_VENDOR_LIBRARIES)
	include("${SRC}/cmake/lib/AddLibrariesVendor.cmake" REQUIRED)
	AddLibrariesVendor(${EXE} TRUE)
else()
	include("${SRC}/cmake/lib/AddLibrariesFindPackage.cmake" REQUIRED)
	AddLibrariesFindPackage(${EXE})
endif()

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
