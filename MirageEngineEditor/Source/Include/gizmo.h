#pragma once

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogGizmo)

namespace MirageEngine
{
	class SceneGraph;
}

namespace MirageEditor::Gizmo
{
	void EditTransform(MirageEngine::SceneGraph* _sceneGraph);
	void EditView(MirageEngine::SceneGraph* _sceneGraph);
}