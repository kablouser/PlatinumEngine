#include "ComponentComposition/AudioComponent.h"

namespace PlatinumEngine
{
	AudioComponent::AudioComponent(std::string sample, bool loop):
		_sample(sample), isLooping(loop)
	{
		_isPaused = false;
		_isPlaying = false;
		_panning = 127;
		if(!AllocateChannel())
			PLATINUM_WARNING("Could not allocate a channel");
		LoadSample(_sample);
	}

	AudioComponent::~AudioComponent()
	{
		_allocatedChannel[_channel] = false;
		Mix_FreeChunk(_sound);
	}

	void AudioComponent::LoadSample(std::string sample)
	{
		_sample = sample;
		fileName = std::filesystem::path(_sample).filename().string();
		if(_sample=="")
		{
			PLATINUM_WARNING("Audio sample missing");
			return;
		}
		_sound = Mix_LoadWAV(_sample.c_str());
		if(_sound==nullptr)
		{
			std::string err = Mix_GetError();
			PLATINUM_ERROR("Chunk load error: " + err);
		}
	}

	void AudioComponent::Play()
	{
		if(_sound == nullptr)
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
			if (isLooping)
				Mix_PlayChannel(_channel, _sound, -1);
			else
				Mix_PlayChannel(_channel, _sound, 0);
		}
	}

	void AudioComponent::Pause()
	{
		if(_sound == nullptr)
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
		if(_sound == nullptr)
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
		if(_sound == nullptr)
			return;
		Mix_HaltChannel(_channel);
		_isPlaying = false;
		_isPaused = false;
	}

	void AudioComponent::SetVolume(int volume)
	{
		if(_sound!= nullptr)
			Mix_VolumeChunk(_sound, volume);
	}

	int AudioComponent::GetVolume()
	{
		if(_sound!= nullptr)
			return Mix_VolumeChunk(_sound, -1);
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

	bool AudioComponent::IsSampleExist()
	{
		if(_sound != nullptr)
			return true;
		return false;
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