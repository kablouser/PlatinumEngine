#include <ComponentComposition/Component.h>

namespace PlatinumEngine
{
	Component::Component() : _isEnabled(true)
	{}

	void Component::SetEnabled(bool isEnabled)
	{
		_isEnabled = isEnabled;
		if(_isEnabled)
			OnEnable();
		else
			OnDisable();
	}

	void Component::OnStart()
	{}

	void Component::OnDestroy()
	{}

	void Component::OnEnable()
	{}

	void Component::OnDisable()
	{}

	void Component::OnUpdate()
	{}
}