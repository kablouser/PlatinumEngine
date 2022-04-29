#include "ComponentComposition/AudioComponent.h"
#include "Logger/Logger.h"

namespace PlatinumEngine
{
	AudioComponent::AudioComponent(std::string sample, AudioType type, bool loop)
	{
		_sample = sample;
		audioType = type;
		isLooping = loop;
		_isPaused = false;
		_isPlaying = false;
		fileName = std::filesystem::path(_sample).filename().string();

		if(audioType==AudioType::clip)
			_sound = Mix_LoadWAV(_sample.c_str());
		else
			_music = Mix_LoadMUS(_sample.c_str());
	}
	AudioComponent::~AudioComponent()
	{
		if(audioType==AudioType::clip)
		    Mix_FreeChunk(_sound);
		else
		    Mix_FreeMusic(_music);
	}

	void AudioComponent::LoadSample(std::string sample, AudioType type)
	{
		_sample = sample;
		audioType = type;
		fileName = std::filesystem::path(_sample).filename().string();
		if(audioType==AudioType::clip)
			_sound = Mix_LoadWAV(_sample.c_str());
		else
			_music = Mix_LoadMUS(_sample.c_str());
	}

	void AudioComponent::ReloadSample()
	{
		if(audioType==AudioType::clip)
			_sound = Mix_LoadWAV(_sample.c_str());
		else
			_music = Mix_LoadMUS(_sample.c_str());
	}

	void AudioComponent::Play()
	{
		if(!isPlaying(AudioType::clip,_channel))
		{
			_isPlaying = false;
			_isPaused = false;
		}
		if(_isPaused)
		{
			Resume();
			return;
		}
		if(!_isPlaying)
		{
			_channel = -2;
			_isPlaying = true;
			_isPaused = false;
			if (audioType == AudioType::clip)
			{
				if (isLooping)
					_channel = Mix_PlayChannel(-1, _sound, -1);
				else
					_channel = Mix_PlayChannel(-1, _sound, 0);
			}
			else
			{
				if (isLooping)
					_channel = Mix_PlayMusic(_music, -1);
				else
					_channel = Mix_PlayMusic(_music, 0);
			}
		}
	}

	void AudioComponent::Pause()
	{
		if(_isPlaying)
		{
			_isPlaying = false;
			_isPaused = true;
			if (audioType == AudioType::clip)
				Mix_Pause(_channel);
			else
				Mix_PauseMusic();
		}
	}

	void AudioComponent::Resume()
	{
		if(_isPaused)
		{
			_isPlaying = true;
			_isPaused = false;
			if (audioType == AudioType::clip)
				Mix_Resume(_channel);
			else
				Mix_ResumeMusic();
		}
	}

	void AudioComponent::Stop()
	{
		_isPlaying = false;
		_isPaused = false;
		if(audioType==AudioType::clip)
			Mix_HaltChannel(_channel);
		else
			Mix_HaltMusic();
	}

	void AudioComponent::SetVolume(int volume)
	{
		if(audioType==AudioType::clip)
			Mix_VolumeChunk(_sound, volume);
		else
			Mix_VolumeMusic(volume);
	}

	int AudioComponent::GetVolume()
	{
		if(audioType==AudioType::clip)
			return Mix_VolumeChunk(_sound, -1);
		else
			return Mix_VolumeMusic(-1);
	}

	bool AudioComponent::isPlaying(AudioType type, int channel)
	{
		if(type==AudioType::clip && Mix_Playing(channel) > 0)
			return true;
		else if(type==AudioType::music && Mix_PlayingMusic() > 0)
			return true;
		return false;
	}

	bool AudioComponent::isPaused(AudioType type, int channel)
	{
		if(type==AudioType::clip && Mix_Paused(channel) > 0)
			return true;
		else if(type==AudioType::music && Mix_PausedMusic() > 0)
			return true;
		return false;
	}
}