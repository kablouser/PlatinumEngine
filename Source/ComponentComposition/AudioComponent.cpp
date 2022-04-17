#include "ComponentComposition/AudioComponent.h"

namespace PlatinumEngine
{
	AudioComponent::AudioComponent(std::string sample)
	{
		//if( SDL_Init( SDL_INIT_AUDIO ) < 0 ) printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		_sample = sample;
	}
	AudioComponent::~AudioComponent()
	{
		SDL_CloseAudioDevice(_deviceId);
		SDL_FreeWAV(_wavBuffer);
		SDL_Quit();
	}

	void AudioComponent::Play()
	{
		SDL_LoadWAV(_sample.c_str(), &_wavSpec, &_wavBuffer, &_wavLength);
		_deviceId = SDL_OpenAudioDevice(NULL, 0, &_wavSpec, NULL, 0);
		int success = SDL_QueueAudio(_deviceId, _wavBuffer, _wavLength);
		SDL_PauseAudioDevice(_deviceId, 0);
	}

	void AudioComponent::Pause()
	{
		SDL_PauseAudioDevice(_deviceId, 1);
	}

	void AudioComponent::Stop()
	{
		SDL_CloseAudioDevice(_deviceId);
	}

	void AudioComponent::SetVolume(float volume)
	{
		//SDL_MixAudio(_wavBuffer,_wavBuffer,_wavLength, volume);
	}

	void AudioComponent::SetLoop(bool loop)
	{
		_isLooping = loop;
	}
}