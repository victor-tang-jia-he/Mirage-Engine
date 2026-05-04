#include "Components/component.h"
#include "AI/pathfinding.h"
#include <vector>

namespace MirageEngine::Core
{
	namespace Components
	{
		class NavAgent : public Component
		{
		private:
			std::vector<Vector3> m_CurrentPath;
			int m_NextWaypointIndex;
			Vector3 m_TargetDestination;
			AI::NavProfile m_Profile;
			GameObject* m_Owner;

			// Variables to show in inspector
			float m_MovementSpeed = 5.0f;
			float m_RotationSpeed = 10.0f;
			float m_StoppingDistance = 0.1f;
			float m_RecalculateInterval;

		public:
			NavAgent() = default;
			virtual ~NavAgent() = default;

			void Init() override;
			void Update() override;
			entt::id_type GetType() const override { return entt::type_id<NavAgent>().hash(); }

			void SetDestination(Vector3 _newDestination);

			// Getters
			std::vector<Vector3> GetCurrentPath() { return m_CurrentPath; }
			int GetNextWaypointIndex() { return m_NextWaypointIndex; }
			Vector3 GetTargetDestination() { return m_TargetDestination; }
			AI::NavProfile GetProfile() { return m_Profile; }
			float GetMovementSpeed() { return m_MovementSpeed; }
			float GetRotationSpeed() { return m_RotationSpeed; }
			float GetStoppingDistance() { return m_StoppingDistance; }
			float GetRecalculateInterval() { return m_RecalculateInterval; }

			// Setters
			void SetMovementSpeed(float _newMoveSpeed) { m_MovementSpeed = _newMoveSpeed; }
			void SetRotationSpeed(float _newRotationSpeed) { m_RotationSpeed = _newRotationSpeed; }
			void SetStoppingDistance(float _newStoppingDistance) { m_StoppingDistance = _newStoppingDistance; }
			void SetRecalculateInterval(float _newRecalculateInterval) { m_RecalculateInterval = _newRecalculateInterval; }
		};
	}
}