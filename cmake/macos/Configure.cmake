set(APP_FRAMEWORK_TYPE "MACOSX SDL2")

# Builds Mac application bundle, packaged into a DMG with CPack by default.

list(APPEND APP_SOURCES "${APP_SRC_DIR}/src/app/physfsrwops.c")
list(APPEND APP_HEADERS "${APP_SRC_DIR}/src/app/physfsrwops.h")

if(${APP_PACKAGE_TYPE} STREQUAL "Portable Mac App")
	message(STATUS "Configuring portable Mac app package")
	add_executable(${APP_EXE} MACOSX_BUNDLE "${APP_SRC_DIR}/pkg/macos/${APP_EXE}.icns" ${APP_SOURCES} ${APP_HEADERS})
	set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_PORTABLE CACHE STRING "" FORCE)
	configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
	set_source_files_properties("${APP_SRC_DIR}/pkg/macos/${APP_EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_EXE})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
	set(MACOSX_BUNDLE_ICON_FILE ${APP_EXE})
	set(SDL_FILESYSTEM_BASE_DIR_TYPE parent)
elseif(${APP_PACKAGE_TYPE} STREQUAL "Installable Mac App")
	message(STATUS "Configuring installable Mac app package")
	add_executable(${APP_EXE} MACOSX_BUNDLE "${APP_SRC_DIR}/pkg/macos/${APP_EXE}.icns" ${APP_SOURCES} ${APP_HEADERS})
	set(APP_FILESYSTEM_TYPE APP_FILESYSTEM_INSTALLABLE CACHE STRING "" FORCE)
	configure_file("${APP_SRC_DIR}/src/app/APP_build_config.h.in" "${APP_BIN_DIR}/src/app/APP_build_config.h" @ONLY)
	set_source_files_properties("${APP_SRC_DIR}/pkg/macos/${APP_EXE}.icns" PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
	set(MACOSX_BUNDLE_BUNDLE_NAME ${APP_EXE})
	set(MACOSX_BUNDLE_GUI_IDENTIFIER ${BUNDLE_IDENTIFIER})
	set(MACOSX_BUNDLE_ICON_FILE ${APP_EXE})
	set(SDL_FILESYSTEM_BASE_DIR_TYPE resource)
else()
	message(FATAL_ERROR "Package type \"${APP_PACKAGE_TYPE}\" unsupported when building a macOS app")
endif()

set_target_properties(${APP_EXE} PROPERTIES
	MACOSX_BUNDLE_INFO_PLIST "${APP_SRC_DIR}/pkg/macos/MacOSXBundleInfo.plist.in"
	INSTALL_RPATH @executable_path
	RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}"
)

if(${APP_PACKAGE_TYPE} MATCHES Portable)
	set(APP_INSTALL_DIR ${APP_EXE})
	install(TARGETS ${APP_EXE} BUNDLE DESTINATION "${APP_EXE}")
	install(FILES "${APP_SRC_DIR}/changelog.txt" "${APP_SRC_DIR}/heboris.txt" "${APP_SRC_DIR}/LICENSE-hashmap.txt" "${APP_SRC_DIR}/README.md" DESTINATION "${APP_EXE}")
	install(DIRECTORY "${APP_SRC_DIR}/config/mission" "${APP_SRC_DIR}/config/stage" DESTINATION "${APP_EXE}/config")
	install(DIRECTORY "${APP_SRC_DIR}/res" DESTINATION "${APP_EXE}")
elseif(${APP_PACKAGE_TYPE} MATCHES Installable)
	set(APP_INSTALL_DIR .)
	install(TARGETS ${APP_EXE} BUNDLE DESTINATION .)
	install(FILES "${APP_SRC_DIR}/changelog.txt" "${APP_SRC_DIR}/heboris.txt" "${APP_SRC_DIR}/LICENSE-hashmap.txt" "${APP_SRC_DIR}/README.md" DESTINATION .)
	install(DIRECTORY "${APP_SRC_DIR}/config/mission" "${APP_SRC_DIR}/config/stage" DESTINATION "${APP_EXE}.app/Contents/Resources/config")
	install(DIRECTORY "${APP_SRC_DIR}/res" DESTINATION "${APP_EXE}.app/Contents/Resources")
endif()

if(APP_VENDORED)
	include("${APP_SRC_DIR}/cmake/lib/AddLibrariesVendor.cmake" REQUIRED)
	AddLibrariesVendor(${APP_EXE} TRUE)
else()
	include("${APP_SRC_DIR}/cmake/lib/AddLibrariesPkgConfig.cmake" REQUIRED)
	AddLibrariesPkgConfig(${APP_EXE})
endif()

set(APP_APPLE_CERT_NAME - CACHE STRING "The name of the Apple supplied code signing certificate to use. Defaults to adhoc signing (\"-\")")
include(CMakeFindBinUtils)
install(CODE "

set(APP_POST_EXCLUDES
\"^/usr/lib\"
\"^/System/Library/Frameworks\"
)

file(GET_RUNTIME_DEPENDENCIES
	EXECUTABLES \$<TARGET_FILE:${APP_EXE}>
	POST_EXCLUDE_REGEXES
		\${APP_POST_EXCLUDES}
	RESOLVED_DEPENDENCIES_VAR RESOLVED_DEPENDENCIES
	UNRESOLVED_DEPENDENCIES_VAR UNRESOLVED_DEPENDENCIES
)

file(INSTALL
	DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs\"
	TYPE SHARED_LIBRARY
	FOLLOW_SYMLINK_CHAIN
	FILES \${RESOLVED_DEPENDENCIES}
)

set(APP_CHANGES \"\")
set(APP_LIBS \"\")
foreach(RESOLVED_DEPENDENCY \${RESOLVED_DEPENDENCIES})
	string(REGEX REPLACE \"^.*/([^/]+)\" \"\\\\1\" LIB \"\${RESOLVED_DEPENDENCY}\")
	list(APPEND APP_CHANGES -change \"\${RESOLVED_DEPENDENCY}\" \"@rpath/../libs/\${LIB}\")
	list(APPEND APP_LIBS \"\${LIB}\")
endforeach()

if(\"${APP_APPLE_CERT_NAME}\" STREQUAL -)
	set(APP_ENTITLEMENTS adhoc)
else()
	set(APP_ENTITLEMENTS identity-required)
endif()

foreach(APP_LIB \${APP_LIBS})
	execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${APP_CHANGES} -id \"@rpath/../libs/\${APP_LIB}\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
	execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
	execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APP_APPLE_CERT_NAME}\" --entitlements \"${APP_SRC_DIR}/pkg/macos/entitlements-\${APP_ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/libs/\${APP_LIB}\")
endforeach()
execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${APP_CHANGES} \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app/Contents/MacOS/${APP_EXE}\")
execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app\")
execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APP_APPLE_CERT_NAME}\" --entitlements \"${APP_SRC_DIR}/pkg/macos/entitlements-\${APP_ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${APP_INSTALL_DIR}/${APP_EXE}.app\")

")

set(CPACK_DMG_VOLUME_NAME ${APP_EXE})
set(CPACK_PACKAGE_VENDOR "${PROJECT_ORG}")
if(${APP_PACKAGE_TYPE} MATCHES Portable)
	set(CPACK_PACKAGE_FILE_NAME "${APP_EXE}-Portable-App-macOS")
elseif(${APP_PACKAGE_TYPE} MATCHES Installable)
	set(CPACK_PACKAGE_FILE_NAME "${APP_EXE}-Installable-App-macOS")
endif()
set(CPACK_GENERATOR DragNDrop)
include(CPack)
