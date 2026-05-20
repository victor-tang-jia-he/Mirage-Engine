#include "editor_app.h"
#include "viewport.h"
#include "hierarchy.h"
#include "inspector.h"
#include "console.h"
#include "explorer.h"
#include "UI_wrapper.h"
#include "gizmo.h"
#include "renderer.h"
#include "Serialization/serializer.h"
#include "Serialization/deserializer.h"
#include "Physics/physics_manager.h"
#include "Light/light_manager.h"

#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogEditorApp, Loggers::Color::CYAN);

namespace Mirage
{
	std::shared_ptr<MirageEditor::EditorCamera> EditorApp::EditorCamera = nullptr;

	void Mirage::EditorApp::Init(void* _windowHandle, MirageEngine::Renderer* _renderer, MirageEngine::SceneGraph* _engineSceneGraph)
	{
		m_WindowHandle = _windowHandle;;
		UIWrapper::Init(m_WindowHandle);

		m_SceneGraph = _engineSceneGraph;

		m_PanelList.push_back(std::make_unique<Viewport>("Game", Viewport::Type::Editor));
		m_PanelList.push_back(std::make_unique<Viewport>("Viewport", Viewport::Type::Game));
		m_PanelList.push_back(std::make_unique<Hierarchy>("Hierarchy", m_SceneGraph, _renderer));
		m_PanelList.push_back(std::make_unique<Inspector>("Inspector", m_SceneGraph));
		m_PanelList.push_back(std::make_unique<Console>("Console"));
		m_PanelList.push_back(std::make_unique<Explorer>("Explorer"));

		for (auto& panel : m_PanelList)
		{
			panel->InitPanel();
			LOG(logTemp, LogEditorApp, "Initializing panel: ", panel->GetTitle());
		}

		EditorCamera = std::make_shared<MirageEditor::EditorCamera>();
		EditorCamera->BindDefaultInput();
		EditorCamera->SetPosition({ 0.f, 0.f, 5.f });
	}

	void Mirage::EditorApp::OnUpdate(uint32_t _editorTextureID, uint32_t _gameTextureID)
	{
		UIWrapper::BeginFrame();
		m_MainMenuBar.DrawBar(this);

		for (auto& panel : m_PanelList)
		{
			if (auto* vp = dynamic_cast<Viewport*>(panel.get()))
			{
				if (vp->GetViewportType() == Viewport::Type::Game)
					vp->DrawPanel(_gameTextureID);
				else
				{
					vp->DrawPanel(_editorTextureID);
				}
			}
			else
			{
				panel->DrawPanel(_editorTextureID);
			}
		}

		MirageEditor::Gizmo::EditTransform(m_SceneGraph, EditorCamera);

		UIWrapper::EndFrame();
	}

	void Mirage::EditorApp::Shutdown()
	{
		for (auto& panel : m_PanelList)
		{
			panel->ShutdownPanel();
			LOG(logTemp, LogEditorApp, "Shutting down panel: ", panel->GetTitle());
		}

		UIWrapper::Shutdown();

	}

	void EditorApp::SaveScene()
	{
		MirageEngine::Serializer::SaveScene(*m_SceneGraph);
	}

	void EditorApp::LoadScene()
	{
		m_SceneGraph->ClearScene();
		*m_SceneGraph = MirageEngine::SceneGraph();
		MirageEngine::LightManager::Instance().ShutDown();
		MirageEngine::Core::Physics::PhysicsManager::Instance().Initialize();
		*m_SceneGraph = *MirageEngine::Deserializer::LoadSceneFromFile("Assets/Scenes/scene.json");
	}

	void EditorApp::PlayGame()
	{
		SaveScene();
		m_IsPlaying = true;
		LOG(logTemp, LogEditorApp, "Play mode started");
	}

	void EditorApp::StopGame()
	{
		LoadScene();
		m_IsPlaying = false;
		m_IsPaused = false;
		LOG(logTemp, LogEditorApp, "Play mode stopping");
	}
}