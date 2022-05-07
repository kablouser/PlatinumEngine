#pragma once

#include <string>
#include <SDL_mixer.h>

namespace PlatinumEngine
{
	/**
	 * Represents an asset.
	 * Uses Mix_Chunk not Mix_Music
	 *
	 * tl;dr Mix_Chunk is lower bit rate but you can play lots of them at once
	 * whereas Mix_Music is higher bit rate but only 1 music played at a time
	 *
	 * Mix_Chunk and Mix_Music uses slightly different file types. So it's just really complicated.
	 *
	 * Simply this whole problem by just using Mix_Chunk for everything.
	 * Mix_Chunk can use WAVE (aka .wav), AIFF, RIFF, OGG, and VOC files.
	 */
	class AudioClip
	{
	public:

		std::string fileName;
		Mix_Chunk* chunk;

		AudioClip();
		AudioClip(const char* fileName);
		~AudioClip();

		// NO COPY ALLOWED
		AudioClip(AudioClip&) = delete;
		AudioClip& operator=(AudioClip&) = delete;

		// move okay
		AudioClip(AudioClip&&) noexcept;
		AudioClip& operator=(AudioClip&&) noexcept;

		/**
		 * @return true if chunk was loaded correctly, otherwise false.
		 */
		bool IsValid() const;
	};
}