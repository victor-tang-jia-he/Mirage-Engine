#include "hierarchy.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Hierarchy::InitPanel()
	{

	}

	void Hierarchy::DrawPanel(uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Hierarchy");
		UIWrapper::EndWindow();
	}

	void Hierarchy::ShutdownPanel()
	{

	}
}