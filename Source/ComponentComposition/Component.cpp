#include <ComponentComposition/Component.h>
#include <ComponentComposition/GameObject.h>
#include <SceneManager/Scene.h>
#include <Logger/Logger.h>

namespace PlatinumEngine
{
	//--------------------------------------------------------------------------------------------------------------
	// Constructors/destructors
	//--------------------------------------------------------------------------------------------------------------

	Component::Component() : _isEnabled(true), _isEnabledInHierarchy(false)
	{
	}

	Component::~Component() = default;

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
			return _isEnabled && _gameObject.pointer->IsEnabledInHierarchy();
		else
			return false;
	}

	//--------------------------------------------------------------------------------------------------------------
	// _gameObject controls
	//--------------------------------------------------------------------------------------------------------------

	SavedReference<GameObject>& Component::GetGameObject()
	{
		return _gameObject;
	}

	void Component::SetGameObject(SavedReference<GameObject> gameObject, Scene& scene)
	{
		if (_gameObject == gameObject)
			return;

		SavedReference<Component> referenceToThis = scene.idSystem.GetSavedReference(this);
		if (!referenceToThis)
		{
			PLATINUM_WARNING("This Component is not in the ID System, cannot SetParent");
			return;
		}

		if (_gameObject)
			_gameObject.pointer->RemoveComponent(referenceToThis);
		if (gameObject)
			gameObject.pointer->_components.push_back(referenceToThis);

		_gameObject = std::move(gameObject);
		UpdateIsEnabledInHierarchy(scene);
	}


	//------------------------------------------------------------------------------------------------------------------
	// Events
	// Default empty implementations of events.
	//------------------------------------------------------------------------------------------------------------------

	void Component::OnStart(Scene& scene)
	{
	}

	void Component::OnEnd(Scene& scene)
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

	void Component::OnIDSystemUpdate(Scene& scene)
	{
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	bool Component::CalculateIsEnabledInHierarchy() const
	{
		if (_gameObject)
			return _isEnabled && _gameObject.pointer->_isEnabledInHierarchy;
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
