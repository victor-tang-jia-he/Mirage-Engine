#pragma once
#include "i_panel.h"
#include <string>

namespace MirageEngine {
	class SceneGraph;
	class GameObject;
	class Renderer;
}
namespace Mirage	
{
	class Hierarchy : public IPanel
	{
	private:
		MirageEngine::SceneGraph* m_Context;
		MirageEngine::Renderer* m_Renderer;
		bool m_AnItemWasRightClicked;

		std::string m_NextModalToOpen;

	public:
		Hierarchy(const char* _name, MirageEngine::SceneGraph* _context, MirageEngine::Renderer* _renderer = nullptr) : IPanel(_name), m_Context(_context), m_Renderer(_renderer) {};

		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;

	private:
		void DrawHierarchyTree();
		void RenderRecursiveNode(MirageEngine::GameObject* _obj);
		void RenderHierarchyItemMenu();
		void RenderHierarchyEmptyMenu();

		void RenderDeleteModal();
	};
}