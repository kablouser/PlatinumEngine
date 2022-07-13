/*
 * custom_mix_pitch.h
 *
 *  Created on: 10 jan 2018
 *      Author: Carlos Faruolo
 */

#ifndef CUSTOM_MIX_PITCH_H_
#define CUSTOM_MIX_PITCH_H_

#include <SDL_mixer.h>

/* Get chunk time length (in ms) given its size and current audio format */
int Custom_Mix_ComputeChunkLengthMillisec(int chunk_size);

/* Register a proper playback speed effect handler for this channel according to the current audio format. Effect valid for the current (or next) playback only. */
void Custom_Mix_RegisterPlaybackSpeedEffect(int channel, Mix_Chunk* chunk, float* speed, int loop, int self_halt);

#endif /* CUSTOM_MIX_PITCH_H_ */
