#pragma once
#include <vector>
#include <string>
#include <sstream>
namespace PlatinumEngine
{

	/**
	 * Macro for logging information using function syntax.
	 * Prints message into console and saves into any constructed Loggers.
	 * Expression can be either char* or std::string. E.g. :
	 * PLATINUM_INFO("Sky is blue.");
	 * PLATINUM_INFO(std::string("Sky is blue."));
	 */
#define PLATINUM_INFO(expression) PlatinumEngine::Logger::LogInfo(expression, __FILE__, __LINE__)
	/**
	 * Macro for logging warning using function syntax.
	 * Prints message into console and saves into any constructed Loggers.
	 * Expression can be either char* or std::string. E.g. :
	 * PLATINUM_WARNING("Reactor is overheating.");
	 * PLATINUM_WARNING(std::string("Reactor is overheating."));
	 */
#define PLATINUM_WARNING(expression) PlatinumEngine::Logger::LogWarning(expression, __FILE__, __LINE__)
	/**
	 * Macro for logging error using function syntax.
	 * Prints message into console and saves into any constructed Loggers.
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

	/**
	 * Macro for logging information using streaming syntax. New line is added at end automatically.
	 * Prints message into console and saves into any constructed Loggers.
	 * PLATINUM_INFO_STREAM << "Pi is " << 3.141 << std::endl << "e is " << 2.718;
	 */
#define PLATINUM_INFO_STREAM PlatinumEngine::LoggerStream(PlatinumEngine::Logger::LogType::info, __FILE__, __LINE__)
	/**
	 * Macro for logging warning using streaming syntax. New line is added at end automatically.
	 * Prints message into console and saves into any constructed Loggers.
	 * PLATINUM_WARNING_STREAM << "Engine " << i << " has malfunctioned.";
	 */
#define PLATINUM_WARNING_STREAM PlatinumEngine::LoggerStream(PlatinumEngine::Logger::LogType::warning, __FILE__, __LINE__)
	/**
	 * Macro for logging error using streaming syntax. New line is added at end automatically.
	 * Prints message into console and saves into any constructed Loggers.
	 * PLATINUM_ERROR_STREAM << "Terminal damage, please consult your local GP.";
	 */
#define PLATINUM_ERROR_STREAM PlatinumEngine::LoggerStream(PlatinumEngine::Logger::LogType::error, __FILE__, __LINE__)

	/**
	 * Unlikely to use this class directly. You should use the macros above instead.
	 * Saves the file and line when this object was constructed.
	 * Anything that can be streamed into sstringstream/ostream 's can be put into this object.
	 * When this object is destroyed, all of its input streamed content is given to the global logging functions.
	 */
	class LoggerStream
	{
	public:
		explicit LoggerStream(Logger::LogType type, const char* file = nullptr, unsigned int line = 0);

		~LoggerStream();

		// Accepts any type that ostringstream accepts (mostly)
		// And returns the ostringstream.
		// This weirdness means you cannot give std::endl as the first input.
		// Example:
		// LoggerStream(Logger::LogType::info) << "Hello world"; // OK
		// LoggerStream(Logger::LogType::info) << "Hello world" << std::endl; // OK
		// LoggerStream(Logger::LogType::info) << std::endl; // NOT GOOD
		template <typename T>
		std::ostringstream& operator<<(T&& t)
		{
			_stringStream << std::forward<T>(t);
			return _stringStream;
		}

	private:
		Logger::LogType _type;
		std::string _file;
		unsigned int _line;

		std::ostringstream _stringStream;
	};
}