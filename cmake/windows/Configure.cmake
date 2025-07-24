set(APP_FRAMEWORK_TYPE "WINDOWS SDL2")

add_compile_options("/MP")
add_compile_options("/utf-8")

list(APPEND APP_SOURCES "${APP_SRC_DIR}/src/app/physfsrwops.c")
list(APPEND APP_HEADERS "${APP_SRC_DIR}/src/app/physfsrwops.h")

option(CMAKE_WIN32_EXECUTABLE "Hide the Windows console." OFF)

add_executable(${APP_EXE}
	${APP_SOURCES} ${APP_HEADERS}
	"${APP_SRC_DIR}/pkg/windows/icon.rc"
)

if(APP_VENDORED)
	include("${APP_SRC_DIR}/cmake/lib/AddLibrariesVendor.cmake" REQUIRED)
	AddLibrariesVendor(${APP_EXE} TRUE)
else()
	include("${APP_SRC_DIR}/cmake/lib/AddLibrariesFindPackage.cmake" REQUIRED)
	AddLibrariesFindPackage(${APP_EXE})
endif()

if(${APP_PACKAGE_TYPE} STREQUAL "WorkingDir")
	message(STATUS "Configuring working directory version; CMake installation is not supported")
else()
	if(${APP_PACKAGE_TYPE} STREQUAL "Portable")
		message(STATUS "Configuring portable package")
		set(FILESYSTEM_TYPE FILESYSTEM_PORTABLE)
	elseif(${APP_PACKAGE_TYPE} STREQUAL "Installable")
		message(STATUS "Configuring installable package")
		set(FILESYSTEM_TYPE FILESYSTEM_INSTALLABLE)
	else()
		message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported; must be \"WorkingDir\", \"Portable\", or \"Installable\"")
	endif()

	install(TARGETS ${APP_EXE} DESTINATION ".")
	install(DIRECTORY "${APP_SRC_DIR}/config/mission" "${APP_SRC_DIR}/config/stage" DESTINATION "config")
	install(DIRECTORY "${APP_SRC_DIR}/res" DESTINATION ".")
	install(FILES "${APP_SRC_DIR}/changelog.txt" "${APP_SRC_DIR}/heboris.txt" "${APP_SRC_DIR}/LICENSE-hashmap.txt" "${APP_SRC_DIR}/README.md" DESTINATION ".")
	include("${APP_SRC_DIR}/cmake/windows/InstallRuntimeDependenciesMSVC.cmake" REQUIRED)
endif()
configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
