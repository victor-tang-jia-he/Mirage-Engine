#pragma once
#include <i_panel.h>
#include <main_menu_bar.h>
#include <iostream>
#include <vector>
#include <memory>
#include <editor_camera.h>
#include <SceneGraph/scene_graph.h>

DECLARE_LOG_CATEGORY_EXTERN(logEditorApp)

namespace MirageEngine
{
	class GameObject;
}

namespace Mirage
{
	class EditorApp
	{
	private:
		std::shared_ptr<MirageEngine::GameObject> m_SelectedObject = nullptr;
		std::vector<std::unique_ptr<IPanel>> m_PanelList;
		void* m_WindowHandle = nullptr;
		MainMenuBar m_MainMenuBar;

		std::shared_ptr <MirageEditor::EditorCamera> m_EditorCamera;
		MirageEngine::SceneGraph* m_SceneGraph;

		// Play mode variables
		bool m_IsPlaying = false;
		bool m_IsPaused = false;
		bool m_IsStoped = false;

	public:
		void Init(void* _windowHandle, MirageEngine::Renderer* _renderer, MirageEngine::SceneGraph* _engineSceneGraph);
		void OnUpdate(uint32_t _textureID);
		void Shutdown();

		void HandlePlayMode();

		void SetSelectedObject(std::shared_ptr<MirageEngine::GameObject> _object) { m_SelectedObject = _object; };
		std::shared_ptr<MirageEngine::GameObject> GetSelectedObject() const { return m_SelectedObject; };
		bool IsPlaying() const { return m_IsPlaying; }
		bool IsPaused() const { return m_IsPaused; }
		bool IsStopped() const { return m_IsStoped; }

		std::shared_ptr <MirageEditor::EditorCamera> GetEditorCamera() const { return m_EditorCamera; }
	};
}