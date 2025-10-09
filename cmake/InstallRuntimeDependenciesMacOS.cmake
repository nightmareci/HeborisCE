set_target_properties(${EXE} PROPERTIES
        INSTALL_RPATH @executable_path
)
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
        DESTINATION \"\${CMAKE_INSTALL_PREFIX}/${EXE}/libs\"
        TYPE SHARED_LIBRARY
        FOLLOW_SYMLINK_CHAIN
        FILES \${RESOLVED_DEPENDENCIES}
)

set(CHANGES \"\")
set(LIBS \"\")
foreach(RESOLVED_DEPENDENCY \${RESOLVED_DEPENDENCIES})
        string(REGEX REPLACE \"^.*/([^/]+)\" \"\\\\1\" LIB \"\${RESOLVED_DEPENDENCY}\")
        list(APPEND CHANGES -change \"\${RESOLVED_DEPENDENCY}\" \"@rpath/libs/\${LIB}\")
        list(APPEND LIBS \"\${LIB}\")
endforeach()

if(\"${APPLE_CERT_NAME}\" STREQUAL -)
        set(ENTITLEMENTS adhoc)
else()
        set(ENTITLEMENTS identity-required)
endif()

foreach(LIB \${LIBS})
        execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${CHANGES} -id \"@rpath/libs/\${LIB}\" \"\${CMAKE_INSTALL_PREFIX}/${EXE}/libs/\${LIB}\")
        execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${EXE}/libs/\${LIB}\")
        execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APPLE_CERT_NAME}\" --entitlements \"${SRC}/pkg/macos/entitlements-\${ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${EXE}/libs/\${LIB}\")
endforeach()
execute_process(COMMAND ${CMAKE_INSTALL_NAME_TOOL} \${CHANGES} \"\${CMAKE_INSTALL_PREFIX}/${EXE}/${EXE}\")
execute_process(COMMAND xcrun codesign --remove-signature \"\${CMAKE_INSTALL_PREFIX}/${EXE}/${EXE}\")
execute_process(COMMAND xcrun codesign -f -o runtime --timestamp -s \"${APPLE_CERT_NAME}\" --entitlements \"${SRC}/pkg/macos/entitlements-\${ENTITLEMENTS}.xml\" \"\${CMAKE_INSTALL_PREFIX}/${EXE}/${EXE}\")

")

set(CPACK_DMG_VOLUME_NAME ${EXE})
set(CPACK_PACKAGE_VENDOR nightmareci)
set(CPACK_PACKAGE_FILE_NAME "${EXE}-Portable-Commandline-macOS")
set(CPACK_GENERATOR DragNDrop)
include(CPack)