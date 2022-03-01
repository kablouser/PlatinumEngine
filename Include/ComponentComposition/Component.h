#pragma once

namespace PlatinumEngine
{
	class Component
	{
	public:
		Component();

		void SetEnabled(bool isEnabled);

		virtual void OnStart();

		virtual void OnDestroy();

		virtual void OnEnable();

		virtual void OnDisable();

		virtual void OnUpdate();

	private:
		bool _isEnabled;
		// GameObject* parent;
	};
}