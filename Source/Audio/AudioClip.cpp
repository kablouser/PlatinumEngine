#include <Audio/AudioClip.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	AudioClip::AudioClip() : chunk(nullptr)
	{
	}

	AudioClip::AudioClip(const char* fileName) : fileName(fileName)
	{
		chunk = Mix_LoadWAV(fileName);
		if (chunk == nullptr)
			PLATINUM_ERROR_STREAM << "Failed to load audio clip: " << fileName;
	}

	AudioClip::~AudioClip()
	{
		Mix_FreeChunk(chunk);
	}

	AudioClip::AudioClip(AudioClip&& moveFrom) noexcept :
		fileName(std::move(moveFrom.fileName)), chunk(moveFrom.chunk)
	{
		moveFrom.chunk = nullptr;
	}

	AudioClip& AudioClip::operator=(AudioClip&& moveFrom) noexcept
	{
		fileName = std::move(moveFrom.fileName);
		chunk = moveFrom.chunk;
		moveFrom.chunk = nullptr;
		return *this;
	}

	bool AudioClip::IsValid() const
	{
		return chunk != nullptr;
	}
}