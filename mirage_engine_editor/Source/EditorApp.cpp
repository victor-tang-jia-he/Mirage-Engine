#include "../Include/EditorApp.h"
#include "../Include/Viewport.h"
#include "../Include/Hierarchy.h"
#include "../Include/Inspector.h"
#include "../Include/Console.h"
#include "../Include/Explorer.h"
#include "../Include/UIWrapper.h"

namespace Mirage
{
	void Mirage::EditorApp::Init(void* windowHandle)
	{
		WindowHandle = windowHandle;
		UIWrapper::Init(WindowHandle);

		PanelList.push_back(std::make_unique<Viewport>("Viewport"));
		PanelList.push_back(std::make_unique<Hierarchy>("Hierarchy"));
		PanelList.push_back(std::make_unique<Inspector>("Inspector"));
		PanelList.push_back(std::make_unique<Console>("Console"));
		PanelList.push_back(std::make_unique<Explorer>("Explorer"));

		for (auto& panel : PanelList)
		{
			panel->InitPanel();
		}
	}

	void Mirage::EditorApp::Update()
	{
		UIWrapper::BeginFrame();

		for (auto& panel : PanelList)
		{
			if (panel->IsPanelActive())
			{
				panel->DrawPanel();
			}
		}

		UIWrapper::EndFrame();
	}

	void Mirage::EditorApp::Shutdown()
	{
		for (auto& panel : PanelList)
		{
			panel->ShutdownPanel();
		}

		UIWrapper::Shutdown();
	}
}