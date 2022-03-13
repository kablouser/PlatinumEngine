// todo change to <> brackets
#include <Logger/Logger.h>
// to print to console
#include <iostream>
// for easier string manipulation
#include <sstream>
// for displaying gui controls
#include <imgui.h>

namespace
{
	// Static here make this only visible inside this source file
	static std::vector<PlatinumEngine::Logger*> allLoggers;

	// Static here make this only visible inside this source file
	static void LogInConsole(const char* messageType, const char* message, const char* file, unsigned int line,
			PlatinumEngine::Logger::LogType type)
	{
		std::string fileString(file);
		// remove the full path, just use the filename
		fileString = fileString.substr(fileString.find_last_of("\\/") + 1);

		std::ostringstream outputStringStream;
		outputStringStream << messageType << ' ' << message;
		if (file != nullptr)
			outputStringStream << ' ' << fileString << "(" << line << ")";

		// copy to string
		std::string output = outputStringStream.str();
		// only use std::cout, not std::cerr
		// because it preserves the ordering
		std::cout << output << std::endl;
		// save output in all loggers
		for(size_t i = 0; i < allLoggers.size(); ++i)
			allLoggers[i]->SaveLog(type, output);
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
		ImGui::Begin("Logger", isOpen);

		if (ImGui::Button("Clear"))
			savedLogs.clear();
		ImGui::SameLine();
		bool isCheckboxClicked = ImGui::Checkbox("Scroll to bottom", &_scrollToBottom);

		ImGui::Separator();

		ImGui::BeginChild("scrolling");
		// make InputText background clear
		ImGui::PushStyleColor(ImGuiCol_FrameBg, IM_COL32(0, 0, 0, 0));
		// make InputText label go away
		ImGui::PushItemWidth(-1);

		for(size_t i = 0; i < savedLogs.size(); ++i)
			ImGui::InputText(
					savedLogs[i].uniqueID.c_str(),
					&savedLogs[i].message[0],
					savedLogs[i].message.size() + 1,
					ImGuiInputTextFlags_ReadOnly);

		// check if user tried to scroll, but not when checkbox is clicked
		if(ImGui::GetScrollY() != ImGui::GetScrollMaxY() && !isCheckboxClicked)
			// let user scroll around
			_scrollToBottom = false;

		if (_scrollToBottom)
			ImGui::SetScrollHereY(1.0f);

		// restore settings
		ImGui::PopItemWidth();
		ImGui::PopStyleColor();
		ImGui::EndChild();

		ImGui::End();
	}

	void Logger::SaveLog(Logger::LogType type, const std::string& message)
	{
		std::string uniqueID = "##Log"+std::to_string(_nextUniqueID);
		++_nextUniqueID;

		savedLogs.push_back({uniqueID, type, message});
	}
}