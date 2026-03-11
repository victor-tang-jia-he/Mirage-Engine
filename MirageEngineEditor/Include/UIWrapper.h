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

		// Widgets
		static bool CreateSliderInt(const char* sliderName, int* value, int vMin, int vMax);
		static bool CreateSliderFloat(const char* sliderName, float* value, float vMin, float vMax);
		static bool CreateButton(const char* buttonName);
		static bool CreateCheckbox(const char* checkBoxName, bool* check);
	};
}