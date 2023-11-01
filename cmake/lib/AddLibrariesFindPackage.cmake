function(AddLibrariesFindPackage EXE)
	find_package(SDL2 REQUIRED)
	find_package(SDL2_image REQUIRED)
	find_package(SDL2_mixer REQUIRED)
	find_package(PhysFS REQUIRED)
	target_link_libraries(${EXE}
		PUBLIC
			SDL2::SDL2
			SDL2::SDL2main
			SDL2_image::SDL2_image
			SDL2_mixer::SDL2_mixer
			${PHYSFS_LIBRARY}
	)
	target_include_directories(${EXE} SYSTEM PRIVATE
		"${PHYSFS_INCLUDE_DIR}"
	)
endfunction()
