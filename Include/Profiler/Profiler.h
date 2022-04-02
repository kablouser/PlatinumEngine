#pragma once

// for timing
#include <chrono>
// for naming
#include <string>

namespace PlatinumEngine
{
	class Profiler
	{
	public:

		class Section
		{
		public:
			explicit Section(const std::string& uniqueName);
			~Section();
		private:
			size_t _nameIndex;
			std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
		};

		class Frame
		{
		public:
			Frame();
			~Frame();
		private:
			std::chrono::time_point<std::chrono::high_resolution_clock> _startTime;
		};

		Profiler();

		void ShowGUIWindow(bool* outIsOpen);

	private:
		unsigned int _showCounter;
		std::string _showText;
	};
}