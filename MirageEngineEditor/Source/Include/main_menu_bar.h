#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <memory>

namespace Mirage
{
	class EditorApp;

	class MainMenuBar
	{
	public:
		void DrawBar(Mirage::EditorApp* _editor);

	private:
		void FileMenu(Mirage::EditorApp* _editor);
		void EditMenu();

		void Play(Mirage::EditorApp* _editor);
	};
}