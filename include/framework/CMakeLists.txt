set(_HEADERS
	audio.h
	filesystem.h
	framework.h
	input.h
	main.h
	stdinc.h
	video.h
)
target_sources(${EXECUTABLE} PRIVATE ${_HEADERS})
target_precompile_headers(${EXECUTABLE} PRIVATE ${_HEADERS})
