#pragma once

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogGuizmo)

namespace MirageEngine
{
	class SceneGraph;
}

namespace MirageEditor::Guizmo
{
	void EditTransform(MirageEngine::SceneGraph* _sceneGraph);
}