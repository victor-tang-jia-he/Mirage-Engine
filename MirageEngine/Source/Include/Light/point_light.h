#pragma once
#include <Scripting/engine_script_api.h>
#include <Light/i_light.h>
#include <Components/component.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPointLight)

namespace MirageEngine
{
    class ENGINE_API PointLight : public Light, public MirageEngine::Core::Components::Component, public std::enable_shared_from_this<PointLight>
    {
    private:
            MirageEngine::Core::Math::Vector3 Position = { 0, 0, 0 };

	public:
            PointLight();
			PointLight(GameObject* _owner);
            ~PointLight() = default;

            void AttachNew(std::shared_ptr<GameObject> _owner);

            void OnStart() override;

            entt::id_type GetType() const override;
			MirageEngine::Core::Math::Vector3 GetPosition() const { return Position; }

			void SetPosition(const MirageEngine::Core::Math::Vector3& _position) { Position = _position; }
    };
}