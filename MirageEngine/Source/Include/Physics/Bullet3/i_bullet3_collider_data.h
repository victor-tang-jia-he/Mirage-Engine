#pragma once

// Forward declarations
class btCollisionShape;
class btTransform;

namespace MirageEngine::Core::Physics
{
	class IBulletColliderData
	{
	public:
		virtual ~IBulletColliderData() = default;
		virtual btCollisionShape* GetShape() = 0;
		virtual btTransform GetTransform() = 0;
	};
}