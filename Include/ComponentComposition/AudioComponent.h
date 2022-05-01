#pragma once

#define SDL_MAIN_HANDLED

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
		std::string fileName;
		bool isLooping;

	private:
		Mix_Chunk* _sound;
		std::string _sample;
		int _channel;
		int _panning;
		bool _isPlaying;
		bool _isPaused;
		static std::vector<bool> _allocatedChannel;

	public:
		/**
		 * Initializes an audio component with a given sample and audio type and also specify if it should loop or not
		 * @param sample the audio sample to be played (preferably .wav)
		 * @param type whether the audio is wave type(clip) or music (typically .mp3)
		 * @param loop whether the audio should loop when it is played (infinite looping)
		 */
		AudioComponent(std::string sample = "", bool loop = false);
		~AudioComponent();


		//PUBLIC FUNCTIONS

		//Loads a sample
		void LoadSample(std::string sample);
		// Plays the sample (Also resumes paused sample)
		void Play();
		// Pauses currently played sample
		void Pause();
		// Resumes currently paused sample
		void Resume();
		// Stops the sample from being played (Rewinds back to the start)
		void Stop();
		// Sets volume (Range from 0-128)
		void SetVolume(int volume);
		// Get the current volume
		int GetVolume();
		// Sets the panning value of the audio (Stereo only)(Range from 0-255)
		void SetPanning(int panValueRight);
		// Returns the panning value
		int GetPanning();
		// Returns the allocated channel
		int GetChannel();
		// Returns whether a sample has been loaded or not
		bool IsSampleExist();


		//STATIC FUNCTIONS

		// Returns whether audio is playing or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPlaying(int channel=-1);
		// Returns whether audio is paused or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPaused(int channel=-1);

	private:
		bool AllocateChannel();
	};
}