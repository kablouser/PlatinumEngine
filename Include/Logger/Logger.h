#pragma once
#include <vector>
#include <string>

namespace PlatinumEngine
{
	/**
	 * Macro for logging information. Prints message into console and saves into any constructed Loggers.
	 * Expression can be either char* or std::string. E.g. :
	 * PLATINUM_INFO("Sky is blue.");
	 * PLATINUM_INFO(std::string("Sky is blue."));
	 */
#define PLATINUM_INFO(expression) PlatinumEngine::Logger::LogInfo(expression, __FILE__, __LINE__)
	/**
	 * Macro for logging warning. Prints message into console and saves into any constructed Loggers.
	 * Expression can be either char* or std::string. E.g. :
	 * PLATINUM_WARNING("Reactor is overheating.");
	 * PLATINUM_WARNING(std::string("Reactor is overheating."));
	 */
#define PLATINUM_WARNING(expression) PlatinumEngine::Logger::LogWarning(expression, __FILE__, __LINE__)
	/**
	 * Macro for logging error. Prints message into console and saves into any constructed Loggers.
	 * Expression can be either char* or std::string. E.g. :
	 * PLATINUM_ERROR("Division by 0.");
	 * PLATINUM_ERROR(std::string("Division by 0."));
	 */
#define PLATINUM_ERROR(expression) PlatinumEngine::Logger::LogError(expression, __FILE__, __LINE__)

	/**
	 * Global functions allow logging from any place without a reference to a logger.
	 * A logger instance will then save any logs during its lifetime. And display those logs in a ImGui window.
	 */
	class Logger
	{
	public:

		enum class LogType {info, warning, error};

		class Log
		{
		public:
			// ImGui InputText requires a unique string for highlighting to work. This is that.
			std::string uniqueID;
			LogType type;
			std::string message;
		};

		// You shouldn't use these functions directly, use the macros instead.
		static void LogInfo(const std::string& message, const char* file = nullptr, unsigned int line = 0);
		static void LogWarning(const std::string& message, const char* file = nullptr, unsigned int line = 0);
		static void LogError(const std::string& message, const char* file = nullptr, unsigned int line = 0);
		static void LogInfo(const char* message, const char* file = nullptr, unsigned int line = 0);
		static void LogWarning(const char* message, const char* file = nullptr, unsigned int line = 0);
		static void LogError(const char* message, const char* file = nullptr, unsigned int line = 0);

		/**
		 * Logs saved during the lifetime of this Logger object.
		 */
		std::vector<Log> savedLogs;

		/*
		 * Starts saving logs inside of this object
		 */
		Logger();

		~Logger();

		/*
		 * Copy constructors messes with the static loggers list.
		 */
		Logger(Logger&&) = delete;

		/**
		 * Shows a ImGui window that shows and controls savedLogs.
		 * @param isOpen is the window open? in and out.
		 */
		void ShowGUIWindow(bool* isOpen);

		/**
		 * Save a new log into this object instance. You shouldn't use this function directly.
		 */
		void SaveLog(LogType type, const std::string& message);

	private:

		// next available ID that's *probably* unique, loops back when numbers run out
		unsigned int _nextUniqueID;
		// should this window scroll to the bottom of the logs?
		bool _scrollToBottom;
	};
}