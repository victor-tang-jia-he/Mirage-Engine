#pragma once

#include <Light/i_light.h>

DECLARE_LOG_CATEGORY_EXTERN(LogSpotLight)

namespace MirageEngine
{
    struct ALIGN16 SpotLight : public Light, public MirageEngine::Core::Components::Component, public std::enable_shared_from_this<SpotLight>
    {
            MirageEngine::Core::Math::Vector3 Position;
            MirageEngine::Core::Math::Vector3 Direction;
            float InnerCutoff = 0;
            float OuterCutoff = 0;

            SpotLight();
            ~SpotLight() = default;

            void OnStart() override;

            entt::id_type GetType() const override;
    };
} // namespace MirageEngine
