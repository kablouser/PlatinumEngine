#include <Profiler/Profiler.h>

#include <cassert>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>

#include <imgui.h>
#include <implot.h>
#include <GLFW/glfw3.h>

#include <Helpers/CircularBuffer.h>

// private namespace, things in this cannot be used outside this file
namespace
{
	constexpr unsigned int MAX_RECORD_LENGTH = 60 * 10; // 60 fps for 10 seconds

	class FrameRecord
	{
	public:
		double timeStart = 0.0;
		double duration = 0.0;
		double durationRecordedBySections = 0.0;
	};

	class SectionRecord
	{
	public:
		double timeStart = -1.0;
		double duration = 0.0;
		double durationCumulative = 0.0;
	};

	class FrameHistory
	{
	public:
		bool isWorking = false;
		FrameRecord workingRecord;
		PlatinumEngine::CircularBuffer<FrameRecord, MAX_RECORD_LENGTH> history;
	};

	class SectionHistory
	{
	public:
		std::string name;
		bool isWorking = false;
		SectionRecord workingRecord;
		PlatinumEngine::CircularBuffer<SectionRecord, MAX_RECORD_LENGTH> history;
	};

	FrameHistory frameHistory;

	std::map<std::string, size_t> sectionNameToIndices;
	std::vector<SectionHistory> sectionHistories;

	ImPlotPoint FramePlotGetter(void* _, int index)
	{
		FrameRecord& frameRecord = frameHistory.history.GetFromEnd(index);
		return { frameRecord.timeStart, frameRecord.duration };
	}

	ImPlotPoint FramePlotReferenceGetter(void* _, int index)
	{
		FrameRecord& frameRecord = frameHistory.history.GetFromEnd(index);
		return { frameRecord.timeStart, 0.0 };
	}

	ImPlotPoint SectionPlotGetter(void* sectionIndexData, int index)
	{
		size_t sectionIndex = (size_t)sectionIndexData;
		SectionRecord& sectionRecord = sectionHistories.at(sectionIndex).history.GetFromEnd(index);
		return { sectionRecord.timeStart, sectionRecord.durationCumulative };
	}

	ImPlotPoint SectionPlotReferenceGetter(void* sectionIndexData, int index)
	{
		size_t sectionIndex = (size_t)sectionIndexData;
		SectionRecord& sectionRecord = sectionHistories.at(sectionIndex).history.GetFromEnd(index);
		return { sectionRecord.timeStart, 0.0 };
	}
}

namespace PlatinumEngine
{
	Profiler::Section::Section(const std::string& uniqueName) :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		auto insertName = sectionNameToIndices.insert({ uniqueName, 0 });
		auto sectionIndex = insertName.first;
		bool isInserted = insertName.second;
		if (isInserted)
		{
			sectionHistories.push_back({ uniqueName, true, { glfwGetTime() }, });
			_sectionIndex = sectionIndex->second = sectionHistories.size() - 1;
		}
		else
		{
			_sectionIndex = sectionIndex->second;
			SectionHistory& sectionHistory = sectionHistories.at(_sectionIndex);
			// if this fails, the section has started somewhere else already, only 1 section per uniqueName
			assert(!sectionHistory.isWorking);
			sectionHistory.isWorking = true;
			if (sectionHistory.workingRecord.timeStart == -1.0)
				sectionHistory.workingRecord.timeStart = glfwGetTime();
		}
	}

	Profiler::Section::~Section()
	{
		SectionHistory& sectionHistory = sectionHistories.at(_sectionIndex);
		// should be created, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(sectionHistory.isWorking);
		sectionHistory.isWorking = false;
		std::chrono::duration<double, std::milli> doubleMilliseconds =
				std::chrono::high_resolution_clock::now() - _startTime;
		sectionHistory.workingRecord.duration += doubleMilliseconds.count();
	}

	Profiler::Frame::Frame() :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		// there should only be 1 frame at a time
		assert(!frameHistory.isWorking);
		frameHistory.isWorking = true;
		frameHistory.workingRecord = { glfwGetTime() };
	}

	Profiler::Frame::~Frame()
	{
		// should be created, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(frameHistory.isWorking);
		frameHistory.isWorking = false;
		std::chrono::duration<double, std::milli> doubleMilliseconds =
				std::chrono::high_resolution_clock::now() - _startTime;
		frameHistory.workingRecord.duration = doubleMilliseconds.count();
		frameHistory.workingRecord.durationRecordedBySections = 0.0;

		for (SectionHistory& sectionHistory: sectionHistories)
		{
			// all sections should be finished by now
			assert(!sectionHistory.isWorking);
			frameHistory.workingRecord.durationRecordedBySections += sectionHistory.workingRecord.duration;
			// cumulative total
			sectionHistory.workingRecord.durationCumulative = frameHistory.workingRecord.durationRecordedBySections;
			sectionHistory.history.Add(sectionHistory.workingRecord);
			sectionHistory.workingRecord = {};
			// TODO testing for programming error
			assert(sectionHistory.workingRecord.timeStart == -1.0);
		}

		frameHistory.history.Add(frameHistory.workingRecord);
		frameHistory.workingRecord = {};
		// TODO testing for programming error
		assert(frameHistory.workingRecord.timeStart == 0.0);
	}

	Profiler::Profiler() : _showCounter()
	{
	}

	void Profiler::ShowGUIWindow(bool* outIsOpen)
	{
		if (ImGui::Begin("Profiler", outIsOpen))
		{
			static float historySeconds = 2;
			static bool isProfilerPlotHovered = false;
			static ImPlotPoint plotMousePosition;
			ImGui::SliderFloat("History", &historySeconds, 0.1, 15);

			if (isProfilerPlotHovered)
			{
//				ImGui::SameLine();
//				ImGui::Text("x:%f y:%f", plotMousePosition.x, plotMousePosition.y);
			}

			if (ImPlot::BeginPlot("##ProfilerPlot", ImVec2(-1, -1), ImPlotFlags_NoBoxSelect))
			{
				int drawCount = static_cast<int>(frameHistory.history.Size());

				double currentTime;
				if (0 < drawCount)
					currentTime = frameHistory.history.GetFromEnd(0).timeStart;
				else
					currentTime = 0.0;

				ImPlot::SetupAxes(nullptr, nullptr, ImPlotAxisFlags_NoTickLabels, ImPlotAxisFlags_LockMin);
				ImPlot::SetupAxisLimits(ImAxis_X1, currentTime - historySeconds, currentTime, ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 30.0);

				isProfilerPlotHovered = ImPlot::IsPlotHovered();
				plotMousePosition = ImPlot::GetPlotMousePos();

				if (0 < drawCount)
				{
					ImPlot::PushStyleVar(ImPlotStyleVar_FillAlpha, 0.25f);
					std::string randomName = "Frame duration " + std::to_string(rand());
					ImPlot::PlotShadedG(
							randomName.c_str(),
							FramePlotGetter,
							nullptr,
							FramePlotReferenceGetter,
							nullptr,
							drawCount);
					// reverse order, because draw back to front
					// they are in accumulative order
					for (size_t i = sectionHistories.size() - 1; i < sectionHistories.size(); --i)
					{
						ImPlot::PlotShadedG(
								sectionHistories[i].name.c_str(),
								SectionPlotGetter,
								(void*)i,
								SectionPlotReferenceGetter,
								(void*)i,
								drawCount);
					}
					ImPlot::PopStyleVar();

					ImPlot::PlotLineG(
							"Frame duration",
							FramePlotGetter,
							nullptr,
							drawCount);
					// reverse order, because draw back to front
					// they are in accumulative order
					for (size_t i = sectionHistories.size() - 1; i < sectionHistories.size(); --i)
					{
						ImPlot::PlotLineG(
								sectionHistories[i].name.c_str(),
								SectionPlotGetter,
								(void*)i,
								drawCount);
					}
				}

				ImPlot::EndPlot();
			}
			else
			{
				isProfilerPlotHovered = false;
			}
		}
		ImGui::End();
		ImPlot::ShowDemoWindow(nullptr);
	}
}
