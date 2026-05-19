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
			if (ImGui::MenuItem("New")) {}
			if (ImGui::MenuItem("Open", "Ctrl+O")) {}
			if (ImGui::BeginMenu("Open Recent"))
			{
				ImGui::MenuItem("fish_hat.c");
				ImGui::MenuItem("fish_hat.inl");
				ImGui::MenuItem("fish_hat.h");
				if (ImGui::BeginMenu("More.."))
				{
					ImGui::MenuItem("Hello");
					ImGui::MenuItem("Sailor");
					if (ImGui::BeginMenu("Recurse.."))
					{
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Save", "Ctrl+S")) {}
			if (ImGui::MenuItem("Save As..")) {}

			ImGui::Separator();
			if (ImGui::BeginMenu("Options"))
			{
				static bool enabled = true;
				ImGui::MenuItem("Enabled", "", &enabled);
				ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Borders);
				for (int i = 0; i < 10; i++)
					ImGui::Text("Scrolling Text %d", i);
				ImGui::EndChild();
				static float f = 0.5f;
				static int n = 0;
				ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
				ImGui::InputFloat("Input", &f, 0.1f);
				ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
	}

	void MainMenuBar::EditMenu()
	{
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
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