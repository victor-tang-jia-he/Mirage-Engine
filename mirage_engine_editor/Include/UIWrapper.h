#pragma once
#include <string>
#include "vector3.h"

namespace Math = Bwt::Core::Maths;

namespace Mirage
{
	class UIWrapper
	{
	public:
		static void Init(void* windowHandler);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		// window
		static bool BeginWindow(const char* windowName, bool* isOpen = nullptr);
		static void EndWindow();

		// Widgets
		static void CreateSeparatorText(const char* separatorName);
		static void CreateColorPicker(Math::Vector3* color);

		static bool CreateSliderInt(const char* sliderName, int* value, int vMin, int vMax);
		static bool CreateSliderFloat(const char* sliderName, float* value, float vMin, float vMax);
		static bool CreateButton(const char* buttonName);
		static bool CreateCheckbox(const char* checkBoxName, bool* check);
	};
}