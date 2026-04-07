#include "viewport.h"
#include "UI_wrapper.h"
#include "imgui.h"

namespace Mirage
{
	void Viewport::InitPanel()
	{

	}

	void Viewport::DrawPanel(uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Viewport");
		ImVec2 viewportSize = ImGui::GetContentRegionAvail();
		ImGui::Image((ImTextureID)(intptr_t)_textureID, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
		UIWrapper::EndWindow();
	}

	void Viewport::ShutdownPanel()
	{

	}
}