#include <catch2/catch.hpp>

#include <ComponentComposition/Component.h>
#include <ComponentComposition/GameObject.h>

#include <SceneManager/Scene.h>
#include <SceneManager/SceneManager.h>

#include <Renderer/Renderer.h>

#include <Logger/Logger.h>

// Read the docs
// https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md

// Counting the events, to make sure they get called at the right times
int
		constructCount = 0,
		destructCount = 0,
		onStartCount = 0,
		onEndCount = 0,
		onEnableCount = 0,
		onDisableCount = 0,
		onUpdateCount = 0,
		onRenderCount = 0;

class MyComponent : public PlatinumEngine::Component
{
public:
	MyComponent()
	{
		PLATINUM_INFO("Component constructed");
		constructCount++;
	}

	~MyComponent()
	{
		PLATINUM_INFO("Component destroyed");
		destructCount++;
	}

	void OnStart() override
	{
		PLATINUM_INFO("Component OnStart");
		onStartCount++;
	}

	void OnEnd() override
	{
		PLATINUM_INFO("Component OnEnd");
		onEndCount++;
	}

	void OnEnable(PlatinumEngine::Scene& scene) override
	{
		PLATINUM_INFO("Component OnEnable");
		onEnableCount++;
	}

	void OnDisable(PlatinumEngine::Scene& scene) override
	{
		PLATINUM_INFO("Component OnDisable");
		onDisableCount++;
	}

	void OnUpdate(PlatinumEngine::Scene& scene, double deltaTime) override
	{
		PLATINUM_INFO("Component OnUpdate");
		onUpdateCount++;
	}

	void OnRender(PlatinumEngine::Scene& scene, PlatinumEngine::Renderer& renderer) override
	{
		PLATINUM_INFO("Component OnRender");
		onRenderCount++;
	}
};

TEST_CASE("Scene", "[scene]")
{
	constructCount =
	destructCount =
	onStartCount =
	onEndCount =
	onEnableCount =
	onDisableCount =
	onUpdateCount =
	onRenderCount = 0;

	{
		PlatinumEngine::SceneManager sceneManager;
		PlatinumEngine::Scene& scene = sceneManager.GetScene();

		auto gameObject0 = scene.AddGameObject();
		auto gameObject1 = scene.AddGameObject("gameObject1", gameObject0, true);
		auto gameObject2 = scene.AddGameObject("gameObject2", gameObject0, false);
		CHECK(scene.GetGameObjectsCount() == 3);
		CHECK(scene.GetRootGameObjectsCount() == 1);

		auto myComponent = scene.AddComponent<MyComponent>(gameObject1, true);
		CHECK(scene.GetComponentsCount() == 1);
		CHECK(constructCount == 1);

		scene.Start();
		CHECK(onStartCount == 1);
		CHECK(onEnableCount == 1);

		scene.Update(15.0);
		CHECK(onUpdateCount == 1);

		PlatinumEngine::Renderer* fakeRenderer = nullptr;
		scene.Render(*fakeRenderer);
		CHECK(onRenderCount == 1);

		scene.End();
		CHECK(onDisableCount == 1);
		CHECK(onEndCount == 1);

		// when the game hasn't started, enabling/disabling anything shouldn't trigger events
		myComponent->SetEnabled(false, scene);
		gameObject1->SetEnabled(false, scene);
		myComponent->SetEnabled(true, scene);
		gameObject1->SetEnabled(true, scene);
		CHECK(onEnableCount == 1);
		CHECK(onDisableCount == 1);

		scene.RemoveComponent(*myComponent);
		CHECK(destructCount == 1);
		CHECK(scene.GetComponentsCount() == 0);

		myComponent = scene.AddComponent<MyComponent>(gameObject2, true);
		CHECK(constructCount == 2);
		CHECK(scene.GetComponentsCount() == 1);

		// attached gameObject is disabled, shouldn't trigger OnEnable when scene starts
		scene.Start();
		CHECK(onStartCount == 2);
		CHECK(onEnableCount == 1);

		gameObject2->SetEnabled(true, scene);
		CHECK(onEnableCount == 2);

		gameObject2->SetEnabled(false, scene);
		CHECK(onDisableCount == 2);

		// detached components are not enabled
		myComponent->SetGameObject(nullptr, scene);
		CHECK(onEnableCount == 2);

		// not all parents are enabled
		auto gameObject3 = scene.AddGameObject("GameObject3", gameObject2, true);
		myComponent->SetGameObject(gameObject3, scene);
		CHECK(onEnableCount == 2);

		// all parents are enabled now
		gameObject3->SetParent(gameObject1, scene);
		CHECK(onEnableCount == 3);

		// not all parents are enabled
		gameObject3->SetParent(gameObject2, scene);
		CHECK(onDisableCount == 3);

		// enabled now
		gameObject2->SetEnabled(true, scene);
		CHECK(onEnableCount == 4);

		// destroyed now
		scene.RemoveGameObject(*gameObject0);
		CHECK(onDisableCount == 4);
		CHECK(destructCount == 2);
		CHECK(scene.GetGameObjectsCount() == 0);
		CHECK(scene.GetRootGameObjectsCount() == 0);
		CHECK(scene.GetComponentsCount() == 0);

		auto gameObject4 = scene.AddGameObject("GameObject4", nullptr, true);
		myComponent = scene.AddComponent<MyComponent>(gameObject4, false);
		CHECK(scene.GetGameObjectsCount() == 1);
		CHECK(scene.GetRootGameObjectsCount() == 1);
		CHECK(scene.GetComponentsCount() == 1);
		// constructed, but component is not enabled
		CHECK(constructCount == 3);
		CHECK(onEnableCount == 4);
	}

	// destroyed now, but was not enabled, so onDisable is not called.
	CHECK(destructCount == 3);
	CHECK(onDisableCount == 4);
}