#pragma once

#include <vector4.h>

namespace MirageEngine
{
    enum class LightType
    {
        Directional,
        Spot,
        Point,
        None
    };

    class Light
    {
    protected:
        MirageEngine::Core::Math::Vector4 m_Color = { 1,1,1,1 };
        float m_Intensity = 0.5f;
        LightType m_Type = LightType::None;

    public:
        Light() = default;
        ~Light() = default;

		LightType GetType() const { return m_Type; }
        MirageEngine::Core::Math::Vector4 GetColor() const { return m_Color; }
        float GetIntensity() const { return m_Intensity; }

        void SetColor(MirageEngine::Core::Math::Vector4 _color) { m_Color = _color; }
        void SetIntensity(float _intensity) { m_Intensity = _intensity; }
    };

} // namespace MirageEngine
