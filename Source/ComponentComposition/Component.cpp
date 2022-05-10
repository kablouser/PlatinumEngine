#include <ComponentComposition/Component.h>
#include <Application.h>


namespace PlatinumEngine
{
	void Component::CreateTypeInfo(TypeDatabase& typeDatabase)
	{
		/*
		SavedReference<GameObject> _gameObject;
		bool _isEnabled;
		bool _isEnabledInHierarchy;
		 */
		typeDatabase.BeginTypeInfoWithoutAllocator<Component>()
		        .WithField<SavedReference<GameObject>>("_gameObject", PLATINUM_OFFSETOF(Component, _gameObject))
				.WithField<bool>("_isEnabled", PLATINUM_OFFSETOF(Component, _isEnabled))
				.WithField<bool>("_isEnabledInHierarchy", PLATINUM_OFFSETOF(Component, _isEnabledInHierarchy));
	}

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

	void Component::SetEnabled(bool isEnabled)
	{
		// don't trigger event's if enabled state is not changed
		if (_isEnabled == isEnabled)
			return;

		_isEnabled = isEnabled;
		UpdateIsEnabledInHierarchy();
	}

	bool Component::IsEnabledInHierarchy() const
	{
		if (_gameObject)
			return _isEnabled && _gameObject.DeRef()->IsEnabledInHierarchy();
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

		SavedReference<Component> referenceToThis = Application::Instance->idSystem.GetSavedReference(this);
		if (!referenceToThis)
		{
			PLATINUM_WARNING("This Component is not in the ID System, cannot SetParent");
			return;
		}

		if (_gameObject)
			_gameObject.DeRef()->RemoveComponent(referenceToThis);
		if (gameObject)
			gameObject.DeRef()->_components.push_back(referenceToThis);

		_gameObject = std::move(gameObject);
		UpdateIsEnabledInHierarchy();
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

	void Component::OnEnable()
	{
	}

	void Component::OnDisable()
	{
	}

	void Component::OnUpdate()
	{
	}

	void Component::OnRender()
	{
	}

	void Component::OnIDSystemUpdate()
	{
	}

	//--------------------------------------------------------------------------------------------------------------
	// Internal controls
	//--------------------------------------------------------------------------------------------------------------

	bool Component::CalculateIsEnabledInHierarchy() const
	{
		if (_gameObject)
			return _isEnabled && _gameObject.DeRef()->_isEnabledInHierarchy;
		else
			return false;
	}

	void Component::UpdateIsEnabledInHierarchy()
	{
		bool isEnabledInHierarchyNow = CalculateIsEnabledInHierarchy();

		if (_isEnabledInHierarchy == isEnabledInHierarchyNow)
			return;

		_isEnabledInHierarchy = isEnabledInHierarchyNow;

		// only trigger events when Scene is started
		if (Application::Instance->scene.IsStarted())
		{
			if (_isEnabledInHierarchy)
				OnEnable();
			else
				OnDisable();
		}
	}
}
