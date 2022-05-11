#pragma once

#define SDL_MAIN_HANDLED

#include "ComponentComposition/Component.h"
#include "Logger/Logger.h"
#include <Audio/AudioClip.h>
#include "SDL.h"
#include "SDL_mixer.h"
#include <filesystem>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	class AudioComponent: public Component
	{
	public:
		std::string fileName;
		bool isLooping;
		SavedReference<AudioClip> audioClip;

	private:
		int _channel;
		int _panning;
		bool _isPlaying;
		bool _isPaused;
		static std::vector<bool> _allocatedChannel;

	public:
		AudioComponent();
		~AudioComponent();


		//PUBLIC FUNCTIONS

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

		void OnIDSystemUpdate() override;


		//STATIC FUNCTIONS

		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		// Returns whether audio is playing or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPlaying(int channel=-1);
		// Returns whether audio is paused or not (Can also specify a channel for audio clips otherwise it checks for all channels [value: -1])
		static bool IsPaused(int channel=-1);

	private:
		bool AllocateChannel();
	};
}