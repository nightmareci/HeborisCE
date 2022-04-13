if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VITASDK)
	message(FATAL_ERROR "Vita SDK CMake toolchain file must be provided to the CMake configure command.")
else()
	include("${VITASDK}/share/vita.cmake" REQUIRED)
endif()

set(VITA_APP_NAME ${EXE})

set(VITA_TITLEID  "NMCI00000")

set(VITA_VERSION  "01.00")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

add_executable(${EXE} ${EXE_SOURCES})

target_compile_definitions(${EXE} PRIVATE USE_PHYSFS_SETSANECONFIG)

set(BUILD_TARGET ${CMAKE_SYSTEM_NAME} CACHE STRING "The build target. By default, the current platform is the build target. Can be explicitly set to \"Vita\", to force building for PlayStation Vita.")

set(DEFAULT_JOYKEY_ASSIGN [[
	{\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 8\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 6\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 7\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 9\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 1\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 2\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 3\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 5\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 10\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 11\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 0\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 1\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 2\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 3\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 4\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 5\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 6\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 7\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 8\
	        }\
	    },\
	    {\
	        .device = 0,\
	        .guid = GetJoyPadGUID(0),\
	        .type = JOYKEY_BUTTON,\
	        .setting = {\
	            .button = 9\
	        }\
	    },\
	}
]])

set(DEFAULT_SCREEN_MODE "(SCREENMODE_FULLSCREEN | SCREENMODE_DETAILLEVEL | SCREENMODE_VSYNC)")

set(SCREEN_SUBPIXEL_OFFSET "0.0f")

configure_file("${SRC}/src/main_sdl/platform.h.in" "src/main_sdl/platform.h" @ONLY)

target_link_directories(${EXE} PRIVATE ${BIN})

target_link_libraries(${EXE}
	SDL2_image
	SDL2_mixer
	SDL2

	physfs

	jpeg
	png
	webp

	mikmod
	mpg123
	vorbisfile
	vorbis
	ogg
	FLAC

	pthread
	z
	m

	SceDisplay_stub
	SceCtrl_stub
	SceAudio_stub
	SceAudioIn_stub
	SceSysmodule_stub
	SceGxm_stub
	SceCommonDialog_stub
	SceAppMgr_stub
	SceTouch_stub
	SceHid_stub
	SceMotion_stub
)

target_include_directories(${EXE} PRIVATE
	"${VITASDK}/arm-vita-eabi/include/SDL2"
)

file(GLOB_RECURSE RES RELATIVE ${SRC} res/*)
file(GLOB_RECURSE MISSION RELATIVE ${SRC} config/mission/*)
file(GLOB_RECURSE STAGE RELATIVE ${SRC} config/stage/*)

set(ASSETS "")
foreach(ITEM ${RES})
	list(APPEND ASSETS FILE)
	list(APPEND ASSETS ${ITEM})
	list(APPEND ASSETS ${ITEM})
endforeach()
foreach(ITEM ${MISSION})
	list(APPEND ASSETS FILE)
	list(APPEND ASSETS ${ITEM})
	list(APPEND ASSETS ${ITEM})
endforeach()
foreach(ITEM ${STAGE})
	list(APPEND ASSETS FILE)
	list(APPEND ASSETS ${ITEM})
	list(APPEND ASSETS ${ITEM})
endforeach()

vita_create_self(${EXE}.self ${EXE} UNSAFE)
vita_create_vpk(${EXE}.vpk ${VITA_TITLEID} ${EXE}.self
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}

	# Game assets.
	${ASSETS}

	# Files for LiveArea and boot.
	FILE ${SRC}/pkg/vita/sce_sys/icon0.png sce_sys/icon0.png
	FILE ${SRC}/pkg/vita/sce_sys/pic0.png sce_sys/pic0.png
	FILE ${SRC}/pkg/vita/sce_sys/livearea/contents/bg0.png sce_sys/livearea/contents/bg0.png
	FILE ${SRC}/pkg/vita/sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
)
