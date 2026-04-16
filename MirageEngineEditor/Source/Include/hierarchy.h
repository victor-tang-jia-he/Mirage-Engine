#pragma once
#include "i_panel.h"
#include <memory>

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
		bool m_AnItemWasRightClicked;

	public:
		Hierarchy(const char* _name, MirageEngine::SceneGraph* _context) : IPanel(_name), m_Context(_context) {};

		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;

	private:
		void DrawHierarchyTree();
		void RenderRecursiveNode(std::shared_ptr<MirageEngine::GameObject> _obj);
		void RenderHierarchyItemMenu();
		void RenderHierarchyEmptyMenu();
	};
}