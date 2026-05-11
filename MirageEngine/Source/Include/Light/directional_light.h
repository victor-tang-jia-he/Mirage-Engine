#pragma once

#include <Light/i_light.h>
DECLARE_LOG_CATEGORY_EXTERN(LogDirectionalLight)

namespace MirageEngine
{
    struct ALIGN16 DirectionalLight : public Light, public MirageEngine::Core::Components::Component, public std::enable_shared_from_this<DirectionalLight>
    {
            MirageEngine::Core::Math::Vector3 Direction = { 0, -1, 0 };

            DirectionalLight();
            ~DirectionalLight() = default;

            void OnStart() override;

            entt::id_type GetType() const override;
    };
} // namespace MirageEngine
