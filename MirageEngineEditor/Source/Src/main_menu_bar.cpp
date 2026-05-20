#include "main_menu_bar.h"
#include "editor_app.h"

#include <iostream>

namespace Mirage
{
	void MainMenuBar::DrawBar(Mirage::EditorApp* _editor)
	{
		if (ImGui::BeginMainMenuBar())
		{
			FileMenu();
			EditMenu();
			Play(_editor);
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

	void MainMenuBar::Play(Mirage::EditorApp* _editor)
	{
		float menuBarHeight = ImGui::GetFrameHeight();
		float buttonWidth = 100.0f;

		// In same line, put the buton in the middle with the width of the window
		ImGui::SameLine(ImGui::GetWindowWidth() / 2);

		// Determine button label based on actual editor state
		const char* playButtonLabel = _editor->GetIsPlaying() ? "Stop" : "Play";

		// Play / Pause
		if (ImGui::Button(playButtonLabel, ImVec2(buttonWidth, menuBarHeight)))
		{
			if (!_editor->GetIsPlaying())
			{
				_editor->PlayGame();
			}
			else
			{
				_editor->StopGame();
			}
		}

		// Pause
		const char* pauseButtonLabel = _editor->GetIsPaused() ? "Resume" : "Pause";
		if (ImGui::Button(pauseButtonLabel, ImVec2(buttonWidth, menuBarHeight)))
		{
			_editor->SetIsPaused(!_editor->GetIsPaused());
		}
	}
}