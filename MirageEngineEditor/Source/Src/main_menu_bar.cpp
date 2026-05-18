#include "main_menu_bar.h"
#include <iostream>

namespace Mirage
{
	void MainMenuBar::DrawBar()
	{
		if (ImGui::BeginMainMenuBar())
		{
			FileMenu();
			EditMenu();
			Play();
			ImGui::EndMainMenuBar();
		}
	}

	void MainMenuBar::FileMenu()
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}
	}

	void MainMenuBar::EditMenu()
	{
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}
	}

	void MainMenuBar::Play()
	{
		float menuBarHeight = ImGui::GetFrameHeight();
		float buttonWidth = 100.0f;

		// In same line, put the buton in the middle with the width of the window
		ImGui::SameLine(ImGui::GetWindowWidth() / 2);
		const char* mode = "Play";

		// Play / Pause
		if (ImGui::Button(mode, ImVec2(buttonWidth, menuBarHeight)))
		{
			// Play button action
		}

		// Pause
		if (ImGui::Button("Pause ", ImVec2(buttonWidth, menuBarHeight)))
		{
			// Pause button action
		}
	}
}