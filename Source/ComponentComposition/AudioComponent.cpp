#include <ComponentComposition/AudioComponent.h>
#include <SceneManager/Scene.h>
#include <Application.h>

namespace PlatinumEngine
{
	void AudioComponent::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<AudioComponent>()
				.WithInherit<Component>()
				.WithField<SavedReference<AudioClip>>("audioClip", PLATINUM_OFFSETOF(AudioComponent, audioClip))
				.WithField<bool>("_isLooping", PLATINUM_OFFSETOF(AudioComponent, _isLooping))
				.WithField<int>("_channel", PLATINUM_OFFSETOF(AudioComponent, _channel))
				.WithField<int>("_volume", PLATINUM_OFFSETOF(AudioComponent, _volume));
	}

	AudioComponent::AudioComponent() : _isLooping(false), _channel(-1)
	{
	}

	void AudioComponent::Play()
	{
		if (!audioClip)
			return;

		_channel = Mix_PlayChannel(-1, audioClip.DeRef()->chunk, _isLooping ? -1 : 0);
		Mix_Volume(_channel, _volume);
	}

	void AudioComponent::Pause()
	{
		if (_channel == -1)
			return;
		Mix_Pause(_channel);
	}

	void AudioComponent::Resume()
	{
		if (_channel == -1)
			return;
		Mix_Resume(_channel);
	}

	void AudioComponent::Stop()
	{
		if (_channel == -1)
			return;
		Mix_HaltChannel(_channel);
	}

	void AudioComponent::SetVolume(int volume)
	{
		_volume = volume;
	}

	void AudioComponent::SetLoop(bool loop)
	{
		_isLooping = loop;
	}

	void AudioComponent::OnIDSystemUpdate()
	{
		audioClip.OnIDSystemUpdate(Application::Instance->idSystem);
	}
}