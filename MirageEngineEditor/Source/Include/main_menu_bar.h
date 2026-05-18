#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Mirage
{
	class MainMenuBar
	{
	public:
		void DrawBar();

	private:
		void FileMenu();
		void EditMenu();

		void Play();
	};
}