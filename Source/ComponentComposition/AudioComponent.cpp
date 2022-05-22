#include <ComponentComposition/AudioComponent.h>
#include <SceneManager/Scene.h>
#include <Application.h>

namespace PlatinumEngine
{
	AudioComponent::AudioComponent():
		isLooping(false), _isPaused(false), _isPlaying(false), _panning(127), isFilterEnabled(false), playbackSpeed(1.f), isPlaybackShiftEnabled(false), isDistortEnabled(false)
	{
		if(!AllocateChannel())
			PLATINUM_WARNING("Could not allocate a channel");
		_filterInfo.filter = nullptr;
		_filterInfo.type = "None";
		std::memset(_filterInfo.userdata, 0, sizeof(_filterInfo.userdata));
		_distortInfo.previousStreamLength = 0;
		_distortInfo.mixDistortDryValue = 1.f;
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
			RegisterEffects();
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
		Mix_UnregisterAllEffects(_channel);
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

	void AudioComponent::SetFilterParams(float params[8])
	{
		if(_filterInfo.filter==nullptr)
			return;
		for(int i=0;i<_filterInfo.filter->getNumParams();i++)
			_filterInfo.userdata[i]=params[i];
	}

	void AudioComponent::GetFilterParamsInfo(int &numParams, char* (&paramNames)[8], float (&paramValues)[8])
	{
		if(_filterInfo.filter==nullptr)
			return;
		numParams = _filterInfo.filter->getNumParams();
		for(int i=0;i<numParams;i++)
		{
			paramNames[i] = (char*)_filterInfo.filter->getParamInfo(i).getName();
			paramValues[i] = (float)_filterInfo.userdata[i];
		}

	}

	void AudioComponent::SetFilterType(int type)
	{
		switch (type)
		{
			case 1: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::LowPass, 2> (); break;
			case 2: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::HighPass, 2> (); break;
			case 3: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::BandPass1, 2> (); break;
			case 4: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::BandPass2, 2> (); break;
			case 5: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::BandStop, 2> (); break;
			case 6: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::LowShelf, 2> (); break;
			case 7: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::HighShelf, 2> (); break;
			case 8: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::BandShelf, 2> (); break;
			case 9: _filterInfo.filter = new Dsp::FilterDesign<Dsp::RBJ::Design::AllPass, 2> (); break;
			case 10: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::LowPass<2>, 2> (); break;
			case 11: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::HighPass<2>, 2> (); break;
			case 12: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::BandPass<2>, 2> (); break;
			case 13: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::BandStop<2>, 2> (); break;
			case 14: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::LowShelf<2>, 2> (); break;
			case 15: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::HighShelf<2>, 2> (); break;
			case 16: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Butterworth::Design::BandShelf<2>, 2> (); break;
			case 17: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::LowPass<2>, 2> (); break;
			case 18: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::HighPass<2>, 2> (); break;
			case 19: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::BandPass<2>, 2> (); break;
			case 20: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::BandStop<2>, 2> (); break;
			case 21: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::LowShelf<2>, 2> (); break;
			case 22: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::HighShelf<2>, 2> (); break;
			case 23: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevI::Design::BandShelf<2>, 2> (); break;
			case 24: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowPass<2>, 2> (); break;
			case 25: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::HighPass<2>, 2> (); break;
			case 26: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::BandPass<2>, 2> (); break;
			case 27: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::BandStop<2>, 2> (); break;
			case 28: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::LowShelf<2>, 2> (); break;
			case 29: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::HighShelf<2>, 2> (); break;
			case 30: _filterInfo.filter = new Dsp::FilterDesign<Dsp::ChebyshevII::Design::BandShelf<2>, 2> (); break;
			case 31: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Elliptic::Design::LowPass<2>, 2> (); break;
			case 32: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Elliptic::Design::HighPass<2>, 2> (); break;
			case 33: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Elliptic::Design::BandPass<2>, 2> (); break;
			case 34: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Elliptic::Design::BandStop<2>, 2> (); break;
			case 35: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Legendre::Design::LowPass<2>, 2> (); break;
			case 36: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Legendre::Design::HighPass<2>, 2> (); break;
			case 37: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Legendre::Design::BandPass<2>, 2> (); break;
			case 38: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Legendre::Design::BandStop<2>, 2> (); break;
			case 39: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Bessel::Design::LowPass<2>, 2> (); break;
			case 40: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Bessel::Design::HighPass<2>, 2> (); break;
			case 41: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Bessel::Design::BandPass<2>, 2> (); break;
			case 42: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Bessel::Design::BandStop<2>, 2> (); break;
			case 43: _filterInfo.filter = new Dsp::FilterDesign<Dsp::Bessel::Design::LowShelf<2>, 2> (); break;
			default: _filterInfo.filter = nullptr;
		}
		_filterInfo.type = filterTypes[type].c_str();
	}

	void AudioComponent::FilterEffect(int channel, void* stream, int length, void* userData)
	{
		filterData* userParam = static_cast<filterData*>(userData);

		//Get relevant information from user
		Dsp::Filter* filter = userParam->filter;
		const char* type = userParam->type;
		float* filterParam = userParam->userdata;

		//Stream related operations
		const std::size_t buffer_length = length/sizeof(Sint16);
		Sint16* buffer = static_cast<Sint16*>(stream);
		int numSamples = buffer_length/2;

		//Getting parameters for filter to apply on buffer
		Dsp::Params params;
		for(int i=0;i<filter->getNumParams();i++)
			params[i] = filterParam[i];

		//Left and right channels
		double* db[2];
		db[0] = new double[numSamples];
		db[1] = new double[numSamples];

		for(int i=0;i<numSamples;i++)
		{
			db[0][i]=(double)buffer[i*2];
			db[1][i]=(double)buffer[i*2+1];
		}

		filter->setParams (params);
		filter->process (numSamples, db);

		for(int i=0;i<numSamples;i++)
		{
			buffer[i*2]  =(int)db[0][i];
			buffer[i*2+1]=(int)db[1][i];
		}
	}

	void AudioComponent::SetDistortDryValue(float value)
	{
		_distortInfo.mixDistortDryValue = value;
	}

	float AudioComponent::GetDistortDryValue()
	{
		return _distortInfo.mixDistortDryValue;
	}

	void AudioComponent::DistortEffect(int channel, void* stream, int length, void* userData)
	{
		distortData* userParam = static_cast<distortData*>(userData);

		Sint16* previousStream = userParam->previousStream;
		int previousStreamLength = userParam->previousStreamLength;
		float mixDryValue = userParam->mixDistortDryValue;
		float mixWetValue = 1.f - mixDryValue;

		//Stream related operations
		const std::size_t buffer_length = length/sizeof(Sint16);
		Sint16* buffer = static_cast<Sint16*>(stream);
		int numSamples = buffer_length/2;

		Sint16 buffer_copy[buffer_length];
		memcpy(buffer_copy, buffer, buffer_length);

		int offset = numSamples/2;
		for(int i=0; i<numSamples; i+=2)
		{
			if(i<offset)
			{
				if(previousStreamLength/2 == numSamples)
				{
					buffer_copy[i]  =(Sint16) (buffer[i]    *mixDryValue + previousStream[numSamples+i-offset]  *mixWetValue);
					buffer_copy[i+1]=(Sint16) (buffer[i + 1]*mixDryValue + previousStream[numSamples+i-offset+1]*mixWetValue);
				}
			}
			else
			{
				buffer_copy[i]  =(Sint16) (buffer[i]  *mixDryValue + buffer[i-offset]  *mixWetValue);
				buffer_copy[i+1]=(Sint16) (buffer[i+1]*mixDryValue + buffer[i-offset+1]*mixWetValue);
			}
		}

		if(previousStreamLength!=buffer_length)
			userParam->previousStream = new Sint16[buffer_length];

		memcpy(userParam->previousStream, buffer, buffer_length);
		memcpy(buffer, buffer_copy, buffer_length);

		userParam->previousStreamLength = buffer_length;

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
				.WithField<bool>("_isPaused", PLATINUM_OFFSETOF(AudioComponent, _isPaused))
				.WithField<bool>("isDistortEnabled", PLATINUM_OFFSETOF(AudioComponent, isDistortEnabled))
				.WithField<bool>("isFilterEnabled", PLATINUM_OFFSETOF(AudioComponent, isFilterEnabled))
				.WithField<bool>("isPlaybackShiftEnabled", PLATINUM_OFFSETOF(AudioComponent, isPlaybackShiftEnabled))
				.WithField<float>("playbackSpeed", PLATINUM_OFFSETOF(AudioComponent, playbackSpeed))
				.WithField<filterData>("_filterInfo", PLATINUM_OFFSETOF(AudioComponent, _filterInfo))
				.WithField<distortData>("_distortInfo", PLATINUM_OFFSETOF(AudioComponent, _distortInfo));
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

	void AudioComponent::RegisterEffects()
	{
		if(isPlaybackShiftEnabled)
			Custom_Mix_RegisterPlaybackSpeedEffect(_channel, audioClip.DeRef()->chunk, &playbackSpeed, isLooping, 0);
		if(isDistortEnabled)
			Mix_RegisterEffect(_channel, DistortEffect, NULL, &_distortInfo);
		if(isFilterEnabled && (std::string(_filterInfo.type).compare("None")!=0 && _filterInfo.filter!=nullptr))
			Mix_RegisterEffect(_channel, FilterEffect, NULL, &_filterInfo);

	}

	std::vector<bool> AudioComponent::_allocatedChannel = std::vector<bool>(Mix_AllocateChannels(-1));
	std::vector<std::string> AudioComponent::filterTypes ={ "None",
		"RBJLowPass", "RBJHighPass", "RBJBandPass1", "RBJBandPass2", "RBJBandStop", "RBJLowShelf", "RBJHighShelf", "RBJBandShelf", "RBJAllPass",
		"ButterworthLowPass", "ButterworthHighPass", "ButterworthBandPass", "ButterworthBandStop", "ButterworthLowShelf", "ButterworthHighShelf", "ButterworthBandShelf",
		"ChebyshevILowPass", "ChebyshevIHighPass", "ChebyshevIBandPass", "ChebyshevIBandStop", "ChebyshevILowShelf", "ChebyshevIHighShelf", "ChebyshevIBandShelf",
		"ChebyshevIILowPass", "ChebyshevIIHighPass", "ChebyshevIIBandPass", "ChebyshevIIBandStop", "ChebyshevIILowShelf", "ChebyshevIIHighShelf", "ChebyshevIIBandShelf",
		"EllipticLowPass", "EllipticHighPass", "EllipticBandPass", "EllipticBandStop",
		"LegendreLowPass", "LegendreHighPass", "LegendreBandPass", "LegendreBandStop",
		"BesselLowPass", "BesselHighPass", "BesselBandPass", "BesselBandStop", "BesselLowShelf"};

}