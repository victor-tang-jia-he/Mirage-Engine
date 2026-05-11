#pragma once
#include <iostream>

#include "AI/influence_grid.h"

namespace MirageEngine::Core::Components
{
    class InfluenceEmitter : public Component
    {
        private:

            std::string m_ChannelName;
            float m_Radius = 5.0f;
            float m_Strength = 10.0f;
            int m_CachedChannelIndex = -1;

            InfluenceGrid* m_Grid = nullptr;

        public:

            InfluenceEmitter() = default;
            virtual ~InfluenceEmitter() = default;

            void OnStart() override;
            void OnUpdate(float _dt) override;
            entt::id_type GetType() const override;

            // Getters
            std::string GetInfluenceType() const { return m_ChannelName; }
            float GetRadius() const { return m_Radius; }
            float GetStrength() const { return m_Strength; }
            InfluenceGrid* GetTargetGrid() const { return m_Grid; }

            // Setters
            void SetInfluenceType(std::string _type)
            {
                m_ChannelName = _type;
                m_CachedChannelIndex = -1;
            }
            void SetRadius(float _radius) { m_Radius = _radius; }
            void SetStrength(float _strength) { m_Strength = _strength; }
            void SetTargetGrid(InfluenceGrid* _grid) { m_Grid = _grid; }
    };
} // namespace MirageEngine::Core::Components
