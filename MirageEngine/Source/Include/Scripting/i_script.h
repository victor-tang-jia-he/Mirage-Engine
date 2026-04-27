#pragma once

namespace MirageEngine::Core::Scripting 
{
	class IScript
	{
	public:
		virtual ~IScript() = default;

		virtual void        OnStart() = 0;
		virtual void        OnUpdate(float dt) = 0;
		virtual void        OnFixedUpdate(float dt) = 0;
		virtual void        OnDestroy() = 0;
		virtual const char* GetName()   const = 0;
	};
}