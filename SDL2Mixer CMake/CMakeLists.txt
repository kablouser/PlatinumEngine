cmake_minimum_required(VERSION 3.1.0)
project(SDL2_mixer C)

# Modified simplified version of Github's CMakeLists.txt on https://github.com/libsdl-org/SDL_mixer
# We don't need other formats apart from .wav
# Also, we 'trust' that SDL2main and SDL2-static are present

set(SDL2_MIXER_SRC_DIR ${CMAKE_BINARY_DIR}/_deps/sdl2mixer-src)

include_directories(include ${SDL2_MIXER_SRC_DIR})

add_library(SDL2_mixer)

target_sources(SDL2_mixer PRIVATE
	${SDL2_MIXER_SRC_DIR}/effect_position.c
	${SDL2_MIXER_SRC_DIR}/effects_internal.c
	${SDL2_MIXER_SRC_DIR}/effect_stereoreverse.c
	${SDL2_MIXER_SRC_DIR}/mixer.c
	${SDL2_MIXER_SRC_DIR}/music.c
	#${SDL2_MIXER_SRC_DIR}/utils.c
	${SDL2_MIXER_SRC_DIR}/load_aiff.c
	${SDL2_MIXER_SRC_DIR}/load_voc.c
	${SDL2_MIXER_SRC_DIR}/music_cmd.c
	${SDL2_MIXER_SRC_DIR}/music_wav.c
	${SDL2_MIXER_SRC_DIR}/music_flac.c
	${SDL2_MIXER_SRC_DIR}/music_mad.c
	${SDL2_MIXER_SRC_DIR}/music_mpg123.c
	${SDL2_MIXER_SRC_DIR}/music_ogg.c
	${SDL2_MIXER_SRC_DIR}/music_opus.c
	${SDL2_MIXER_SRC_DIR}/music_mikmod.c
	${SDL2_MIXER_SRC_DIR}/music_modplug.c
	#${SDL2_MIXER_SRC_DIR}/music_xmp.c
	${SDL2_MIXER_SRC_DIR}/music_fluidsynth.c
	${SDL2_MIXER_SRC_DIR}/music_timidity.c
	${SDL2_MIXER_SRC_DIR}/music_nativemidi.c)

target_compile_definitions(SDL2_mixer PRIVATE -DMUSIC_WAV)
target_include_directories(SDL2_mixer PUBLIC include)
target_link_libraries(SDL2_mixer PRIVATE SDL2main SDL2-static)
