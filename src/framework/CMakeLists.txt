set(_HEADERS
	audio_private.h
	bdf.h
	error.h
	filesystem_private.h
	global.h
	hashtable.h
	input_private.h
	main_private.h
	mem_private.h
	video_private.h
	worker.h
)
target_sources(${EXECUTABLE} PRIVATE
	audio.c
	bdf.c
	error.c
	filesystem.c
	global.c
	hashtable.c
	input.c
	main.c
	mem.c
	video.c

	${_HEADERS}
)
target_precompile_headers(${EXECUTABLE} PRIVATE ${_HEADERS})

if(WORKER_MULTITHREADED)
	target_sources(${EXECUTABLE} PRIVATE worker_multithreaded.c)
else()
	target_sources(${EXECUTABLE} PRIVATE worker_synchronous.c)
endif()

if(VITA)
	target_sources(${EXECUTABLE} PRIVATE vita.c)
endif()
