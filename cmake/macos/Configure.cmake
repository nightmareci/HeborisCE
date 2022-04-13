# Builds Mac application bundle, packaged into a DMG with CPack by default.

if(${PACKAGE_TYPE} STREQUAL "Portable Mac App")
	message(STATUS "Configuring portable Mac app package")
	add_executable(${EXE} MACOSX_BUNDLE "${SRC}/pkg/macos/${EXE}.icns" ${EXE_SOURCES})
	set(BASE_PATH "SDL_GetBasePath()")
	set(BASE_PATH_APPEND "\"\"")
	set(PREF_PATH "SDL_GetBasePath()")
	configure_file("${SRC}/src/main_sdl/platform.h.in" "${BIN}/src/main_sdl/platform.h" @ONLY)
	set_source_files_properties("${SRC}/pkg/macos/${EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set(MACOSX_BUNDLE_BUNDLE_NAME ${EXE})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
	set(MACOSX_BUNDLE_ICON_FILE ${EXE})
	set(SDL_FILESYSTEM_BASE_DIR_TYPE parent)
elseif(${PACKAGE_TYPE} STREQUAL "Installable Mac App")
	message(STATUS "Configuring installable Mac app package")
	add_executable(${EXE} MACOSX_BUNDLE "${SRC}/pkg/macos/${EXE}.icns" ${EXE_SOURCES})
	set(BASE_PATH "SDL_GetBasePath()")
	set(BASE_PATH_APPEND "\"\"")
	set(PREF_PATH "SDL_GetPrefPath(\"nightmareci\", \"HeborisC7EX SDL2\")")
	configure_file("${SRC}/src/main_sdl/platform.h.in" "${BIN}/src/main_sdl/platform.h" @ONLY)
	set_source_files_properties("${SRC}/pkg/macos/${EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set(MACOSX_BUNDLE_BUNDLE_NAME ${EXE})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
	set(MACOSX_BUNDLE_ICON_FILE ${EXE})
	set(SDL_FILESYSTEM_BASE_DIR_TYPE resource)
else()
	message(FATAL_ERROR "Package type \"${PACKAGE_TYPE}\" unsupported when building a macOS app")
endif()

set_target_properties(${EXE} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST "${SRC}/pkg/macos/MacOSXBundleInfo.plist.in"
	INSTALL_RPATH @executable_path
	RUNTIME_OUTPUT_DIRECTORY ${BIN}
)

if(${PACKAGE_TYPE} MATCHES Portable)
	set(INSTALL_DIR ${EXE})
	install(TARGETS ${EXE} BUNDLE DESTINATION "${EXE}")
	install(FILES "${SRC}/changelog.txt" "${SRC}/heboris.txt" "${SRC}/README.md" DESTINATION "${EXE}")
	install(DIRECTORY "${SRC}/config/mission" "${SRC}/config/stage" DESTINATION "${EXE}/config")
	install(DIRECTORY "${SRC}/res" DESTINATION "${EXE}")
elseif(${PACKAGE_TYPE} MATCHES Installable)
	set(INSTALL_DIR .)
	install(TARGETS ${EXE} BUNDLE DESTINATION .)
	install(FILES "${SRC}/changelog.txt" "${SRC}/heboris.txt" "${SRC}/README.md" DESTINATION .)
	install(DIRECTORY "${SRC}/config/mission" "${SRC}/config/stage" DESTINATION "${EXE}.app/Contents/Resources/config")
	install(DIRECTORY "${SRC}/res" DESTINATION "${EXE}.app/Contents/Resources")
endif()

include(${SRC}/cmake/unix/AddLibrariesPkgConfig.cmake REQUIRED)
AddLibrariesPkgConfig(${EXE})

set(APPLE_CERT_NAME - CACHE STRING "The name of the Apple supplied code signing certificate to use. Defaults to adhoc signing (\"-\")")
include(CMakeFindBinUtils)
install(CODE "

set(POST_EXCLUDES
\"^/usr/lib\"
\"^/System/Library/Frameworks\"
)

file(GET_RUNTIME_DEPENDENCIES
	EXECUTABLES \$<TARGET_FILE:${EXE}>
	POST_EXCLUDE_REGEXES
		\${POST_EXCLUDES}
	RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPENDENCIES
	UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPENDENCIES
)

file(INSTALL
	DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app/Contents/libs\"
	TYPE SHARED_LIBRARY
	FOLLOW_SYMLINK_CHAIN
	FILES \${RESOLVED_DEPENDENCIES}
)

set(CHANGES \"\")
set(LIBS \"\")
foreach(RESOLVED_DEPENDENCY \${RESOLVED_DEPENDENCIES})
	string(REGEX REPLACE \"^.*/([^/]+)\" \"\\\\1\" LIB \"\${RESOLVED_DEPENDENCY}\")
	list(APPEND CHANGES -change \"\${RESOLVED_DEPENDENCY}\" \"@rpath/../libs/\${LIB}\")
	list(APPEND LIBS \"\${LIB}\")
endforeach()

if(\"${APPLE_CERT_NAME}\" STREQUAL -)
	set(ENTITLEMENTS adhoc)
else()
	set(ENTITLEMENTS identity-required)
endif()

foreach(LIB \${LIBS})
	execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${CHANGES} -id \"@rpath/../libs/\${LIB}\" \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app/Contents/libs/\${LIB}\")
	execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app/Contents/libs/\${LIB}\")
	execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APPLE_CERT_NAME}\" --entitlements \"${SRC}/pkg/macos/entitlements-\${ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app/Contents/libs/\${LIB}\")
endforeach()
execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${CHANGES} \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app/Contents/MacOS/${EXE}\")
execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app\")
execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APPLE_CERT_NAME}\" --entitlements \"${SRC}/pkg/macos/entitlements-\${ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${INSTALL_DIR}/${EXE}.app\")

")

set(CPACK_DMG_VOLUME_NAME ${EXE})
set(CPACK_PACKAGE_VENDOR nightmareci)
if(${PACKAGE_TYPE} MATCHES Portable)
	set(CPACK_PACKAGE_FILE_NAME "${EXE}-Portable-App-macOS")
elseif(${PACKAGE_TYPE} MATCHES Installable)
	set(CPACK_PACKAGE_FILE_NAME "${EXE}-Installable-App-macOS")
endif()
set(CPACK_GENERATOR DragNDrop)
include(CPack)
