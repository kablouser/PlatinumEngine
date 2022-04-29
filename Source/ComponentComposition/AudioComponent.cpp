#include "ComponentComposition/AudioComponent.h"

namespace PlatinumEngine
{
	AudioComponent::AudioComponent(std::string sample, AudioType audioType, bool loop)
	{
		//SDL Initialization to be done here?
		//if(SDL_Init(SDL_INIT_AUDIO)<0) printf("SDL could not initialize! SDL Error: %s\n",SDL_GetError());
		//if(Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,1024)<0) printf("Failed to open audio! Mix Error: %s\n",Mix_GetError());

		_sample = sample;
		_audioType = audioType;
		_isLooping = loop;
		fileName = std::filesystem::path(_sample).filename().string();

		if(_audioType==AudioType::clip)
			_sound = Mix_LoadWAV(_sample.c_str());
		else
			_music = Mix_LoadMUS(_sample.c_str());
	}
	AudioComponent::~AudioComponent()
	{
		//Should Quit be called here?
		//Mix_Quit();
		//SDL_Quit();
		if(_audioType==AudioType::clip)
		    Mix_FreeChunk(_sound);
		else
		    Mix_FreeMusic(_music);
	}

	void AudioComponent::LoadSample(std::string sample, AudioType audioType)
	{
		_sample = sample;
		_audioType = audioType;
		fileName = std::filesystem::path(_sample).filename().string();
		if(_audioType==AudioType::clip)
			_sound = Mix_LoadWAV(_sample.c_str());
		else
			_music = Mix_LoadMUS(_sample.c_str());
	}

	void AudioComponent::Play()
	{
		_channel = -1;
		if(_audioType==AudioType::clip)
		{
			if(_isLooping)
				_channel = Mix_PlayChannel(-1, _sound, -1);
			else
				_channel = Mix_PlayChannel(-1, _sound, 0);
		}
		else
		{
			if(_isLooping)
				_channel = Mix_PlayMusic( _music, -1);
			else
				_channel = Mix_PlayMusic( _music, 0);
		}
	}

	void AudioComponent::Pause()
	{
		if(_audioType==AudioType::clip)
			Mix_Pause(_channel);
		else
			Mix_PauseMusic();
	}

	void AudioComponent::Resume()
	{
		if(_audioType==AudioType::clip)
			Mix_Resume(_channel);
		else
			Mix_ResumeMusic();
	}

	void AudioComponent::Stop()
	{
		if(_audioType==AudioType::clip)
			Mix_HaltChannel(_channel);
		else
			Mix_HaltMusic();
	}

	void AudioComponent::SetVolume(int volume)
	{
		if(_audioType==AudioType::clip)
			Mix_Volume(_channel, volume);
		else
			Mix_VolumeMusic(volume);
	}

	void AudioComponent::SetLoop(bool loop)
	{
		_isLooping = loop;
	}
}