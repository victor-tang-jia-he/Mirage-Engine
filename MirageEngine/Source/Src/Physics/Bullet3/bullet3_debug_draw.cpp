#include <Physics/Bullet3/bullet3_debug_draw.h>
#include <DebugDraw/debug_draw.h>
#include <vector3.h>


namespace MirageEngine::Core::Physics
{
	using namespace MirageEngine::Core::Math;
	void BulletDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
        Debug_Draw.Line(
            Vector3(from.x(), from.y(), from.z()),
            Vector3(to.x(), to.y(), to.z()),
            Vector3(color.x(), color.y(), color.z())
        );
	}
}