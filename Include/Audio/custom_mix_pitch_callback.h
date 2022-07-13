/*
 * custom_mix_pitch_callback.h
 *
 *  Created on: 10 de jan de 2018
 *      Author: carlosfaruolo
 */

// Mix_EffectFunc_t callback that redirects to handler method (handler passed via user_data)
// Processing function to be able to change chunk speed/pitch.
// AUDIO_FORMAT_TYPE depends on the current audio format (queryable via Mix_QuerySpec)
void Custom_Mix_PlaybackSpeedEffectFuncCallback(int mix_channel, void* stream, int length, void* user_data)
{
	int audio_frequency, audio_channel_count;
	Mix_QuerySpec(&audio_frequency, NULL, &audio_channel_count);

	Custom_Mix_PlaybackSpeedEffectHandler* handler = (Custom_Mix_PlaybackSpeedEffectHandler*) user_data;
	const AUDIO_FORMAT_TYPE* chunk_data = (AUDIO_FORMAT_TYPE*) handler->chunk->abuf;

	AUDIO_FORMAT_TYPE* buffer = (AUDIO_FORMAT_TYPE*) stream;
	const int buffer_size = length / sizeof(AUDIO_FORMAT_TYPE);  // buffer size (as array)
	const float speed_factor = *(handler->speed);  // take a "snapshot" of speed factor

	// if there is still sound to be played
	if(handler->position < handler->duration || handler->loop)
	{
		const float delta = 1000.0 / audio_frequency,  // normal duration of each sample
			    vdelta = delta * speed_factor;  // virtual stretched duration, scaled by 'speedFactor'
		
		// if playback is unaltered and pitch is required (for the first time)
		if(!handler->altered && speed_factor != 1.0f)
			handler->altered = 1;  // flags playback modification and proceed to the pitch routine

		if(handler->altered)  // if unaltered, this pitch routine is skipped
		{
			for(int i = 0; i < buffer_size; i += audio_channel_count)
			{
				const int j = i / audio_channel_count;  // j goes from 0 to size/channelCount, incremented 1 by 1
				const float x = handler->position + j * vdelta;  // get "virtual" index. its corresponding value will be interpolated.
				const int k = floor(x / delta);  // get left index to interpolate from original chunk data (right index will be this plus 1)
				const float prop = (x / delta) - k;  // get the proportion of the right value (left will be 1.0 minus this)
				// const float prop2 = prop * prop;  // cache the square of the proportion (needed only for cubic interpolation)
				
				// usually just 2 channels: 0 (left) and 1 (right), but who knows...
				for(int c = 0; c < audio_channel_count; c++)
				{
					// check if k will be within bounds
					if(k * audio_channel_count + audio_channel_count - 1 < handler->chunk_size || handler->loop)
					{
						AUDIO_FORMAT_TYPE v0 = chunk_data[(  k   * audio_channel_count + c) % handler->chunk_size],
								  // v_ = chunk_data[((k-1) * audio_channel_count + c) % handler->chunk_size],
								  // v2 = chunk_data[((k+2) * audio_channel_count + c) % handler->chunk_size],                  
								  v1 = chunk_data[((k+1) * audio_channel_count + c) % handler->chunk_size];

						// put interpolated value on 'data'
						// buffer[i + c] = (1 - prop) * v0 + prop * v1;  // linear interpolation
						buffer[i + c] = v0 + prop * (v1 - v0);  // linear interpolation (single-multiplication version)
						// buffer[i + c] = v0 + 0.5f * prop * ((prop - 3) * v0 - (prop - 2) * 2 * v1 + (prop - 1) * v2);  // quadratic interpolation
						// buffer[i + c] = v0 + (prop / 6) * ((3 * prop - prop2 - 2) * v_ + (prop2 - 2 * prop - 1) * 3 * v0 + (prop - prop2 + 2) * 3 * v1 + (prop2 - 1) * v2);  // cubic interpolation
						// buffer[i + c] = v0 + 0.5f * prop * ((2 * prop2 - 3 * prop - 1) * (v0 - v1) + (prop2 - 2 * prop + 1) * (v0 - v_) + (prop2 - prop) * (v2 - v2));  // cubic spline interpolation
					}
					else  // if k will be out of bounds (chunk bounds), it means we already finished; thus, we'll pass silence
					{
						buffer[i + c] = 0;
					}
				}
			}
		}

		// update position
		handler->position += (buffer_size / audio_channel_count) * vdelta;

		// reset position if looping
		if(handler->loop) while(handler->position > handler->duration)
			handler->position -= handler->duration;
	}
	else  // if we already played the whole sound but finished earlier than expected by SDL_mixer (due to faster playback speed)
	{
		// set silence on the buffer since Mix_HaltChannel() poops out some of it for a few ms.
		for(int i = 0; i < buffer_size; i++)
			buffer[i] = 0;

		if(handler->self_halt)
			Mix_HaltChannel(mix_channel);  // XXX unsafe call, since it locks audio; but no safer solution was found yet...
	}
}
