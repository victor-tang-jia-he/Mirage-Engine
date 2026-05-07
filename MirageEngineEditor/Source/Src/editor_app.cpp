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

namespace Mirage
{
	void Mirage::EditorApp::Init(void* _windowHandle, MirageEngine::Renderer* _renderer, MirageEngine::SceneGraph* _engineSceneGraph)
	{
		m_WindowHandle = _windowHandle;;
		UIWrapper::Init(m_WindowHandle);

		m_SceneGraph = _engineSceneGraph;

		m_PanelList.push_back(std::make_unique<Viewport>("Viewport"));
		m_PanelList.push_back(std::make_unique<Hierarchy>("Hierarchy", m_SceneGraph, _renderer));
		m_PanelList.push_back(std::make_unique<Inspector>("Inspector", m_SceneGraph));
		m_PanelList.push_back(std::make_unique<Console>("Console"));
		m_PanelList.push_back(std::make_unique<Explorer>("Explorer"));

		for (auto& panel : m_PanelList)
		{
			panel->InitPanel();
		}
	}

	void Mirage::EditorApp::OnUpdate(uint32_t _textureID)
	{
		UIWrapper::BeginFrame();
		m_MainMenuBar.DrawBar();

		for (auto& panel : m_PanelList)
		{
			if (panel->IsPanelActive())
			{
				panel->DrawPanel(_textureID);
			}
		}

		MirageEditor::Gizmo::EditView(m_SceneGraph);
		MirageEditor::Gizmo::EditTransform(m_SceneGraph);

		HandlePlayMode();

		UIWrapper::EndFrame();
	}

	void Mirage::EditorApp::Shutdown()
	{
		for (auto& panel : m_PanelList)
		{
			panel->ShutdownPanel();
		}

		UIWrapper::Shutdown();
	}
	void EditorApp::HandlePlayMode()
	{
		if (ImGui::IsKeyPressed(ImGuiKey_K) && !m_IsPlaying)
		{
			MirageEngine::Serializer::SaveScene(*m_SceneGraph);
			m_IsPlaying = true;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_L))
		{
			m_IsPaused = !m_IsPaused;
		}
		if (ImGui::IsKeyPressed(ImGuiKey_M) && m_IsPlaying)
		{
			m_IsStoped = true;
		}

		if (m_IsStoped)
		{
			m_IsPlaying = false;
			m_IsPaused = false;
			m_IsStoped = false;
		}
	}
}