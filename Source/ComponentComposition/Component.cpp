#include <ComponentComposition/Component.h>

namespace PlatinumEngine
{
	Component::Component() : _isEnabled(true)
	{}



	Component::Component(bool isEnabled) : _isEnabled(isEnabled)
	{}

	bool Component::IsEnabled() const
	{
		return _isEnabled;
	}

	void Component::SetEnabled(bool isEnabled, bool areEventsTriggered)
	{

		// don't trigger event's if enabled state is not changed
		if(_isEnabled == isEnabled)
			return;

		_isEnabled = isEnabled;

		if(!areEventsTriggered)
			return;


		if(_isEnabled)
			OnEnable();
		else
			OnDisable();
	}


	//------------------------------------------------------------------------------------------------------------------
	// Default empty implementations of events.
	//------------------------------------------------------------------------------------------------------------------

	void Component::OnStart()
	{}

	void Component::OnEnd()

	{}

	void Component::OnEnable()
	{}

	void Component::OnDisable()
	{}


	void Component::OnUpdate(float)
	{}

	void Component::OnRender(Renderer& renderer)
	{}
}