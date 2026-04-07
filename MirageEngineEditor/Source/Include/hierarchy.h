#pragma once
#include "i_panel.h"

namespace MirageEngine {
	class SceneGraph;
	class GameObject;
}
namespace Mirage	
{
	class Hierarchy : public IPanel
	{
	private:
		MirageEngine::SceneGraph* m_Context;

	public:
		Hierarchy(const char* _name, MirageEngine::SceneGraph* _context) : IPanel(_name), m_Context(_context) {};

		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;

	private:
		void DrawHierarchyTree();
		void RenderRecursiveNode(MirageEngine::GameObject* _obj);
	};
}