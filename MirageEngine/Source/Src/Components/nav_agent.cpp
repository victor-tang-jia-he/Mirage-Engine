#include "Components/nav_agent.h"
#include "GameObject/game_object.h"
#include "time_manager.h"

namespace MirageEngine::Core::Components
{
	void NavAgent::Init()
	{
		m_Owner = GetOwner();
	}

	void NavAgent::Update()
	{
		if (m_CurrentPath.empty()) return;
		
		Vector3 targetPos = m_CurrentPath[m_NextWaypointIndex];
		Vector3 myPos = m_Owner->Transform.GetPosition();

		float distance = m_Owner->Transform.GetPosition().Distance(targetPos);
		if (distance < m_StoppingDistance)
		{
			m_NextWaypointIndex++;
			if (m_NextWaypointIndex >= m_CurrentPath.size())
			{
				m_CurrentPath.clear();
				return;
			}
		}

		Vector3 direction = (targetPos - myPos).Normalized();
		float dt = TimeManager::GetDeltaTime();
		Vector3 newPos = myPos + (direction * m_MovementSpeed * dt);
		m_Owner->Transform.SetPosition(newPos);
		m_Owner->Transform.LookAt(targetPos);
	}

	void NavAgent::SetDestination(Vector3 _newDestination)
	{

	}
}