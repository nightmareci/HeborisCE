set(FRAMEWORK_VER "VITA-SDL2")

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND NOT DEFINED VITASDK)
	message(FATAL_ERROR "Vita SDK CMake toolchain file must be provided to the CMake configure command.")
else()
	include("${VITASDK}/share/vita.cmake" REQUIRED)
endif()

list(APPEND EXE_SOURCES "${SRC}/src/main_sdl/physfsrwops.c")
list(APPEND EXE_HEADERS "${SRC}/src/main_sdl/physfsrwops.h")

if(ENABLE_GAME_CONTROLLER)
	list(APPEND EXE_SOURCES "${SRC}/src/main_sdl/gamecontroller_vita.c")
endif()

add_executable(${EXE} ${EXE_SOURCES} ${EXE_HEADERS})

set(ENABLE_KEYBOARD FALSE)
set(ENABLE_JOYSTICK FALSE)
set(ONLY_CONTROLLER_TYPE YGS_CONTROLLER_PLAYSTATION)
set(ONLY_SDL_CONTROLLER_TYPE SDL_CONTROLLER_TYPE_PS4)

set(FILESYSTEM_TYPE FILESYSTEM_PHYSFS)

set(DEFAULT_SCREEN_MODE "(YGS_SCREENMODE_FULLSCREEN | YGS_SCREENMODE_DETAILLEVEL | YGS_SCREENMODE_VSYNC)")

set(SCREEN_SUBPIXEL_OFFSET "0.0f")

configure_file("${SRC}/src/main_sdl/defs.h.in" "src/main_sdl/defs.h" @ONLY)

target_link_directories(${EXE} PRIVATE "${BIN}")

target_link_libraries(${EXE}
	SDL2_image
	SDL2_mixer
	SDL2

	physfs

	jpeg
	png
	webp

	mikmod
	modplug
	mpg123
	vorbisfile
	vorbis
	ogg
	FLAC

	pthread
	z
	m
	stdc++

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

file(ARCHIVE_CREATE OUTPUT "${BIN}/assets.zip"
	PATHS "${SRC}/res/bg" "${SRC}/res/bgm" "${SRC}/res/font" "${SRC}/res/graphics" "${SRC}/res/se" "${SRC}/config/mission" "${SRC}/config/stage"
	FORMAT zip
)

set(VITA_APP_NAME ${EXE})
set(VITA_TITLEID "NMCI00000")
set(VITA_VERSION "01.00")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

vita_create_self(${EXE}.self ${EXE})
vita_create_vpk(${EXE}.vpk ${VITA_TITLEID} ${EXE}.self
	VERSION ${VITA_VERSION}
	NAME "${VITA_APP_NAME}"

	# Game assets.
	FILE "${BIN}/assets.zip" assets.zip

	# Files for LiveArea and boot.
	FILE "${SRC}/pkg/vita/sce_sys/icon0.png" sce_sys/icon0.png
	FILE "${SRC}/pkg/vita/sce_sys/pic0.png" sce_sys/pic0.png
	FILE "${SRC}/pkg/vita/sce_sys/livearea/contents/bg0.png" sce_sys/livearea/contents/bg0.png
	FILE "${SRC}/pkg/vita/sce_sys/livearea/contents/template.xml" sce_sys/livearea/contents/template.xml
)
