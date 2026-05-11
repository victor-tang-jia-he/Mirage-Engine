
#include "Components/influence_emitter.h"
#include "AI/influence_grid.h"
#include "GameObject/game_object.h"
#include "time_manager.h"
#include "application.h"
#include <entt/entt.hpp>

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
    void InfluenceEmitter::OnStart() {}

    void InfluenceEmitter::OnUpdate(float _dt)
    {
        if (m_Grid == nullptr)
        {
            Application* app = Application::Instance();
            if (app)
            {
                m_Grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();
                if (m_Grid)
                {
                    std::cout << "[ENGINE] SUCCESS: Grid found by Lazy Loading! Address: " << m_Grid << std::endl;
                }
                else
                {
                    static float timer = 0;
                    timer += TimeManager::GetDeltaTime();
                    if (timer > 2.0f)
                    {
                        std::cout << "[ENGINE] SEARCHING: Still no grid in scene..." << std::endl;
                        timer = 0;
                    }
                }
            }
        }

        if (m_Grid)
        {
            Vector3 pos = GetOwner()->GetPosition();
            float strengthToApply = m_Strength * _dt;
            if (m_CachedChannelIndex == -1)
                m_CachedChannelIndex = m_Grid->GetChannelsIndex(m_ChannelName);
            m_Grid->AddInfluenceArea(pos, m_Radius, strengthToApply, m_CachedChannelIndex);
        }
    }

    entt::id_type MirageEngine::Core::Components::InfluenceEmitter::GetType() const 
    { 
        return entt::type_id<InfluenceEmitter>().hash(); 
    }

} // namespace MirageEngine::Core::Components
