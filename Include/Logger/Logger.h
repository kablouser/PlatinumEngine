#pragma once
#include <vector>
#include <string>

namespace PlatinumEngine
{
#define PLATINUM_INFO(expression) PlatinumEngine::Logger::LogInfo(expression, __FILE__, __LINE__)
#define PLATINUM_WARNING(expression) PlatinumEngine::Logger::LogWarning(expression, __FILE__, __LINE__)
#define PLATINUM_ERROR(expression) PlatinumEngine::Logger::LogError(expression, __FILE__, __LINE__)

#define PLATINUM_INFO_SAME_LINE(expression) PlatinumEngine::Logger::LogInfo(expression, __FILE__, __LINE__)
#define PLATINUM_WARNING_SAME_LINE(expression) PlatinumEngine::Logger::LogWarning(expression, __FILE__, __LINE__)
#define PLATINUM_ERROR_SAME_LINE(expression) PlatinumEngine::Logger::LogError(expression, __FILE__, __LINE__)

	class Logger
	{
	public:

		static void LogInfo(const std::string& message, const char* file = nullptr, unsigned int line = 0);
		static void LogWarning(const std::string& message, const char* file = nullptr, unsigned int line = 0);
		static void LogError(const std::string& message, const char* file = nullptr, unsigned int line = 0);

		static void LogInfo(const char* message, const char* file = nullptr, unsigned int line = 0);
		static void LogWarning(const char* message, const char* file = nullptr, unsigned int line = 0);
		static void LogError(const char* message, const char* file = nullptr, unsigned int line = 0);

		std::vector<std::string> savedLogs;

		/*
		 * starts saving all logged messages inside of this object
		 */
		Logger();

		~Logger();

		/*
		 * Copy constructors messes with the loggers static list.
		 */
		Logger(Logger&&) = delete;

		void ShowGUIWindow(bool* isOpen);
	};
}