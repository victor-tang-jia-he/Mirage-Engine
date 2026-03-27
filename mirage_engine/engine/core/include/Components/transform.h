#include <quaternion.h>

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class Transform
			{
				MirageEngine::Core::Math::Vector3 position{ 0,0,0 };
				MirageEngine::Core::Math::Vector3 scale{ 1,1,1 };
				MirageEngine::Core::Math::Quaternion rotation{ 0,0,0 };

				MirageEngine::Core::Math::Matrix4 localMatrix;
				bool dirty = true;

				// utility
				MirageEngine::Core::Math::Vector3 Rotate(const MirageEngine::Core::Math::Quaternion& q, const MirageEngine::Core::Math::Vector3& v) const;

			public:
				void LookAt(const MirageEngine::Core::Math::Vector3& target);

				MirageEngine::Core::Math::Vector3 Forward() const;
				MirageEngine::Core::Math::Vector3 Up() const;
				MirageEngine::Core::Math::Vector3 Right() const;
				MirageEngine::Core::Math::Vector3 Left() const;

				void SetPosition(MirageEngine::Core::Math::Vector3 _pos);
				void SetScale(MirageEngine::Core::Math::Vector3 _scale);
				void SetRotation(MirageEngine::Core::Math::Quaternion _rotation);

				MirageEngine::Core::Math::Matrix4 GetMatrix();
				MirageEngine::Core::Math::Vector3 GetPosition() const { return position; }
				MirageEngine::Core::Math::Vector3 GetScale() const { return scale; }
				MirageEngine::Core::Math::Quaternion GetRotation()  const { return rotation; }
				MirageEngine::Core::Math::Vector3 GetRotationEuler() const { return rotation.ToEulerAngle() ; }

			};
		}
	}
}