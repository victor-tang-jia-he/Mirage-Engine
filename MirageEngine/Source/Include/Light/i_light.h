#pragma once

#include <matrix4.h>
#include <Components/component.h>

namespace MirageEngine
{
    enum class LightType
    {
        Directional,
        Spot,
        Point,
        None
    };

    struct ALIGN16 Light
    {
            MirageEngine::Core::Math::Vector4 Color = { 1, 1, 1, 1 };
            float Intensity = 0.5f;
            LightType Type = LightType::None;
    };

} // namespace MirageEngine
