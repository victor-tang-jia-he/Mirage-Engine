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

		
		MirageEngine::SceneGraph* m_SceneGraph;

		// Play mode variables
		bool m_IsPlaying = false;
		bool m_IsPaused = false;

		int m_GameCameraIndex = 0;

	public:
		static std::shared_ptr <MirageEditor::EditorCamera> EditorCamera;
		void Init(void* _windowHandle, MirageEngine::Renderer* _renderer, MirageEngine::SceneGraph* _engineSceneGraph);
		void OnUpdate(uint32_t _editorTextureID, uint32_t _gameTextureID);
		void Shutdown();

		void SaveScene();
		void LoadScene();
		void PlayGame();
		void StopGame();

		void SetSelectedObject(std::shared_ptr<MirageEngine::GameObject> _object) { m_SelectedObject = _object; };
		
		// Getters
		std::shared_ptr<MirageEngine::GameObject> GetSelectedObject() const { return m_SelectedObject; };
		bool GetIsPlaying() const { return m_IsPlaying; }
		bool GetIsPaused() const { return m_IsPaused; }
		
		// Setters
		void SetIsPlaying(bool _isPlaying) { m_IsPlaying = _isPlaying; }
		void SetIsPaused(bool _isPaused) { m_IsPaused = _isPaused; }

		std::shared_ptr <MirageEditor::EditorCamera> GetEditorCamera() const { return EditorCamera; }
	};
}