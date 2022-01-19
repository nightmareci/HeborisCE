function(AddLibrariesPkgConfig EXE)
	find_package(PkgConfig REQUIRED)

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
endfunction()
