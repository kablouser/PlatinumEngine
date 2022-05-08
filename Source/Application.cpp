#include <Application.h>
#include <cassert>

namespace PlatinumEngine
{
	Application::InstanceSetter::InstanceSetter(Application* newInstance)
	{
		assert(Instance == nullptr && "Only a single instance allowed at a time");
		Instance = newInstance;
	}
	Application::InstanceSetter::~InstanceSetter()
	{
		assert(Instance != nullptr);
		Instance = nullptr;
	}

	Application* Application::Instance = nullptr;

	Application::Application() : _instanceSetter(this)
	{
	}

	Application::~Application()
	{
	}
}