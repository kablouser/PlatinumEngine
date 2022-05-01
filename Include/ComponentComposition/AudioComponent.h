#pragma once

#include "ComponentComposition/Component.h"
#include "Logger/Logger.h"
#include "SDL.h"
#include "SDL_mixer.h"
#include <filesystem>

namespace PlatinumEngine
{
	class AudioComponent: public Component
	{
	public:
		enum class AudioType{clip,music};
		std::string fileName;
		bool isLooping;
		AudioType audioType;

	private:
		Mix_Music* _music;
		Mix_Chunk* _sound;
		std::string _sample;
		int _channel;
		bool _isPlaying, _isPaused;

	public:
		/**
		 * Initializes an audio component with a given sample and audio type and also specify if it should loop or not
		 * @param sample the audio sample to be played (preferably .wav)
		 * @param type whether the audio is wave type(clip) or music (typically .mp3)
		 * @param loop whether the audio should loop when it is played (infinite looping)
		 */
		AudioComponent(std::string sample = "", AudioType type = AudioType::clip, bool loop = false);
		~AudioComponent();

		//Loads a sample
		void LoadSample(std::string sample, AudioType type = AudioType::clip);
		//Reloads a sample (incase we change the audiotype)
		void ReloadSample();
		// Plays the sample (Also resumes paused sample)
		void Play();
		// Pauses currently played sample
		void Pause();
		// Resumes currently paused sample
		void Resume();
		// Stops the sample from being played
		void Stop();
		// Sets volume (Range from 0-128)
		void SetVolume(int volume);
		// Get the current volume
		int GetVolume();
		// Returns whether a sample has been loaded or not
		bool IsSampleExist();

		//STATIC FUNCTIONS

		// Returns whether audio is playing or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPlaying(AudioType type=AudioType::clip, int channel=-1);
		// Returns whether audio is paused or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPaused(AudioType type=AudioType::clip, int channel=-1);

	};
}