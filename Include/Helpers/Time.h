//
// Created by shawn on 2022/4/22.
//

#pragma once

#include <GLFW/glfw3.h>

namespace PlatinumEngine
{
	class Time
	{
	private:
		//Values to return
		int framesPassed = 0;
		float seconds = 0.0f;
		float delta = 0.0f;
		float time = 0.0f;
		float fps = 0.0f;

		//Calculating delta time
		float lastFrame = 0.0f;
		float currentFrame = 0.0f;

	public:
		//Called at the start of the main loop
		void update(bool isPause)
		{
			//Find delta time
			if(isPause)
			{
				Time::currentFrame = Time::lastFrame;
			}
			else
			{
				Time::currentFrame = (float)glfwGetTime();
			}
			Time::delta = Time::currentFrame - Time::lastFrame;

			//Render time
			Time::time = (float)glfwGetTime();

			//Find FPS
			Time::fps = 1.0f / Time::delta;

			//Done
			Time::framesPassed++;
			Time::seconds += Time::delta;
			Time::lastFrame = Time::currentFrame;
		}

		void Stop()
		{

		}

		//Since all values should be read only they need getters
		float getCurrentFrame() { return Time::currentFrame; }
		int   getFramesPassed() { return Time::framesPassed; }
		float getLastFrame() { return Time::lastFrame; }
		float getSeconds() { return Time::seconds; }
		float GetDelta() { return Time::delta; }
		float getTime() { return Time::time; }
		float getFps() { return Time::fps; }
	};
}