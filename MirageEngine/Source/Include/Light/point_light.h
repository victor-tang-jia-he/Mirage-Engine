#pragma once

#include <Light/i_light.h>

DECLARE_LOG_CATEGORY_EXTERN(LogPointLight)

namespace entt
{
    using id_type = std::uint32_t;
}

namespace MirageEngine
{
    struct ALIGN16 PointLight : public Light, public MirageEngine::Core::Components::Component, public std::enable_shared_from_this<PointLight>
    {
            MirageEngine::Core::Math::Vector3 Position = { 0, 0, 0 };

            PointLight();

            void OnStart() override;

            ~PointLight() = default;

            entt::id_type GetType() const override;
    };
}