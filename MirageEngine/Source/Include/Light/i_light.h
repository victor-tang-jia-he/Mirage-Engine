#pragma once

#include <vector4.h>

namespace MirageEngine
{
    enum class LightType
    {
        DIRECTIONNAL,
        SPOT,
        POINT,
        NONE
    };

    class Light
    {
    protected:
        Core::Math::Vector4 p_Color = { 1,1,1,1 };
        float p_Intensity = 0.5f;
        LightType p_Type = LightType::NONE;

    public:
        Light() = default;
        ~Light() = default;

		LightType GetType() const { return p_Type; }
        Core::Math::Vector4 GetColor() const { return p_Color; }
        float GetIntensity() const { return p_Intensity; }

        void SetColor(Core::Math::Vector4 _color) { p_Color = _color; }
        void SetIntensity(float _intensity) { p_Intensity = _intensity; }
    };

} // namespace MirageEngine
