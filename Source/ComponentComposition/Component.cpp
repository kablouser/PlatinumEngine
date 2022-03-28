#include <ComponentComposition/Component.h>
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Component::Component() : _gameObject(nullptr), _isEnabled(true), _isEnabledInHierarchy(false)
	{
	}

	Component::~Component()
	{
	}

	//--------------------------------------------------------------------------------------------------------------
	// _isEnabled controls
	//--------------------------------------------------------------------------------------------------------------

	bool Component::IsEnabled() const
	{
		return _isEnabled;
	}

	void Component::SetEnabled(bool isEnabled, Scene& scene)
	{

		// don't trigger event's if enabled state is not changed
		if (_isEnabled == isEnabled)
			return;

		_isEnabled = isEnabled;
		UpdateIsEnabledInHierarchy(scene);
	}

	bool Component::IsEnabledInHierarchy() const
	{
		if (_gameObject)
			return _isEnabled && _gameObject->IsEnabledInHierarchy();
		else
			return false;
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObject controls
	//--------------------------------------------------------------------------------------------------------------

	GameObject* Component::GetGameObject()
	{
		return _gameObject;
	}

	void Component::SetGameObject(GameObject* gameObject, Scene& scene)
	{
		if (_gameObject == gameObject)
			return;
    
		if (_gameObject)
			_gameObject->RemoveComponent(this);

		if (gameObject)
			gameObject->_components.push_back(this);

		_gameObject = gameObject;
		UpdateIsEnabledInHierarchy(scene);
	}


	//------------------------------------------------------------------------------------------------------------------
	// Events
	// Default empty implementations of events.
	//------------------------------------------------------------------------------------------------------------------

	void Component::OnStart()
	{
	}

	void Component::OnEnd()
	{
	}

	void Component::OnEnable(Scene& scene)
	{
	}

	void Component::OnDisable(Scene& scene)
	{
	}

	void Component::OnUpdate(Scene& scene, double deltaTime)
	{
	}

	void Component::OnRender(Scene& scene, Renderer& renderer)
	{
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	bool Component::CalculateIsEnabledInHierarchy() const
	{
		if (_gameObject)
			return _isEnabled && _gameObject->_isEnabledInHierarchy;
		else
			return false;
	}
  
	void Component::UpdateIsEnabledInHierarchy(Scene& scene)
	{
		bool isEnabledInHierarchyNow = CalculateIsEnabledInHierarchy();

		if (_isEnabledInHierarchy == isEnabledInHierarchyNow)
			return;

		_isEnabledInHierarchy = isEnabledInHierarchyNow;

		// only trigger events when Scene is started
		if (scene.IsStarted())
		{
			if (_isEnabledInHierarchy)
				OnEnable(scene);
			else
				OnDisable(scene);
		}
	}
}
