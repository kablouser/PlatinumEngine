#pragma once

#define SDL_MAIN_HANDLED

#include "ComponentComposition/Component.h"
#include "SDL.h"
#include "SDL_mixer.h"

namespace PlatinumEngine
{
	class AudioComponent: public Component
	{
	public:
		enum class AudioType{clip,music};

	private:
		Mix_Music* _music;
		Mix_Chunk* _sound;
		std::string _sample;
		bool _isLooping;
		AudioType _audioType;
		int _channel;

	public:
		/**
		 * Initializes an audio component with a given sample and audio type and also specify if it should loop or not
		 * @param sample the audio sample to be played (preferably .wav)
		 * @param audioType whether the audio is short (sound effect) or long (music)
		 * @param loop whether the audio should loop when it is played (infinite looping)
		 */
		AudioComponent(std::string sample = "", AudioType audioType = AudioType::clip, bool loop = false);
		~AudioComponent();

		//Loads a sample
		void LoadSample(std::string sample, AudioType audioType = AudioType::clip);
		// Plays the sample
		void Play();
		// Pauses currently played sample
		void Pause();
		// Resumes currently paused sample
		void Resume();
		// Stops the sample from being played
		void Stop();

		// Sets volume (Range from 0-128)
		void SetVolume(int volume);
		// Sets whether sample will be looped
		void SetLoop(bool loop);

	};
}