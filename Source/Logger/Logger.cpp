#include <Logger/Logger.h>
// to print to console
#include <iostream>
// for dynamic allocation
#include <vector>
// for easier string manipulation
#include <sstream>
// for displaying gui controls
#include <imgui.h>
#include <IconsKenney.h>
namespace
{
	// Static here make this only visible inside this source file
	static std::vector<PlatinumEngine::Logger*> allLoggers;

	// Static here make this only visible inside this source file

	static void LogInConsole(const char* messageType, const char* message, const char* file, unsigned int line,
			PlatinumEngine::Logger::LogType type)
	{
		// stringbuilder
		std::ostringstream outputStringStream;
		// need to seperate message into lines, otherwise GUI cutoff text
		std::vector<std::string> outputLines;

		outputStringStream << messageType << ' ';

		for(size_t i = 0; message[i] != '\0'; ++i)
		{
			if(message[i] == '\n')
			{
				// newline
				outputLines.push_back(outputStringStream.str());
				// delete contents of ostringstream and reset
				outputStringStream.str("");
				outputStringStream.clear();
			}
			else
			{
				outputStringStream << message[i];
			}
		}

		// should we print the file and line?
		if (file != nullptr && file[0] != '\0')
		{
			std::string fileString(file);
			// remove the full path, just use the filename
			fileString = fileString.substr(fileString.find_last_of("\\/") + 1);
			outputStringStream << ' ' << fileString << "(" << line << ")";
		}

		outputLines.push_back(outputStringStream.str());

		for(const std::string& line : outputLines)
		{
			// only use std::cout, not std::cerr
			// because it preserves the ordering
			std::cout << line << std::endl;
			// save output in all loggers
			for(size_t i = 0; i < allLoggers.size(); ++i)
				allLoggers[i]->SaveLog(type, line);
		}
	}
}

namespace PlatinumEngine
{
	void Logger::LogInfo(const std::string& message, const char* file, unsigned int line)
	{
		LogInfo(message.c_str(), file, line);
	}

	void Logger::LogWarning(const std::string& message, const char* file, unsigned int line)
	{
		LogWarning(message.c_str(), file, line);
	}

	void Logger::LogError(const std::string& message, const char* file, unsigned int line)
	{
		LogError(message.c_str(), file, line);
	}

	void Logger::LogInfo(const char* message, const char* file, unsigned int line)
	{

		LogInConsole("(info)", message, file, line, LogType::info);

	}

	void Logger::LogWarning(const char* message, const char* file, unsigned int line)
	{
		// there is no warning output stream in standard C++

		LogInConsole("[Warning]", message, file, line, LogType::warning);

	}

	void Logger::LogError(const char* message, const char* file, unsigned int line)
	{

		LogInConsole("{ERROR}", message, file, line, LogType::error);
	}

	Logger::Logger() : _scrollToBottom(true)

	{
		allLoggers.push_back(this);
	}

	Logger::~Logger()
	{
		// remove from all allLoggers
		for(auto iterator = allLoggers.begin(); iterator != allLoggers.end(); iterator++)
		{
			if(*iterator == this)
			{
				allLoggers.erase(iterator);
				return;
			}
		}
	}

	void Logger::ShowGUIWindow(bool* isOpen)
	{
		if(ImGui::Begin(ICON_KI_EXCLAMATION_TRIANGLE " Logger", isOpen))
		{
			if (ImGui::Button("Clear"))
				savedLogs.clear();
			ImGui::SameLine();
			bool isCheckboxClicked = ImGui::Checkbox("Scroll to bottom", &_scrollToBottom);

			// TODO: If we get a nice way to send stuff to the right without causing an overlap on resize we should do that here
			// ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x - 250.f);
			ImGui::SameLine();
			if (ImGui::RadioButton("Information", _logLevel == LogType::info))
			{
				_logLevel = LogType::info;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Warning", _logLevel == LogType::warning))
			{
				_logLevel = LogType::warning;
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Error", _logLevel == LogType::error))
			{
				_logLevel = LogType::error;
			}

			ImGui::Separator();

			ImGui::BeginChild("scrolling");
			// make InputText background clear
			ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
			// make InputText label go away
			ImGui::PushItemWidth(-1);

			for (size_t i = 0; i < savedLogs.size(); ++i)
			{
				// Since LogType enum is in a nice order, simply compare to find current highest log level
				if (_logLevel <= savedLogs[i].type)
				{
					ImGui::InputText(
							savedLogs[i].uniqueID.c_str(),
							&savedLogs[i].message[0],
							savedLogs[i].message.size() + 1,
							ImGuiInputTextFlags_ReadOnly);
				}
			}

			// check if user tried to scroll, but not when checkbox is clicked
			if (ImGui::GetScrollY() != ImGui::GetScrollMaxY() && !isCheckboxClicked)
				// let user scroll around
				_scrollToBottom = false;

			if (_scrollToBottom)
				ImGui::SetScrollHereY(1.0f);

			// restore settings
			ImGui::PopItemWidth();
			ImGui::PopStyleColor();
			ImGui::EndChild();
		}
		ImGui::End();
	}

	void Logger::SaveLog(Logger::LogType type, const std::string& message)
	{
		std::string uniqueID = "##Log"+std::to_string(_nextUniqueID);
		++_nextUniqueID;

		savedLogs.push_back({uniqueID, type, message});
	}

	LoggerStream::LoggerStream(Logger::LogType type, const char* file, unsigned int line) :
		_type(type),
		_line(line)
	{
		// std::string cannot be constructed with nullptr,
		// and 0 length c-strings are not worth storing
		if (file != nullptr && file[0] != '\0')
			_file = std::string(file);
	}

	LoggerStream::~LoggerStream()
	{
		switch(_type)
		{
		case Logger::LogType::info:
			Logger::LogInfo(_stringStream.str(), _file.c_str(), _line);
			break;
		case Logger::LogType::warning:
			Logger::LogWarning(_stringStream.str(), _file.c_str(), _line);
			break;
		case Logger::LogType::error:
			Logger::LogError(_stringStream.str(), _file.c_str(), _line);
			break;

		default:
			// _type is not recognised
			assert(false);
			break;
		}
	}
}