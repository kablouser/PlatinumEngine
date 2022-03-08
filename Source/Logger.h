#pragma once

namespace PlatinumEngine
{
#define PLATINUM_INFO(expression) PlatinumEngine::Logger::LogInfo(expression, true, __FILE__, __LINE__)
#define PLATINUM_WARNING(expression) PlatinumEngine::Logger::LogWarning(expression, true, __FILE__, __LINE__)
#define PLATINUM_ERROR(expression) PlatinumEngine::Logger::LogError(expression, true, __FILE__, __LINE__)

#define PLATINUM_INFO_SAME_LINE(expression) PlatinumEngine::Logger::LogInfo(expression, false, __FILE__, __LINE__)
#define PLATINUM_WARNING_SAME_LINE(expression) PlatinumEngine::Logger::LogWarning(expression, false, __FILE__, __LINE__)
#define PLATINUM_ERROR_SAME_LINE(expression) PlatinumEngine::Logger::LogError(expression, false, __FILE__, __LINE__)

	class Logger
	{
	public:
		static void LogInfo(const char* message, bool isEndNewLine = true, const char* file = nullptr, unsigned int line = 0);
		static void LogWarning(const char* message, bool isEndNewLine = true, const char* file = nullptr, unsigned int line = 0);
		static void LogError(const char* message, bool isEndNewLine = true, const char* file = nullptr, unsigned int line = 0);
	};
}