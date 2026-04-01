#include "inspector.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Inspector::InitPanel()
	{

	}

	void Inspector::DrawPanel(uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Inspector");
		UIWrapper::EndWindow();
	}

	void Inspector::ShutdownPanel()
	{

	}
}