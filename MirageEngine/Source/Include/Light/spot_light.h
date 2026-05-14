#pragma once
#include <Scripting/engine_script_api.h>
#include <Light/i_light.h>
#include <Components/component.h>

DECLARE_LOG_CATEGORY_EXTERN(LogSpotLight)

namespace MirageEngine
{
    class ENGINE_API SpotLight : public MirageEngine::Core::Components::Component, public Light, public std::enable_shared_from_this<SpotLight>
    {
    private:
        MirageEngine::Core::Math::Vector3 Position;
        MirageEngine::Core::Math::Vector3 Direction = { 0, -1, 0 };
        float InnerCutoff = 0;
        float OuterCutoff = 0;

    public:
        SpotLight();
        SpotLight(GameObject* _owner);
        ~SpotLight() = default;

        void AttachNew(std::shared_ptr<GameObject> _owner);

        void OnStart() override;

        entt::id_type GetType() const override;
        MirageEngine::Core::Math::Vector3 GetPosition() const { return Position; }
        MirageEngine::Core::Math::Vector3 GetDirection() const { return Direction; }
        float GetInnerCutoff() const { return InnerCutoff; }
        float GetOuterCutoff() const { return OuterCutoff; }

        void SetPosition(const MirageEngine::Core::Math::Vector3& _position) { Position = _position; }
        void SetDirection(const MirageEngine::Core::Math::Vector3& _direction) { Direction = _direction; }
        void SetInnerCutoff(float _inner) { InnerCutoff = _inner; }
        void SetOuterCutoff(float _outer) { OuterCutoff = _outer; }
    };
} // namespace MirageEngine
