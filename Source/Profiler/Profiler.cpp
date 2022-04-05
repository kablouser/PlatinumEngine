#include <Profiler/Profiler.h>

#include <cassert>
#include <vector>
#include <map>
#include <algorithm>

#include <imgui.h>
#include <implot.h>
#include <GLFW/glfw3.h>

#include <Helpers/CircularBuffer.h>

// private namespace, things in this cannot be used outside this file
namespace
{
	constexpr unsigned int MAX_RECORD_LENGTH = 60 * 10; // 60 fps for 10 seconds

	class FrameHistory
	{
	public:
		bool isWorking = false;
		size_t workingFrame = 0;
		PlatinumEngine::CircularBuffer<double, MAX_RECORD_LENGTH> savedDurations;
	};

	class SectionHistory
	{
	public:
		std::string name;
		bool isWorking = false;
		double workingDuration = 0.0;
		// cumulative to the order of sectionHistories
		PlatinumEngine::CircularBuffer<double, MAX_RECORD_LENGTH> savedCumulativeDurations;
	};

	FrameHistory frameHistory;

	bool isAnySectionWorking = false;
	std::map<std::string, size_t> sectionNameToIndices;
	std::vector<SectionHistory> sectionHistories;

	bool isRecording = true;

	ImPlotPoint FramePlotGetter(void* _, int index)
	{
		return {
				(double)(frameHistory.workingFrame - index - 1),
				frameHistory.savedDurations.GetFromEnd(index) };
	}

	ImPlotPoint FramePlotReferenceGetter(void* _, int index)
	{
		return { (double)(frameHistory.workingFrame - index - 1), 0 };
	}

	ImPlotPoint SectionPlotGetter(void* encodedSectionIndex, int index)
	{
		return {
				(double)(frameHistory.workingFrame - index - 1),
				sectionHistories.at((size_t)encodedSectionIndex).savedCumulativeDurations.GetFromEnd(index) };
	}
}

namespace PlatinumEngine
{
	Profiler::Section::Section(const std::string& uniqueName) :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		// nested sections are not supported! only 1 section at a time, otherwise the times are wrong
		assert(!isAnySectionWorking);
		isAnySectionWorking = true;

		auto insertName = sectionNameToIndices.insert({ uniqueName, 0 });
		auto sectionIndex = insertName.first;
		bool isInserted = insertName.second;
		if (isInserted)
		{
			sectionHistories.push_back({ uniqueName, true });
			_sectionIndex = sectionIndex->second = sectionHistories.size() - 1;
		}
		else
		{
			_sectionIndex = sectionIndex->second;
			SectionHistory& sectionHistory = sectionHistories.at(_sectionIndex);
			// if this fails, the section has started somewhere else already, only 1 section per uniqueName
			assert(!sectionHistory.isWorking);
			sectionHistory.isWorking = true;
		}
	}

	Profiler::Section::~Section()
	{
		// should be true, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(isAnySectionWorking);
		isAnySectionWorking = false;

		SectionHistory& sectionHistory = sectionHistories.at(_sectionIndex);
		// should be true, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(sectionHistory.isWorking);
		sectionHistory.isWorking = false;
		if (isRecording)
		{
			std::chrono::duration<double, std::milli> doubleMilliseconds =
					std::chrono::high_resolution_clock::now() - _startTime;
			sectionHistory.workingDuration += doubleMilliseconds.count();
		}
	}

	Profiler::Frame::Frame() :
			_startTime(std::chrono::high_resolution_clock::now())
	{
		// there should only be 1 frame at a time
		assert(!frameHistory.isWorking);
		frameHistory.isWorking = true;
	}

	Profiler::Frame::~Frame()
	{
		// should be created, because this object has been constructed
		// if this assertion fails, something has gone horribly wrong
		assert(frameHistory.isWorking);
		frameHistory.isWorking = false;
		if (isRecording)
		{
			std::chrono::duration<double, std::milli> doubleMilliseconds =
					std::chrono::high_resolution_clock::now() - _startTime;
			double frameDuration = doubleMilliseconds.count();

			for (SectionHistory& sectionHistory: sectionHistories)
			{
				// all sections should be finished by now
				assert(!sectionHistory.isWorking);
				frameDuration += sectionHistory.workingDuration;
				// cumulative total
				sectionHistory.savedCumulativeDurations.Add(frameDuration);
				sectionHistory.workingDuration = {};
			}

			frameHistory.savedDurations.Add(frameDuration);
			++frameHistory.workingFrame;
		}
	}

	Profiler::Profiler() : _showFrames(400)
	{
	}

	void Profiler::ShowGUIWindow(bool* outIsOpen)
	{
		if (ImGui::Begin("Profiler", outIsOpen))
		{
			bool isPaused = !isRecording;
			ImGui::Checkbox("Pause", &isPaused);
			isRecording = !isPaused;

			ImGui::SameLine();

			ImGui::SliderInt("Show Frames", &_showFrames, 1, MAX_RECORD_LENGTH);

			if (ImPlot::BeginPlot("##ProfilerPlot", ImVec2(-1, -1),
					ImPlotFlags_NoTitle | ImPlotFlags_NoBoxSelect | ImPlotFlags_NoMouseText))
			{
				int drawCount = std::min(_showFrames, (int)frameHistory.savedDurations.Size());
				// casting from size_t to int might cause negatives
				if (drawCount < 0)
					drawCount = 0;

				double currentFrame = (double)(frameHistory.workingFrame - 1);

				ImPlot::SetupAxes("frame", "time taken (ms)", ImPlotAxisFlags_None, ImPlotAxisFlags_LockMin);
				ImPlot::SetupAxisLimits(ImAxis_X1, currentFrame - (double)_showFrames, currentFrame, ImGuiCond_Always);
				ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 20.0);

				if (0 < drawCount)
				{
					ImPlot::PlotShadedG(
							"Frame duration",
							FramePlotGetter,
							nullptr,
							FramePlotReferenceGetter,
							nullptr,
							drawCount);
					// reverse order, because draw back to front
					// they are in accumulative order
					for (size_t i = sectionHistories.size() - 1; i < sectionHistories.size(); --i)
					{
						SectionHistory& sectionHistory = sectionHistories[i];
						int sectionDrawCount = std::min(
								drawCount,
								(int)sectionHistory.savedCumulativeDurations.Size());
						if (0 < sectionDrawCount)
						{
							ImPlot::PlotShadedG(
									sectionHistory.name.c_str(),
									SectionPlotGetter,
									(void*)i,
									FramePlotReferenceGetter,
									nullptr,
									drawCount);
						}
					}
				}

				ImPlot::EndPlot();
			}
		}
		ImGui::End();
	}
}
