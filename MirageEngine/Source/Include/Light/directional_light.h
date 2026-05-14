#pragma once
#include <Scripting/engine_script_api.h>

#include <Light/i_light.h>
#include <Components/component.h>

DECLARE_LOG_CATEGORY_EXTERN(LogDirectionalLight)

namespace MirageEngine
{
	class ENGINE_API DirectionalLight : public Light, public MirageEngine::Core::Components::Component, public std::enable_shared_from_this<DirectionalLight>
	{
	private:
		MirageEngine::Core::Math::Vector3 Direction{ 0, -1, 0 };

	public:
		DirectionalLight();
		DirectionalLight(GameObject* _owner);
		~DirectionalLight() = default;

		void AttachNew(std::shared_ptr<GameObject> _owner);

		void OnStart() override;

		entt::id_type GetType() const override;
		MirageEngine::Core::Math::Vector3 GetDirection() const { return Direction; }

		void SetDirection(MirageEngine::Core::Math::Vector3 _direction) { Direction = _direction; }
	};
} // namespace MirageEngine
