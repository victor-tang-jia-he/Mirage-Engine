#pragma once
#include <string>
#include "vector3.h"

namespace Math = MirageEngine::Core::Math;

namespace Mirage
{
	class UIWrapper
	{
	public:
		static void Init(void* _windowHandle);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		// window
		static bool BeginWindow(const char* _windowName, bool* _isOpen = nullptr);
		static void EndWindow();

		// Widgets
		static void CreateSeparatorText(const char* _separatorName);
		static void CreateColorPicker(Math::Vector3* _color);

		static bool CreateSliderInt(const char* _sliderName, int* _value, int _vMin, int _vMax);
		static bool CreateSliderFloat(const char* _sliderName, float* _value, float _vMin, float _vMax);
		static bool CreateButton(const char* _buttonName);
		static bool CreateCheckbox(const char* _checkBoxName, bool* _check);
	};
}