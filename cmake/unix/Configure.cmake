if(${BUILD_TARGET} STREQUAL Darwin)
	# The "Darwin" target is a special case, as it's confusingly not the
	# actual name of the operating system it corresponds to, the
	# NeXTSTEP-derived Apple desktop operating system (Mac OS X, macOS). It
	# seems all other system names do correspond to the correct name,
	# however; such sensibly-named systems are handled below in the else()
	# case.
	#
	# The choice of MACOSX was made to leave open the possibility of a
	# classic Mac OS port in the future, which would use MACOS as the shown
	# system name.
	set(FRAMEWORK_VER "MACOSX SDL2")
else()
	# It's probably a fair assumption that system names are always ASCII,
	# so it's likely no conversion to ASCII is needed here; the framework
	# version is displayed using the game font, not the Unicode font, so it
	# must be ASCII.
	string(TOUPPER "${BUILD_TARGET}" FRAMEWORK_TGT)
	set(FRAMEWORK_VER "${FRAMEWORK_TGT} SDL2")
endif()

# Builds commandline program for some Unix-type platforms. The "Portable" package type supports creating distributable packages with CPack.

list(APPEND EXE_SOURCES "${SRC}/src/main_sdl/physfsrwops.c")
list(APPEND EXE_HEADERS "${SRC}/src/main_sdl/physfsrwops.h")

option(ENABLE_LINUX_GPIO_INPUT "Enable input via GPIO (Linux only)" OFF)
if(${ENABLE_LINUX_GPIO_INPUT})
	if(${BUILD_TARGET} STREQUAL Linux)
		set(ENABLE_LINUX_GPIO TRUE)
		target_link_libraries(${EXE} PRIVATE gpiod)
	else()
		message(FATAL_ERROR "\"ENABLE_LINUX_GPIO_INPUT\" is only supported when \"BUILD_TARGET\" is \"Linux\"")
	endif()
endif()

if(${PACKAGE_TYPE} STREQUAL WorkingDir)
	message(STATUS "Configuring working directory version; CMake installation is not supported")
	add_executable(${EXE} ${EXE_SOURCES} ${EXE_HEADERS})
	configure_file("${SRC}/src/main_sdl/defs.h.in" "${BIN}/src/main_sdl/defs.h" @ONLY)
elseif(${PACKAGE_TYPE} STREQUAL Portable)
	message(STATUS "Configuring portable package")
	add_executable(${EXE} ${EXE_SOURCES} ${EXE_HEADERS})
	set(FILESYSTEM_TYPE FILESYSTEM_PORTABLE)
	configure_file("${SRC}/src/main_sdl/defs.h.in" "${BIN}/src/main_sdl/defs.h" @ONLY)
	install(TARGETS ${EXE} DESTINATION ${EXE})
	install(DIRECTORY "${SRC}/config/mission" "${SRC}/config/stage" DESTINATION "${EXE}/config")
	install(DIRECTORY "${SRC}/res" DESTINATION ${EXE})
	install(FILES "${SRC}/changelog.txt" "${SRC}/heboris.txt" "${SRC}/LICENSE-hashmap.txt" "${SRC}/README.md" DESTINATION ${EXE})
elseif(${PACKAGE_TYPE} STREQUAL Installable)
	message(STATUS "Configuring installable package")
	add_executable(${EXE} ${EXE_SOURCES} ${EXE_HEADERS})
	set(FILESYSTEM_TYPE FILESYSTEM_INSTALLABLE)
	include(GNUInstallDirs REQUIRED)
	set(BASE_PATH_APPEND "../${CMAKE_INSTALL_DATAROOTDIR}/${EXE}")
	configure_file("${SRC}/src/main_sdl/defs.h.in" "${BIN}/src/main_sdl/defs.h" @ONLY)
	install(TARGETS ${EXE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
	install(DIRECTORY "${SRC}/config/mission" "${SRC}/config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${EXE}/config")
	install(DIRECTORY "${SRC}/res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${EXE}")
	install(FILES "${SRC}/README.md" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${EXE}")
	if(${BUILD_TARGET} STREQUAL Linux)
		set(LINUX_DESKTOP_ICON ${EXE} CACHE STRING "The file name of the icon to put into the Linux desktop file, without an extension.")
		set(COMMENT "Cross-platform Expansion and continuation of Heboris U.E.")
		configure_file("${SRC}/pkg/linux/entry.desktop.in" "${BIN}/${EXE}.desktop" @ONLY)
		install(FILES "${BIN}/${EXE}.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
		install(FILES "${SRC}/pkg/linux/icon.png" RENAME "${EXE}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")
	endif()
else()
	message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported; must be \"WorkingDir\", \"Portable\", or \"Installable\"")
endif()

set_target_properties(${EXE}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${BIN}"
)

if(USE_VENDOR_LIBRARIES)
	include(${SRC}/cmake/lib/AddLibrariesVendor.cmake REQUIRED)
	AddLibrariesVendor(${EXE} TRUE)
else()
	include(${SRC}/cmake/lib/AddLibrariesPkgConfig.cmake REQUIRED)
	AddLibrariesPkgConfig(${EXE})
endif()

if(${PACKAGE_TYPE} STREQUAL Portable)
	if(MINGW)
		include("${SRC}/cmake/unix/InstallRuntimeDependenciesMinGW.cmake" REQUIRED)
	elseif(${BUILD_TARGET} STREQUAL Darwin)
		include("${SRC}/cmake/unix/InstallRuntimeDependenciesMacOS.cmake" REQUIRED)
	elseif(${BUILD_TARGET} STREQUAL Linux)
		include("${SRC}/cmake/unix/InstallRuntimeDependenciesLinux.cmake" REQUIRED)
	endif()
endif()
