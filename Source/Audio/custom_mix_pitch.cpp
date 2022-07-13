/*
 * custom_mix_pitch.c
 *
 *  Created on: 10 jan 2018
 *      Author: Carlos Faruolo
 */

#include "Audio/custom_mix_pitch.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static Uint16 format_sample_size(Uint16 format) { return (format & 0xFF) / 8; }

/* Get chunk time length (in ms) given its size and current audio format */
int Custom_Mix_ComputeChunkLengthMillisec(int chunkSize)
{
	Uint16 fmt;
	int freq, chancnt;
	Mix_QuerySpec(&freq, &fmt, &chancnt);

	/* bytes / samplesize == sample points */
	const Uint32 points = chunkSize / format_sample_size(fmt);

	/* sample points / channels == sample frames */
	const Uint32 frames = (points / chancnt);

	/* (sample frames * 1000) / frequency == play length, in ms */
	return ((frames * 1000) / freq);
}

/* custom handler object to control which part of the Mix_Chunk's audio data will be played, with which pitch-related modifications. */
typedef struct Custom_Mix_PlaybackSpeedEffectHandler
{
	const Mix_Chunk* chunk;
	const float* speed;  /* ptr to the desired playback speed */
	float position;  /* current position of the sound, in ms */
	int altered;   /* false if this playback has never been pitched. */

	// read-only!
	int loop;       /* whether this is a looped playback */
	int duration;   /* the duration of the sound, in ms */
	int chunk_size;  /* the size of the sound, as a number of indexes (or sample points). thinks of this as a array size when using the proper array type (instead of just Uint8*). */
	int self_halt;   /* flags whether playback should be halted by this callback when playback is finished */
} Custom_Mix_PlaybackSpeedEffectHandler;

/* "Constructor" for Custom_Mix_PlaybackSpeedEffectHandler */
Custom_Mix_PlaybackSpeedEffectHandler* Custom_Mix_CreatePlaybackSpeedEffectHandler(const Mix_Chunk* chunk, const float* speed, int loop, int self_halt)
{
	Uint16 fmt;
	Mix_QuerySpec(NULL, &fmt, NULL);

	Custom_Mix_PlaybackSpeedEffectHandler* handler = (Custom_Mix_PlaybackSpeedEffectHandler*)malloc(sizeof(Custom_Mix_PlaybackSpeedEffectHandler));
	handler->chunk = chunk;
	handler->speed = speed;
	handler->position = 0;
	handler->altered = 0;
	handler->loop = loop;
	handler->duration = Custom_Mix_ComputeChunkLengthMillisec(chunk->alen);
	handler->chunk_size = chunk->alen / format_sample_size(fmt);
	handler->self_halt = self_halt;
	return handler;
}

/* implementation of Uint8 version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackUint8
#define AUDIO_FORMAT_TYPE Uint8
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* implementation of Sint8 version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint8
#define AUDIO_FORMAT_TYPE Sint8
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* implementation of Uint16 version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackUint16
#define AUDIO_FORMAT_TYPE Uint16
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* implementation of Sint16 version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint16
#define AUDIO_FORMAT_TYPE Sint16
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* implementation of Sint32 version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackSint32
#define AUDIO_FORMAT_TYPE Sint32
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* implementation of Float version of the callback */
#define Custom_Mix_PlaybackSpeedEffectFuncCallback Custom_Mix_PlaybackSpeedEffectFuncCallbackFloat
#define AUDIO_FORMAT_TYPE float
#include "Audio/custom_mix_pitch_callback.h"
#undef Custom_Mix_PlaybackSpeedEffectFuncCallback
#undef AUDIO_FORMAT_TYPE

/* Mix_EffectDone_t callback that deletes the handler at the end of the effect usage  (handler passed via userData) */
void Custom_Mix_PlaybackSpeedEffectDoneCallback(int channel, void *userData)
{
	free(userData);
}

/* Register a proper playback speed effect handler for this channel according to the current audio format. Effect valid for the current (or next) playback only. */
void Custom_Mix_RegisterPlaybackSpeedEffect(int channel, Mix_Chunk* chunk, float* speed, int loop, int self_halt)
{
	Uint16 fmt;
	Mix_QuerySpec(NULL, &fmt, NULL);

	Mix_EffectFunc_t effect_func_callback;

	/* select the register function for the current audio format and register the effect using the compatible handlers
	   xxx is it correct to behave the same way to all S16 and U16 formats? Should we create case statements for AUDIO_S16SYS, AUDIO_S16LSB, AUDIO_S16MSB, etc, individually? */
	switch(fmt)
	{
		case AUDIO_U8:  effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackUint8;  break;
		case AUDIO_S8:  effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint8;  break;
		case AUDIO_U16: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackUint16; break;
		default:
		case AUDIO_S16: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint16; break;
		case AUDIO_S32: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackSint32; break;
		case AUDIO_F32: effect_func_callback = Custom_Mix_PlaybackSpeedEffectFuncCallbackFloat;  break;
	}

	Mix_RegisterEffect(channel, effect_func_callback, Custom_Mix_PlaybackSpeedEffectDoneCallback, Custom_Mix_CreatePlaybackSpeedEffectHandler(chunk, speed, loop, self_halt));
}
