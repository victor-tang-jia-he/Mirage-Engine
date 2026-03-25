#include "explorer.h"
#include "UI_wrapper.h"
#include "application.h"

namespace Mirage
{
	void Explorer::InitPanel()
	{
		Application application;
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