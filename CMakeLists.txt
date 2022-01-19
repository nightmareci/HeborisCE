cmake_minimum_required(VERSION 3.19)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(Heboris-Vita)

include("$ENV{VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Heboris")

set(VITA_TITLEID  "HBRS00001")

set(VITA_VERSION  "00.01")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -g -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -std=c++11")

set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1 -d ATTRIBUTE2=12")

include_directories()


link_directories(
  ${CMAKE_CURRENT_BINARY_DIR}
)

# Add all the files needed to compile here
add_executable(${PROJECT_NAME}
	src/game/prototypes.h
	src/game/gamedef.h
	src/game/gamestart.h
	src/game/init.h
	src/game/speed.h

	src/game/init.inc

	src/game/gamestart.c
	src/game/speed.c

	src/main_sdl/include.h
	src/main_sdl/ygs2kfunc.h
	src/main_sdl/SDL_kanji.h
	src/main_sdl/physfsrwops.h

	src/main_sdl/main.c
	src/main_sdl/ygs2kfunc.c
	src/main_sdl/SDL_kanji.c
	src/main_sdl/physfsrwops.c

	src/script/include.h

	src/script/mission_info.h
	src/script/flexdraw.h
	src/script/config.h
	src/script/world.h
	src/script/classic.h
	src/script/ars.h
	src/script/classic_D.h
	src/script/effect.h
	src/script/sound.h
	src/script/practice.h
	src/script/tomoyo.h
	src/script/ranking.h
	src/script/setdef.h
	src/script/replay.h
	src/script/staffroll.h
	src/script/cpu.h
	src/script/sectime.h
	src/script/mission.h
	src/script/view.h
	src/script/grade.h
	src/script/ranking2.h
	src/script/ranking3.h

	src/script/mission_info.c
	src/script/flexdraw.c
	src/script/config.c
	src/script/world.c
	src/script/classic.c
	src/script/ars.c
	src/script/classic_D.c
	src/script/effect.c
	src/script/sound.c
	src/script/practice.c
	src/script/tomoyo.c
	src/script/ranking.c
	src/script/setdef.c
	src/script/replay.c
	src/script/staffroll.c
	src/script/cpu.c
	src/script/sectime.c
	src/script/mission.c
	src/script/view.c
	src/script/grade.c
	src/script/ranking2.c
	src/script/ranking3.c
)

# Library to link to (drop the -l prefix). This will mostly be stubs.
target_link_libraries(${PROJECT_NAME}
	SDL2
	SDL2_image
	SDL2_mixer
	SceDisplay_stub
	SceCtrl_stub
	SceAudio_stub
	SceSysmodule_stub
	SceGxm_stub
	SceCommonDialog_stub
	SceAppMgr_stub
	SceTouch_stub
	SceHid_stub
	SceMotion_stub
	m
	z
	physfs
	jpeg
	png
	webp
	mikmod
	mpg123
	vorbis
	vorbisfile
	ogg
	pthread
	FLAC
)

## Create Vita files
vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} UNSAFE)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}

  FILE res/bg/highDetail/back_vs.png res/bg/highDetail/back_vs.png
  FILE res/bg/highDetail/back01.png res/bg/highDetail/back01.png
  FILE res/bg/highDetail/back02.png res/bg/highDetail/back02.png
  FILE res/bg/highDetail/back03.png res/bg/highDetail/back03.png
  FILE res/bg/highDetail/back04.png res/bg/highDetail/back04.png
  FILE res/bg/highDetail/back05.png res/bg/highDetail/back05.png
  FILE res/bg/highDetail/back06.png res/bg/highDetail/back06.png
  FILE res/bg/highDetail/back07.png res/bg/highDetail/back07.png
  FILE res/bg/highDetail/back08.png res/bg/highDetail/back08.png
  FILE res/bg/highDetail/back09.png res/bg/highDetail/back09.png
  FILE res/bg/highDetail/back10.png res/bg/highDetail/back10.png
  FILE res/bg/highDetail/back11.png res/bg/highDetail/back11.png
  FILE res/bg/highDetail/back12.png res/bg/highDetail/back12.png

  FILE res/bg/lowDetail/back_vs.png res/bg/lowDetail/back_vs.png
  FILE res/bg/lowDetail/back01.png res/bg/lowDetail/back01.png
  FILE res/bg/lowDetail/back02.png res/bg/lowDetail/back02.png
  FILE res/bg/lowDetail/back03.png res/bg/lowDetail/back03.png
  FILE res/bg/lowDetail/back04.png res/bg/lowDetail/back04.png
  FILE res/bg/lowDetail/back05.png res/bg/lowDetail/back05.png
  FILE res/bg/lowDetail/back06.png res/bg/lowDetail/back06.png
  FILE res/bg/lowDetail/back07.png res/bg/lowDetail/back07.png
  FILE res/bg/lowDetail/back08.png res/bg/lowDetail/back08.png
  FILE res/bg/lowDetail/back09.png res/bg/lowDetail/back09.png
  FILE res/bg/lowDetail/back10.png res/bg/lowDetail/back10.png
  FILE res/bg/lowDetail/back11.png res/bg/lowDetail/back11.png
  FILE res/bg/lowDetail/back12.png res/bg/lowDetail/back12.png

  FILE res/bgm/bgm.mid res/bgm/bgm.mid

  FILE res/font/5x10a.bdf res/font/5x10a.bdf
  FILE res/font/6x12a.bdf res/font/6x12a.bdf
  FILE res/font/8x16a.bdf res/font/8x16a.bdf
  FILE res/font/knj10.bdf res/font/knj10.bdf
  FILE res/font/knj12.bdf res/font/knj12.bdf
  FILE res/font/knj16.bdf res/font/knj16.bdf

  FILE res/se/applause.wav res/se/applause.wav
  FILE res/se/cheer.wav res/se/cheer.wav
  FILE res/se/erase1.wav res/se/erase1.wav
  FILE res/se/erase2.wav res/se/erase2.wav
  FILE res/se/erase3.wav res/se/erase3.wav
  FILE res/se/erase4.wav res/se/erase4.wav
  FILE res/se/gameover.wav res/se/gameover.wav
  FILE res/se/go.wav res/se/go.wav
  FILE res/se/gon.wav res/se/gon.wav
  FILE res/se/kachi.wav res/se/kachi.wav
  FILE res/se/kettei.wav res/se/kettei.wav
  FILE res/se/kon.wav res/se/kon.wav
  FILE res/se/levelup.wav res/se/levelup.wav
  FILE res/se/move.wav res/se/move.wav
  FILE res/se/ready.wav res/se/ready.wav
  FILE res/se/rotate.wav res/se/rotate.wav
  FILE res/se/shaki.wav res/se/shaki.wav
  FILE res/se/tumagari.wav res/se/tumagari.wav
  FILE res/se/up.wav res/se/up.wav

  # that was a pain (i want to kill myself after doing this, pray the vim god)
  FILE res/graphics/highDetail/break0.png res/graphics/highDetail/break0.png
  FILE res/graphics/highDetail/break0_tap.png res/graphics/highDetail/break0_tap.png
  FILE res/graphics/highDetail/break1.png res/graphics/highDetail/break1.png
  FILE res/graphics/highDetail/break1_tap.png res/graphics/highDetail/break1_tap.png
  FILE res/graphics/highDetail/break2.png res/graphics/highDetail/break2.png
  FILE res/graphics/highDetail/break2_tap.png res/graphics/highDetail/break2_tap.png
  FILE res/graphics/highDetail/break3.png res/graphics/highDetail/break3.png
  FILE res/graphics/highDetail/break3_tap.png res/graphics/highDetail/break3_tap.png
  FILE res/graphics/highDetail/break4.png res/graphics/highDetail/break4.png
  FILE res/graphics/highDetail/break4_tap.png res/graphics/highDetail/break4_tap.png
  FILE res/graphics/highDetail/break5.png res/graphics/highDetail/break5.png
  FILE res/graphics/highDetail/break5_tap.png res/graphics/highDetail/break5_tap.png
  FILE res/graphics/highDetail/break6.png res/graphics/highDetail/break6.png
  FILE res/graphics/highDetail/break6_tap.png res/graphics/highDetail/break6_tap.png
  FILE res/graphics/highDetail/break7.png res/graphics/highDetail/break7.png
  FILE res/graphics/highDetail/break7_tap.png res/graphics/highDetail/break7_tap.png
  FILE res/graphics/highDetail/del_field.png res/graphics/highDetail/del_field.png
  FILE res/graphics/highDetail/fade.png res/graphics/highDetail/fade.png
  FILE res/graphics/highDetail/fldmirror01.png res/graphics/highDetail/fldmirror01.png
  FILE res/graphics/highDetail/fldmirror02.png res/graphics/highDetail/fldmirror02.png
  FILE res/graphics/highDetail/fldmirror03.png res/graphics/highDetail/fldmirror03.png
  FILE res/graphics/highDetail/fldmirror04.png res/graphics/highDetail/fldmirror04.png
  FILE res/graphics/highDetail/gamemodefont.png res/graphics/highDetail/gamemodefont.png
  FILE res/graphics/highDetail/grade.png res/graphics/highDetail/grade.png
  FILE res/graphics/highDetail/guide.png res/graphics/highDetail/guide.png
  FILE res/graphics/highDetail/hanabi_blue.png res/graphics/highDetail/hanabi_blue.png
  FILE res/graphics/highDetail/hanabi_green.png res/graphics/highDetail/hanabi_green.png
  FILE res/graphics/highDetail/hanabi_orange.png res/graphics/highDetail/hanabi_orange.png
  FILE res/graphics/highDetail/hanabi_purple.png res/graphics/highDetail/hanabi_purple.png
  FILE res/graphics/highDetail/hanabi_red.png res/graphics/highDetail/hanabi_red.png
  FILE res/graphics/highDetail/hanabi_waterblue.png res/graphics/highDetail/hanabi_waterblue.png
  FILE res/graphics/highDetail/heboblk_big.png res/graphics/highDetail/heboblk_big.png
  FILE res/graphics/highDetail/heboblk_old.png res/graphics/highDetail/heboblk_old.png
  FILE res/graphics/highDetail/heboblk_sp.png res/graphics/highDetail/heboblk_sp.png
  FILE res/graphics/highDetail/heboblk_sp2.png res/graphics/highDetail/heboblk_sp2.png
  FILE res/graphics/highDetail/heboblk0.png res/graphics/highDetail/heboblk0.png
  FILE res/graphics/highDetail/heboblk0B.png res/graphics/highDetail/heboblk0B.png
  FILE res/graphics/highDetail/heboblk1.png res/graphics/highDetail/heboblk1.png
  FILE res/graphics/highDetail/heboblk2.png res/graphics/highDetail/heboblk2.png
  FILE res/graphics/highDetail/heboblk3.png res/graphics/highDetail/heboblk3.png
  FILE res/graphics/highDetail/heboblk4_5.png res/graphics/highDetail/heboblk4_5.png
  FILE res/graphics/highDetail/heboblk6.png res/graphics/highDetail/heboblk6.png
  FILE res/graphics/highDetail/heboflb0.png res/graphics/highDetail/heboflb0.png
  FILE res/graphics/highDetail/heboflb1.png res/graphics/highDetail/heboflb1.png
  FILE res/graphics/highDetail/heboflb2.png res/graphics/highDetail/heboflb2.png
  FILE res/graphics/highDetail/heboflb3.png res/graphics/highDetail/heboflb3.png
  FILE res/graphics/highDetail/hebofld.png res/graphics/highDetail/hebofld.png
  FILE res/graphics/highDetail/hebofont.png res/graphics/highDetail/hebofont.png
  FILE res/graphics/highDetail/hebofont3.png res/graphics/highDetail/hebofont3.png
  FILE res/graphics/highDetail/hebofont4.png res/graphics/highDetail/hebofont4.png
  FILE res/graphics/highDetail/hebofont5.png res/graphics/highDetail/hebofont5.png
  FILE res/graphics/highDetail/heboris_road.png res/graphics/highDetail/heboris_road.png
  FILE res/graphics/highDetail/hebospr.png res/graphics/highDetail/hebospr.png
  FILE res/graphics/highDetail/item_guage.png res/graphics/highDetail/item_guage.png
  FILE res/graphics/highDetail/item.png res/graphics/highDetail/item.png
  FILE res/graphics/highDetail/itemerase.png res/graphics/highDetail/itemerase.png
  FILE res/graphics/highDetail/itemGra.png res/graphics/highDetail/itemGra.png
  FILE res/graphics/highDetail/laser.png res/graphics/highDetail/laser.png
  FILE res/graphics/highDetail/line.png res/graphics/highDetail/line.png
  FILE res/graphics/highDetail/loading.png res/graphics/highDetail/loading.png
  FILE res/graphics/highDetail/medal_ti.png res/graphics/highDetail/medal_ti.png
  FILE res/graphics/highDetail/medal.png res/graphics/highDetail/medal.png
  FILE res/graphics/highDetail/mirror_effect_TAP.png res/graphics/highDetail/mirror_effect_TAP.png
  FILE res/graphics/highDetail/perase1.png res/graphics/highDetail/perase1.png
  FILE res/graphics/highDetail/perase2.png res/graphics/highDetail/perase2.png
  FILE res/graphics/highDetail/perase3.png res/graphics/highDetail/perase3.png
  FILE res/graphics/highDetail/perase4.png res/graphics/highDetail/perase4.png
  FILE res/graphics/highDetail/perase5.png res/graphics/highDetail/perase5.png
  FILE res/graphics/highDetail/perase6.png res/graphics/highDetail/perase6.png
  FILE res/graphics/highDetail/perase7.png res/graphics/highDetail/perase7.png
  FILE res/graphics/highDetail/rollmark.png res/graphics/highDetail/rollmark.png
  FILE res/graphics/highDetail/rot.png res/graphics/highDetail/rot.png
  FILE res/graphics/highDetail/rotstext.png res/graphics/highDetail/rotstext.png
  FILE res/graphics/highDetail/shootingstar.png res/graphics/highDetail/shootingstar.png
  FILE res/graphics/highDetail/shuffle_field_effect.png res/graphics/highDetail/shuffle_field_effect.png
  FILE res/graphics/highDetail/staffroll.png res/graphics/highDetail/staffroll.png
  FILE res/graphics/highDetail/text.png res/graphics/highDetail/text.png
  FILE res/graphics/highDetail/text2.png res/graphics/highDetail/text2.png
  FILE res/graphics/highDetail/title_old.png res/graphics/highDetail/title_old.png
  FILE res/graphics/highDetail/title.png res/graphics/highDetail/title.png
  FILE res/graphics/highDetail/tomoyo_eh_fade.png res/graphics/highDetail/tomoyo_eh_fade.png

  FILE res/graphics/lowDetail/break0.png res/graphics/lowDetail/break0.png
  FILE res/graphics/lowDetail/break0_tap.png res/graphics/lowDetail/break0_tap.png
  FILE res/graphics/lowDetail/break1.png res/graphics/lowDetail/break1.png
  FILE res/graphics/lowDetail/break1_tap.png res/graphics/lowDetail/break1_tap.png
  FILE res/graphics/lowDetail/break2.png res/graphics/lowDetail/break2.png
  FILE res/graphics/lowDetail/break2_tap.png res/graphics/lowDetail/break2_tap.png
  FILE res/graphics/lowDetail/break3.png res/graphics/lowDetail/break3.png
  FILE res/graphics/lowDetail/break3_tap.png res/graphics/lowDetail/break3_tap.png
  FILE res/graphics/lowDetail/break4.png res/graphics/lowDetail/break4.png
  FILE res/graphics/lowDetail/break4_tap.png res/graphics/lowDetail/break4_tap.png
  FILE res/graphics/lowDetail/break5.png res/graphics/lowDetail/break5.png
  FILE res/graphics/lowDetail/break5_tap.png res/graphics/lowDetail/break5_tap.png
  FILE res/graphics/lowDetail/break6.png res/graphics/lowDetail/break6.png
  FILE res/graphics/lowDetail/break6_tap.png res/graphics/lowDetail/break6_tap.png
  FILE res/graphics/lowDetail/break7.png res/graphics/lowDetail/break7.png
  FILE res/graphics/lowDetail/break7_tap.png res/graphics/lowDetail/break7_tap.png
  FILE res/graphics/lowDetail/del_field.png res/graphics/lowDetail/del_field.png
  FILE res/graphics/lowDetail/fade.png res/graphics/lowDetail/fade.png
  FILE res/graphics/lowDetail/fldmirror01.png res/graphics/lowDetail/fldmirror01.png
  FILE res/graphics/lowDetail/fldmirror02.png res/graphics/lowDetail/fldmirror02.png
  FILE res/graphics/lowDetail/fldmirror03.png res/graphics/lowDetail/fldmirror03.png
  FILE res/graphics/lowDetail/fldmirror04.png res/graphics/lowDetail/fldmirror04.png
  FILE res/graphics/lowDetail/gamemodefont.png res/graphics/lowDetail/gamemodefont.png
  FILE res/graphics/lowDetail/grade.png res/graphics/lowDetail/grade.png
  FILE res/graphics/lowDetail/guide.png res/graphics/lowDetail/guide.png
  FILE res/graphics/lowDetail/hanabi_blue.png res/graphics/lowDetail/hanabi_blue.png
  FILE res/graphics/lowDetail/hanabi_green.png res/graphics/lowDetail/hanabi_green.png
  FILE res/graphics/lowDetail/hanabi_orange.png res/graphics/lowDetail/hanabi_orange.png
  FILE res/graphics/lowDetail/hanabi_purple.png res/graphics/lowDetail/hanabi_purple.png
  FILE res/graphics/lowDetail/hanabi_red.png res/graphics/lowDetail/hanabi_red.png
  FILE res/graphics/lowDetail/hanabi_waterblue.png res/graphics/lowDetail/hanabi_waterblue.png
  FILE res/graphics/lowDetail/heboblk_big.png res/graphics/lowDetail/heboblk_big.png
  FILE res/graphics/lowDetail/heboblk_old.png res/graphics/lowDetail/heboblk_old.png
  FILE res/graphics/lowDetail/heboblk_sp.png res/graphics/lowDetail/heboblk_sp.png
  FILE res/graphics/lowDetail/heboblk_sp2.png res/graphics/lowDetail/heboblk_sp2.png
  FILE res/graphics/lowDetail/heboblk0.png res/graphics/lowDetail/heboblk0.png
  FILE res/graphics/lowDetail/heboblk0B.png res/graphics/lowDetail/heboblk0B.png
  FILE res/graphics/lowDetail/heboblk1.png res/graphics/lowDetail/heboblk1.png
  FILE res/graphics/lowDetail/heboblk2.png res/graphics/lowDetail/heboblk2.png
  FILE res/graphics/lowDetail/heboblk3.png res/graphics/lowDetail/heboblk3.png
  FILE res/graphics/lowDetail/heboblk4_5.png res/graphics/lowDetail/heboblk4_5.png
  FILE res/graphics/lowDetail/heboblk6.png res/graphics/lowDetail/heboblk6.png
  FILE res/graphics/lowDetail/heboflb0.png res/graphics/lowDetail/heboflb0.png
  FILE res/graphics/lowDetail/heboflb1.png res/graphics/lowDetail/heboflb1.png
  FILE res/graphics/lowDetail/heboflb2.png res/graphics/lowDetail/heboflb2.png
  FILE res/graphics/lowDetail/heboflb3.png res/graphics/lowDetail/heboflb3.png
  FILE res/graphics/lowDetail/hebofld.png res/graphics/lowDetail/hebofld.png
  FILE res/graphics/lowDetail/hebofont.png res/graphics/lowDetail/hebofont.png
  FILE res/graphics/lowDetail/hebofont3.png res/graphics/lowDetail/hebofont3.png
  FILE res/graphics/lowDetail/hebofont4.png res/graphics/lowDetail/hebofont4.png
  FILE res/graphics/lowDetail/hebofont5.png res/graphics/lowDetail/hebofont5.png
  FILE res/graphics/lowDetail/heboris_road.png res/graphics/lowDetail/heboris_road.png
  FILE res/graphics/lowDetail/hebospr.png res/graphics/lowDetail/hebospr.png
  FILE res/graphics/lowDetail/item_guage.png res/graphics/lowDetail/item_guage.png
  FILE res/graphics/lowDetail/item.png res/graphics/lowDetail/item.png
  FILE res/graphics/lowDetail/itemerase.png res/graphics/lowDetail/itemerase.png
  FILE res/graphics/lowDetail/itemGra.png res/graphics/lowDetail/itemGra.png
  FILE res/graphics/lowDetail/laser.png res/graphics/lowDetail/laser.png
  FILE res/graphics/lowDetail/line.png res/graphics/lowDetail/line.png
  FILE res/graphics/lowDetail/loading.png res/graphics/lowDetail/loading.png
  FILE res/graphics/lowDetail/medal_ti.png res/graphics/lowDetail/medal_ti.png
  FILE res/graphics/lowDetail/medal.png res/graphics/lowDetail/medal.png
  FILE res/graphics/lowDetail/mirror_effect_TAP.png res/graphics/lowDetail/mirror_effect_TAP.png
  FILE res/graphics/lowDetail/perase1.png res/graphics/lowDetail/perase1.png
  FILE res/graphics/lowDetail/perase2.png res/graphics/lowDetail/perase2.png
  FILE res/graphics/lowDetail/perase3.png res/graphics/lowDetail/perase3.png
  FILE res/graphics/lowDetail/perase4.png res/graphics/lowDetail/perase4.png
  FILE res/graphics/lowDetail/perase5.png res/graphics/lowDetail/perase5.png
  FILE res/graphics/lowDetail/perase6.png res/graphics/lowDetail/perase6.png
  FILE res/graphics/lowDetail/perase7.png res/graphics/lowDetail/perase7.png
  FILE res/graphics/lowDetail/rollmark.png res/graphics/lowDetail/rollmark.png
  FILE res/graphics/lowDetail/rot.png res/graphics/lowDetail/rot.png
  FILE res/graphics/lowDetail/shootingstar.png res/graphics/lowDetail/shootingstar.png
  FILE res/graphics/lowDetail/shuffle_field_effect.png res/graphics/lowDetail/shuffle_field_effect.png
  FILE res/graphics/lowDetail/staffroll.png res/graphics/lowDetail/staffroll.png
  FILE res/graphics/lowDetail/text.png res/graphics/lowDetail/text.png
  FILE res/graphics/lowDetail/text2.png res/graphics/lowDetail/text2.png
  FILE res/graphics/lowDetail/title_old.png res/graphics/lowDetail/title_old.png
  FILE res/graphics/lowDetail/title.png res/graphics/lowDetail/title.png
  FILE res/graphics/lowDetail/tomoyo_eh_fade.png res/graphics/lowDetail/tomoyo_eh_fade.png

  FILE res/graphics/title/logo_hi.png res/graphics/lowDetail/logo_hi.png
  FILE res/graphics/title/logo_low.png res/graphics/lowDetail/logo_low.png
)
