#pragma once
#include <Scripting/engine_script_api.h>

#include <Light/i_light.h>

DECLARE_LOG_CATEGORY_EXTERN(LogDirectionalLight)

namespace MirageEngine
{
	class ENGINE_API DirectionalLight : public Light
	{
	private:
		Core::Math::Vector3 Direction{ 0, -1, 0 };

	public:
		DirectionalLight();
		~DirectionalLight() = default;

		Core::Math::Vector3 GetDirection() const { return Direction; }

		void SetDirection(Core::Math::Vector3 _direction) { Direction = _direction; }
	};
} // namespace MirageEngine
