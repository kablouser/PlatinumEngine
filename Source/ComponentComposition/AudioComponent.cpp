#include <ComponentComposition/AudioComponent.h>
#include <SceneManager/Scene.h>
#include <Application.h>

namespace PlatinumEngine
{
	AudioComponent::AudioComponent():
		isLooping(false), _isPaused(false), _isPlaying(false), _panning(127)
	{
		if(!AllocateChannel())
			PLATINUM_WARNING("Could not allocate a channel");
	}

	AudioComponent::~AudioComponent()
	{
		_allocatedChannel[_channel] = false;
	}

	void AudioComponent::Play()
	{
		if (!audioClip)
			return;
		if(!IsPlaying(_channel))
		{
			Stop();
		}
		if(_isPaused)
		{
			Resume();
			return;
		}
		if(!_isPlaying)
		{
			_isPlaying = true;
			_isPaused = false;
			Mix_PlayChannel(_channel, audioClip.DeRef()->chunk, isLooping ? -1 : 0);
		}
	}

	void AudioComponent::Pause()
	{
		if (!audioClip)
			return;
		if(_isPlaying)
		{
			_isPlaying = false;
			_isPaused = true;
			Mix_Pause(_channel);
		}
	}

	void AudioComponent::Resume()
	{
		if (!audioClip)
			return;
		if(_isPaused)
		{
			_isPlaying = true;
			_isPaused = false;
			Mix_Resume(_channel);
		}
	}

	void AudioComponent::Stop()
	{
		if (!audioClip)
			return;
		Mix_HaltChannel(_channel);
		_isPlaying = false;
		_isPaused = false;
	}

	void AudioComponent::SetVolume(int volume)
	{
		if(audioClip)
			Mix_Volume(_channel, volume);
	}

	int AudioComponent::GetVolume()
	{
		if(audioClip)
			return Mix_Volume(_channel, -1);
		return MIX_MAX_VOLUME;
	}

	void AudioComponent::SetPanning(int panValueRight)
	{
		if(_channel>=0)
		{
			_panning = panValueRight;
			Mix_SetPanning(_channel, 254-_panning, _panning);
		}
	}

	int AudioComponent::GetPanning()
	{
		return _panning;
	}

	int AudioComponent::GetChannel()
	{
		return _channel;
	}

	void AudioComponent::OnIDSystemUpdate()
	{
		audioClip.OnIDSystemUpdate(Application::Instance->idSystem);
	}

	void AudioComponent::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		typeDatabase.BeginTypeInfo<AudioComponent>()
				.WithInherit<Component>()
				.WithField<SavedReference<AudioClip>>("audioClip", PLATINUM_OFFSETOF(AudioComponent, audioClip))
				.WithField<bool>("isLooping", PLATINUM_OFFSETOF(AudioComponent, isLooping))
				.WithField<int>("_channel", PLATINUM_OFFSETOF(AudioComponent, _channel))
				.WithField<int>("_panning", PLATINUM_OFFSETOF(AudioComponent, _panning))
				.WithField<bool>("_isPlaying", PLATINUM_OFFSETOF(AudioComponent, _isPlaying))
				.WithField<bool>("_isPaused", PLATINUM_OFFSETOF(AudioComponent, _isPaused));
	}

	bool AudioComponent::IsPlaying(int channel)
	{
		if(Mix_Playing(channel) > 0)
			return true;
		return false;
	}

	bool AudioComponent::IsPaused(int channel)
	{
		if(Mix_Paused(channel) > 0)
			return true;
		return false;
	}

	bool AudioComponent::AllocateChannel()
	{
		if(_allocatedChannel.size()<Mix_AllocateChannels(-1))
			_allocatedChannel.resize(Mix_AllocateChannels(-1));
		for(int i=0;i<_allocatedChannel.size();i++)
		{
			if(!_allocatedChannel[i])
			{
				_channel = i;
				_allocatedChannel[i] = true;
				return true;
			}
		}
		_channel = -2;
		return false;
	}

	std::vector<bool> AudioComponent::_allocatedChannel = std::vector<bool>(Mix_AllocateChannels(-1));

}