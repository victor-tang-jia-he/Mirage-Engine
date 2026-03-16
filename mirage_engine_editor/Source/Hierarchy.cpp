#include "hierarchy.h"
#include "UI_wrapper.h"

namespace Mirage
{
	void Hierarchy::InitPanel()
	{

	}

	void Hierarchy::DrawPanel()
	{
		UIWrapper::BeginWindow("Hierarchy");
		UIWrapper::EndWindow();
	}

	void Hierarchy::ShutdownPanel()
	{

	}
}