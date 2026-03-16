#include "console.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Console::InitPanel()
	{

	}

	void Console::DrawPanel()
	{
		UIWrapper::BeginWindow("Console");
		UIWrapper::EndWindow();
	}

	void Console::ShutdownPanel()
	{

	}
}