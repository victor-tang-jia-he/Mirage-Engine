#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogGizmo)

namespace MirageEngine
{
	class SceneGraph;

	namespace Core
	{
		class ICamera;
	}
}

namespace MirageEditor::Gizmo
{
	void EditTransform(MirageEngine::SceneGraph* _sceneGraph, std::shared_ptr<MirageEngine::Core::ICamera> _camera);
	void EditView(MirageEngine::SceneGraph* _sceneGraph);
}