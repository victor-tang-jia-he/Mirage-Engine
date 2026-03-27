#include "viewport.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Viewport::InitPanel()
	{

	}

	void Viewport::DrawPanel()
	{
		UIWrapper::BeginWindow("Viewport");
		UIWrapper::EndWindow();
	}

	void Viewport::ShutdownPanel()
	{

	}
}