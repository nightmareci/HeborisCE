if(${APP_BUILD_TARGET} STREQUAL Darwin)
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
	set(APP_FRAMEWORK_TYPE "MACOSX SDL2")
else()
	# It's probably a fair assumption that system names are always ASCII,
	# so it's likely no conversion to ASCII is needed here; the framework
	# version is displayed using the game font, not the Unicode font, so it
	# must be ASCII.
	string(TOUPPER "${APP_BUILD_TARGET}" APP_FRAMEWORK_TARGET)
	set(APP_FRAMEWORK_TYPE "${APP_FRAMEWORK_TARGET} SDL2")
endif()

# Builds commandline program for some Unix-type platforms. The "Portable" package type supports creating distributable packages with CPack.

list(APPEND APP_SOURCES "${APP_SRC_DIR}/src/app/physfsrwops.c")
list(APPEND APP_HEADERS "${APP_SRC_DIR}/src/app/physfsrwops.h")

option(APP_ENABLE_LINUX_GPIO_INPUT "Enable input via GPIO (Linux only)" OFF)
if(APP_ENABLE_LINUX_GPIO_INPUT)
	if(${APP_BUILD_TARGET} STREQUAL Linux)
		set(APP_ENABLE_LINUX_GPIO TRUE)
		target_link_libraries(${APP_EXE} PRIVATE gpiod)
	else()
		message(FATAL_ERROR "\"APP_ENABLE_LINUX_GPIO_INPUT\" is only supported when \"APP_BUILD_TARGET\" is \"Linux\"")
	endif()
endif()

if(${APP_PACKAGE_TYPE} STREQUAL WorkingDir)
	message(STATUS "Configuring working directory version; CMake installation is not supported")
	add_executable(${APP_EXE} ${APP_SOURCES} ${APP_HEADERS})
	configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
elseif(${APP_PACKAGE_TYPE} STREQUAL Portable)
	message(STATUS "Configuring portable package")
	add_executable(${APP_EXE} ${APP_SOURCES} ${APP_HEADERS})
	set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_PORTABLE)
	configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
	install(TARGETS ${APP_EXE} DESTINATION ${APP_EXE})
	install(DIRECTORY "${APP_SRC_DIR}/config/mission" "${APP_SRC_DIR}/config/stage" DESTINATION "${APP_EXE}/config")
	install(DIRECTORY "${APP_SRC_DIR}/res" DESTINATION ${APP_EXE})
	install(FILES "${APP_SRC_DIR}/changelog.txt" "${APP_SRC_DIR}/heboris.txt" "${APP_SRC_DIR}/LICENSE-hashmap.txt" "${APP_SRC_DIR}/README.md" DESTINATION ${APP_EXE})
elseif(${APP_PACKAGE_TYPE} STREQUAL Installable)
	message(STATUS "Configuring installable package")
	add_executable(${APP_EXE} ${APP_SOURCES} ${APP_HEADERS})
	set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_INSTALLABLE)
	include(GNUInstallDirs REQUIRED)
	set(APP_BASE_PATH_APPEND "../${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}")
	configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
	install(TARGETS ${APP_EXE} DESTINATION "${CMAKE_INSTALL_BINDIR}")
	install(DIRECTORY "${APP_SRC_DIR}/config/mission" "${APP_SRC_DIR}/config/stage" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}/config")
	install(DIRECTORY "${APP_SRC_DIR}/res" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}")
	install(FILES "${APP_SRC_DIR}/README.md" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/${APP_EXE}")
	if(${APP_BUILD_TARGET} STREQUAL Linux)
		set(APP_LINUX_DESKTOP_ICON ${APP_EXE} CACHE STRING "The file name of the icon to put into the Linux desktop file, without an extension.")
		set(APP_COMMENT "Cross-platform Expansion and continuation of Heboris U.E.")
		configure_file("${APP_SRC_DIR}/pkg/linux/entry.desktop.in" "${APP_BIN_DIR}/${APP_EXE}.desktop" @ONLY)
		install(FILES "${APP_BIN_DIR}/${APP_EXE}.desktop" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications")
		install(FILES "${APP_SRC_DIR}/pkg/linux/icon.png" RENAME "${APP_EXE}.png" DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/icons/hicolor/256x256/apps")
	endif()
else()
	message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported; must be \"WorkingDir\", \"Portable\", or \"Installable\"")
endif()

set_target_properties(${APP_EXE}
	PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}"
)

if(APP_VENDORED)
	include(${APP_SRC_DIR}/cmake/lib/AddLibrariesVendor.cmake REQUIRED)
	AddLibrariesVendor(${APP_EXE} TRUE)
else()
	include(${APP_SRC_DIR}/cmake/lib/AddLibrariesPkgConfig.cmake REQUIRED)
	AddLibrariesPkgConfig(${APP_EXE})
endif()

if(${APP_PACKAGE_TYPE} STREQUAL Portable)
	if(MINGW)
		include("${APP_SRC_DIR}/cmake/unix/InstallRuntimeDependenciesMinGW.cmake" REQUIRED)
	elseif(${APP_BUILD_TARGET} STREQUAL Darwin)
		include("${APP_SRC_DIR}/cmake/unix/InstallRuntimeDependenciesMacOS.cmake" REQUIRED)
	elseif(${APP_BUILD_TARGET} STREQUAL Linux)
		include("${APP_SRC_DIR}/cmake/unix/InstallRuntimeDependenciesLinux.cmake" REQUIRED)
	endif()
endif()
