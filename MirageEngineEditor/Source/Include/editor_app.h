#pragma once
#include "i_panel.h"
#include "main_menu_bar.h"
#include <iostream>
#include <vector>
#include <memory>
#include "SceneGraph/scene_graph.h" 

namespace MirageEngine
{
	class GameObject;
}

namespace Mirage
{
	class EditorApp
	{
	private:
		MirageEngine::GameObject* m_SelectedObject = nullptr;
		std::vector<std::unique_ptr<IPanel>> m_PanelList;
		void* m_WindowHandle = nullptr;
		MainMenuBar m_MainMenuBar;
		MirageEngine::SceneGraph* m_SceneGraph;

	public:
		void Init(void* _windowHandle, MirageEngine::SceneGraph* _engineSceneGraph);
		void Update(uint32_t _textureID);
		void Shutdown();

		void SetSelectedObject(MirageEngine::GameObject* _object) { m_SelectedObject = _object; };
		MirageEngine::GameObject* GetSelectedObject() const { return m_SelectedObject; };
	};
}