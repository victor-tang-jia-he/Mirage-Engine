#include "editor_app.h"
#include "viewport.h"
#include "hierarchy.h"
#include "inspector.h"
#include "console.h"
#include "explorer.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Mirage::EditorApp::Init(void* _windowHandle, MirageEngine::SceneGraph* _engineSceneGraph)
	{
		m_WindowHandle = _windowHandle;;
		UIWrapper::Init(m_WindowHandle);

		m_SceneGraph = _engineSceneGraph;

		m_PanelList.push_back(std::make_unique<Viewport>("Viewport"));
		m_PanelList.push_back(std::make_unique<Hierarchy>("Hierarchy", m_SceneGraph));
		m_PanelList.push_back(std::make_unique<Inspector>("Inspector"));
		m_PanelList.push_back(std::make_unique<Console>("Console"));
		m_PanelList.push_back(std::make_unique<Explorer>("Explorer"));

		for (auto& panel : m_PanelList)
		{
			panel->InitPanel();
		}
	}

	void Mirage::EditorApp::Update(uint32_t _textureID)
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
}