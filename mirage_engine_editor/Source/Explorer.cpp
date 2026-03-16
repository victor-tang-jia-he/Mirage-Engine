#include "explorer.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Explorer::InitPanel()
	{

	}

	void Explorer::DrawPanel()
	{
		UIWrapper::BeginWindow("Explorer");
		UIWrapper::EndWindow();
	}

	void Explorer::ShutdownPanel()
	{

	}
}