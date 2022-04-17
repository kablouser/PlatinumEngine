#pragma once

#define SDL_MAIN_HANDLED

#include "ComponentComposition/Component.h"
#include "SDL.h"
#include "SDL_audio.h"

namespace PlatinumEngine
{
	class AudioComponent: public Component
	{
	private:
		std::string _sample;
		SDL_AudioDeviceID _deviceId;
		Uint8* _wavBuffer;
		SDL_AudioSpec _wavSpec;
		Uint32 _wavLength;
		bool _isLooping;

	public:
		enum class AudioType{clip,music};
	public:
		AudioComponent(std::string sample);
		~AudioComponent();
		void Play();
		void Pause();
		void Stop();
		void SetVolume(float volume);
		void SetLoop(bool loop);

	};
}