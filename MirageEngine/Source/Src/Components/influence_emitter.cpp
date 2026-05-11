
#include "Components/influence_emitter.h"
#include "AI/influence_grid.h"
#include "GameObject/game_object.h"
#include "time_manager.h"
#include "application.h"

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
	void InfluenceEmitter::Init()
	{
	}

	void InfluenceEmitter::Update()
	{
		if (m_Grid == nullptr)
		{
			Application* app = Application::Instance();
			if (app)
			{
				m_Grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();
			}
		}

		if (m_Grid)
		{
			Vector3 pos = GetOwner()->GetPosition();
			float strengthToApply = m_Strength * TimeManager::GetDeltaTime();
			if(m_CachedChannelIndex == -1) m_CachedChannelIndex = m_Grid->GetChannelsIndex(m_ChannelName); 
			m_Grid->AddInfluenceArea(pos, m_Radius, strengthToApply, m_CachedChannelIndex);
		}
	}

	void InfluenceEmitter::AttachMeta(std::shared_ptr<GameObject> _Owner)
	{
		std::shared_ptr<InfluenceEmitter> compPtr = std::make_shared<InfluenceEmitter>(*this);
		_Owner->AddComponent<InfluenceEmitter>(compPtr);
	}

	void MirageEngine::Core::Components::InfluenceEmitter::AttachNew(std::shared_ptr<GameObject> _owner)
	{
		std::shared_ptr<InfluenceEmitter> compPtr = std::make_shared<InfluenceEmitter>(_owner.get());
		_owner->AddComponent<InfluenceEmitter>(compPtr);
	}
}