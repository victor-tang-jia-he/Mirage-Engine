#include <Components/nav_agent.h>
#include <GameObject/game_object.h>
#include <AI/pathfinding.h>
#include <AI/influence_grid.h>
#include <time_manager.h>
#include <application.h>
#include <entt/entt.hpp>
#include <DebugDraw/debug_draw.h>

namespace MirageEngine::Core
{
	namespace Components
	{
		void NavAgent::OnStart()
		{
		}

		void NavAgent::OnUpdate(float _dt)
		{
			if (m_CurrentPath.empty()) return;

			Vector3 targetPos = m_CurrentPath[m_NextWaypointIndex];
			Vector3 myPos = GetOwner()->Transform.GetPosition();

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

			Vector3 newPos = myPos + (direction * m_MovementSpeed * _dt);

			m_Owner->SetPosition(newPos);
			m_Owner->Transform.LookAt(targetPos);
		}

		void NavAgent::OnDrawLine()
		{
			if (m_CurrentPath.empty()) return;

			for (size_t i = 0; i < m_CurrentPath.size() - 1; ++i)
			{
				Vector3 start = m_CurrentPath[i];
				Vector3 end = m_CurrentPath[i + 1];

				start.Y += 0.2f;
				end.Y += 0.2f;

				Debug_Draw.Line(start, end, { 0.2f, 0.4f, 1.0f, 1.0f });
			}

			Vector3 currentTarget = m_CurrentPath[m_NextWaypointIndex];
			Debug_Draw.Line(currentTarget, currentTarget + Vector3(0, 2, 0), { 0, 1, 0, 1 });
		}

		void NavAgent::AttachNew(std::shared_ptr<GameObject> _owner)
		{
			std::shared_ptr<NavAgent> compPtr = std::make_shared<NavAgent>(_owner.get());
			_owner->AddComponent<NavAgent>(compPtr);
		}

		entt::id_type NavAgent::GetType() const
		{ 
			return entt::type_id<NavAgent>().hash(); 
		}

		void NavAgent::SetDestination(Vector3 _newDestination)
		{
			m_TargetDestination = _newDestination;

			Application* app = Application::Instance();
			if (!app) return;

			InfluenceGrid* grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();

			if (grid)
			{
				AI::Pathfinding pathfinder;

				m_CurrentPath = pathfinder.FindPath(m_Owner->Transform.GetPosition(), m_TargetDestination, grid, m_Profile);
				m_NextWaypointIndex = 0;
			}
		}
	}
}