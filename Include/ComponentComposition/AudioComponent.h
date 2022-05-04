#pragma once

#include <ComponentComposition/Component.h>
#include <Audio/AudioClip.h>

namespace PlatinumEngine
{
	class AudioComponent: public Component
	{
	public:
		static void CreateTypeInfo(TypeDatabase& typeDatabase);

		SavedReference<AudioClip> audioClip;

	private:
		bool _isLooping;
		int _channel;
		int _volume;

	public:

		AudioComponent();

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

		void OnIDSystemUpdate(Scene& scene) override;
	};
}