#include <Application.h>
#include <cassert>

namespace PlatinumEngine
{
	Application* Application::Instance = nullptr;

	Application::Application()
	{
		assert(Instance == nullptr && "Only a single instance allowed at a time");
		Instance = this;
	}

	Application::~Application()
	{
		assert(Instance == this);
		Instance = nullptr;
	}
}