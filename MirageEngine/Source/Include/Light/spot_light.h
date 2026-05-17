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
        MirageEngine::Core::Math::Vector3 m_Position;
        MirageEngine::Core::Math::Vector3 m_Direction = { 0.f, -1.f, 0.f };
        float m_InnerCutoff = 0.f;
        float m_OuterCutoff = 0.f;

    public:
        SpotLight();
        SpotLight(GameObject* _owner);
        ~SpotLight() = default;

        static void AttachNew(std::shared_ptr<GameObject> _owner);

        void OnStart() override;

        entt::id_type GetType() const override;
        MirageEngine::Core::Math::Vector3 GetPosition() const { return m_Position; }
        MirageEngine::Core::Math::Vector3 GetDirection() const { return m_Direction; }
        float GetInnerCutoff() const { return m_InnerCutoff; }
        float GetOuterCutoff() const { return m_OuterCutoff; }

        void SetPosition(const MirageEngine::Core::Math::Vector3& _position) { m_Position = _position; }
        void SetDirection(const MirageEngine::Core::Math::Vector3& _direction) { m_Direction = _direction; }
        void SetInnerCutoff(float _inner) { m_InnerCutoff = _inner; }
        void SetOuterCutoff(float _outer) { m_OuterCutoff = _outer; }
    };
} // namespace MirageEngine
