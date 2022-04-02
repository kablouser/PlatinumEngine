#include <Profiler/Profiler.h>

#include <cassert>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#include <imgui.h>
#include <implot.h>

// TODO remove
#include <Logger/Logger.h>

// private namespace, things in this cannot be used outside this file
namespace
{
	unsigned int MAX_RECORD_LENGTH = 60 * 10; // 60 fps for 10 seconds

	struct SectionRecord
	{
		size_t nameIndex = 0;
		double duration = 0.0;
	};

	struct FrameRecord
	{
		double timeStart = 0.0;
		double duration = 0.0;
		double durationRecordedBySections = 0.0;
	};

	bool isFrameCreated = false;
	FrameRecord workingFrameRecord;
	std::vector<FrameRecord> finishedFrameRecords;

	std::map<std::string, size_t> sectionNameIndices;
	std::vector<std::string> sectionNames;

	ImPlotPoint FrameRecordGetter(void* user_data, int index)
	{
		FrameRecord& record = finishedFrameRecords.at(index);
		return {
				static_cast<double>(index),
				record.duration
		};
	}

	ImPlotPoint FrameRecordGetterFromEnd(void* user_data, int index)
	{
		return FrameRecordGetter(user_data, finishedFrameRecords.size() - index - 1);
	}
}

namespace PlatinumEngine
{
	Profiler::Section::Section(const std::string& uniqueName) :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		auto insertName = sectionNameIndices.insert({ uniqueName, 0 });
		auto sectionNameIndex = insertName.first;
		bool isInserted = insertName.second;
		if (isInserted)
		{
			sectionNames.push_back(uniqueName);
			_nameIndex = sectionNameIndex->second = sectionNames.size() - 1;
		}
		else
		{
			_nameIndex = sectionNameIndex->second;
		}
	}

	Profiler::Section::~Section()
	{
		std::chrono::duration<double, std::milli> doubleMilliseconds =
				std::chrono::high_resolution_clock::now() - _startTime;
//		workingFrameRecord.sectionRecords.push_back({
//				_nameIndex,
//				doubleMilliseconds.count()
//		});
	}

	Profiler::Frame::Frame() :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		// there should only be 1 frame at a time
		assert(!isFrameCreated);
		isFrameCreated = true;
		workingFrameRecord = {};
	}

	Profiler::Frame::~Frame()
	{
		// should be created, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(isFrameCreated);
		isFrameCreated = false;

		std::chrono::duration<double, std::milli> doubleMilliseconds =
				std::chrono::high_resolution_clock::now() - _startTime;
		workingFrameRecord.duration = doubleMilliseconds.count();
		workingFrameRecord.durationRecordedBySections = 0.0;
//		for (auto& section: workingFrameRecord.sectionRecords)
//			workingFrameRecord.durationRecordedBySections += section.duration;

		finishedFrameRecords.emplace_back(workingFrameRecord);
	}

	Profiler::Profiler() : _showCounter(0)
	{
	}

	void Profiler::ShowGUIWindow(bool* outIsOpen)
	{
		if (ImGui::Begin("Profiler", outIsOpen))
		{

//			if (++_showCounter % 10 == 0)
//			{
//				_showText.clear();
//				if (!finishedFrameRecords.empty())
//				{
//					size_t newestFrame = finishedFrameRecords.size() - 1;
//
//					std::ostringstream stringStream;
//					double recordedFraction =
//							finishedFrameRecords[newestFrame].durationRecordedBySections /
//							finishedFrameRecords[newestFrame].duration;
//
//					stringStream << "Frame" << newestFrame << ": "
//								 << finishedFrameRecords[newestFrame].duration << "ms"
//								 << '(' << recordedFraction << ')';
//					for (auto& section: finishedFrameRecords[newestFrame].sectionRecords)
//					{
//						stringStream << std::endl
//									 << "Section " << sectionNames[section.nameIndex] << ": " << section.duration
//									 << "ms";
//					}
//					_showText = stringStream.str();
//				}
//			}
//
//			ImGui::Text("%s", _showText.c_str());

//			if (ImPlot::BeginPlot("Profiler Plot", { -1, -1 }))
//			{
//				ImPlot::PlotLineG(
//						"Times Line Graph",
//						FrameRecordGetter,
//						nullptr,
//						finishedFrameRecords.size());
//				ImPlot::EndPlot();
//			}

			static int history = 100;
			ImGui::SliderInt("History",&history,1,1000,"%i history");

			const ImPlotAxisFlags flags = ImPlotAxisFlags_NoTickLabels;

			int currentFrame;
			if(finishedFrameRecords.size() == 0)
				currentFrame = 0;
			else
				currentFrame = finishedFrameRecords.size() - 1;

			if (ImPlot::BeginPlot("##ProfilerPlot", ImVec2(-1, -1)))
			{
				ImPlot::SetupAxes(NULL, NULL, flags, flags);
				ImPlot::SetupAxisLimits(ImAxis_X1, currentFrame - history, currentFrame, ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 40.0);

				int drawCount = finishedFrameRecords.size() < history ?
								finishedFrameRecords.size() :
								history;

				ImPlot::PlotLineG(
						"Times Line Graph",
						FrameRecordGetterFromEnd,
						nullptr,
						drawCount);
				ImPlot::EndPlot();
			}
		}
		ImGui::End();
		ImPlot::ShowDemoWindow(nullptr);
	}
}
