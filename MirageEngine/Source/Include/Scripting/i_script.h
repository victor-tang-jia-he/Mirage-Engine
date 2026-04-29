#pragma once

#include <Components/component.h>

namespace MirageEngine::Core::Scripting
{
	class IScript : public Components::Component
	{
	public:
		virtual ~IScript() = default;

		virtual void OnStart() {};
		virtual void OnUpdate(float dt) {};
		virtual void OnFixedUpdate(float dt) {};
		virtual void OnDestroy() {};

		virtual const char* GetName() const = 0;

		virtual entt::id_type GetType() const override { return 1; };
	};
}