#pragma once
#include <string>

namespace Mirage
{
	class UIWrapper
	{
	public:
		static void Init(void* windowHandler);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();
	};
}