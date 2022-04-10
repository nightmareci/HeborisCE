if(NOT DEFINED VITASDK)
	message(FATAL_ERROR "The Vita SDK toolchain file must be provided when running CMake")
endif()

include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME ${EXE})

set(VITA_TITLEID  "HBRS00001")

set(VITA_VERSION  "00.01")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

add_executable(${EXE} ${EXE_SOURCES})

add_compile_definitions(USE_PHYSFS_SETSANECONFIG)

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
file(GLOB_RECURSE CONFIG RELATIVE ${SRC} config/*)

set(RESDATA "")

foreach(ITEM ${RES})
	list(APPEND RESDATA FILE)
	list(APPEND RESDATA ${ITEM})
	list(APPEND RESDATA ${ITEM})
endforeach()

set(CONFIGDATA "")

foreach(ITEM ${CONFIG})
	list(APPEND CONFIGDATA FILE)
	list(APPEND CONFIGDATA ${ITEM})
	list(APPEND CONFIGDATA ${ITEM})
endforeach()

vita_create_self(${EXE}.self ${EXE} UNSAFE)
vita_create_vpk(${EXE}.vpk ${VITA_TITLEID} ${EXE}.self
	VERSION ${VITA_VERSION}
	NAME ${VITA_APP_NAME}

	${CONFIGDATA}
	${RESDATA}
)
