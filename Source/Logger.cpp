// todo change to <> brackets
#include "Logger.h"
// to print to console
#include <iostream>

namespace
{
	// Static here make this function only visible inside this source file
	static void LogInConsole(const char* messageType, const char* message, bool isEndNewLine, const char* file, unsigned int line)
	{
		// only use std::cout, not std::cerr
		// because it preserves the ordering
		std::cout << messageType << ' ' << message;

		if (file != nullptr)
			std::cout << ' ' << file << "(" << line << ")";

		if(isEndNewLine)
			std::cout << std::endl;
	}
}

namespace PlatinumEngine
{
	void Logger::LogInfo(const char* message, bool isEndNewLine, const char* file, unsigned int line)
	{
		LogInConsole("(info)", message, isEndNewLine, file, line);
	}

	void Logger::LogWarning(const char* message, bool isEndNewLine, const char* file, unsigned int line)
	{
		// there is no warning output stream in standard C++
		LogInConsole("[Warning]", message, isEndNewLine, file, line);
	}

	void Logger::LogError(const char* message, bool isEndNewLine, const char* file, unsigned int line)
	{
		LogInConsole("{ERROR}", message, isEndNewLine, file, line);
	}
}