#pragma once

#include <Components/component.h>

namespace MirageEngine::Core::Scripting
{
	class IScript : public Components::Component
	{
	public:
		virtual ~IScript() = default;

		virtual void Init() override {};
		virtual void Update() override {};

		virtual void        OnStart() = 0;
		virtual void        OnUpdate(float dt) = 0;
		virtual void        OnFixedUpdate(float dt) = 0;
		virtual void        OnDestroy() = 0;
		virtual const char* GetName()   const = 0;

		virtual entt::id_type GetType() const override { return 1; };
	};
}