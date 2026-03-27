#include "explorer.h"
#include "UI_wrapper.h"
#include "console.h"
#include <fstream>
#include <iostream>

namespace Mirage
{
	void Explorer::InitPanel()
	{
		m_RootPath = ASSETS_PATH;
		m_CurrentPath = m_RootPath;
		ScanProjectDirectory(m_CurrentPath);
	}

	void Explorer::DrawPanel()
	{
		UIWrapper::BeginWindow("Explorer");

		ImGui::BeginChild("Sidebar", ImVec2(150, 0), true); //LEFT->SideBar
		DrawFolderTree();
		ImGui::EndChild();

		ImGui::SameLine();

		ImGui::BeginChild("Content", ImVec2(0, 0), false); //RIGHT->Content
		DrawDirectoryContent();
		ImGui::EndChild();

		UIWrapper::EndWindow();
	}

	void Explorer::ShutdownPanel()
	{

	}

	void Explorer::ScanProjectDirectory(const std::filesystem::path _rootPath)
	{
		m_Entries.clear();
		if(std::filesystem::exists(_rootPath) && std::filesystem::is_directory(_rootPath))
		{
			for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(_rootPath))
			{
				AssetEntry newEntry;
				newEntry.Name = entry.path().filename().string();
				newEntry.Path = entry.path();
				if (entry.is_directory())
					newEntry.Type = AssetType::Folder;
				else
				{
					auto ext = entry.path().extension();
					if (ext == ".png" || ext == ".jpg") newEntry.Type = AssetType::Texture;
					else if (ext == ".obj") newEntry.Type = AssetType::Model;
					else if (ext == ".cs" || ext == ".py") newEntry.Type = AssetType::Script;
					else if (ext == ".glsl" || ext == ".frag" || ext == ".vert") newEntry.Type = AssetType::Shader;
					else newEntry.Type = AssetType::Unknown;
				}
				m_Entries.push_back(newEntry);
			}
		}
	}

	void Explorer::DrawFolderTree()
	{
		RenderRecursiveFolder(m_RootPath);
	}

	void Explorer::RenderRecursiveFolder(const std::filesystem::path& _path)
	{
		std::error_code ec;
		auto it = std::filesystem::directory_iterator(_path, ec);

		if (ec) return;

		for (const auto& entry : it)
		{
			if (entry.is_directory())
			{
				std::string name = entry.path().filename().string();
				std::string id = entry.path().string();

				bool isLeaf = true;
				for (const auto& sub : std::filesystem::directory_iterator(entry.path())) {
					if (sub.is_directory()) {
						isLeaf = false;
						break;
					}
				}

				bool isSelected = false;
				if (m_CurrentPath == entry.path()) isSelected = true;

				if (UIWrapper::BeginFolderNode(name.c_str(), id.c_str(), isSelected, isLeaf))
				{
					if (UIWrapper::IsItemClicked())
					{
						this->ChangeDirectory(entry.path());
					}

					RenderRecursiveFolder(entry.path());

					UIWrapper::EndFolderNode();
				}
				else
				{
					if (UIWrapper::IsItemClicked()) {
						this->ChangeDirectory(entry.path());
					}
				}
			}
		}
	}

	void Explorer::ChangeDirectory(const std::filesystem::path& _newPath)
	{
		m_SelectedEntry = nullptr;
		m_CurrentPath = _newPath;
		ScanProjectDirectory(m_CurrentPath);
	}

	void Explorer::DrawDirectoryContent()
	{
		m_AnItemWasRightClicked = false;
		RenderAssetGrid();
		if(ImGui::IsWindowHovered() && !m_AnItemWasRightClicked && ImGui::IsMouseClicked(1))
			UIWrapper::RequestModal("EmptySpaceMenu");
		UIWrapper::UpdateModalRequests();
		RenderAssetContextMenu();
		RenderEmptySpaceContextMenu();
		RenderRenameModal();
	}

	void Explorer::HandleFileClick()
	{
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::Button("Close"))
				ImGui::CloseCurrentPopup();
			ImGui::EndPopup();
		}
		
	}

	void Explorer::RenderAssetGrid()
	{
		float windowSize = ImGui::GetContentRegionAvail().x;

		if (UIWrapper::BeginAssetGrid(100.0f, windowSize))
		{
			for (const AssetEntry& entry : m_Entries)
			{
				bool isSelected = (m_SelectedEntry == &entry);
				WidgetAction action = UIWrapper::DrawAssetTile(entry.Name.c_str(), isSelected);

				if (action == WidgetAction::Clicked)
				{
					m_SelectedEntry = (void*)&entry;
				}

				if (action == WidgetAction::DoubleClicked && entry.Type == AssetType::Folder)
				{
					this->ChangeDirectory(entry.Path);
					break;
				}

				if (action == WidgetAction::RightClicked)
				{
					m_EntryToProcess = const_cast<AssetEntry*>(&entry);
					UIWrapper::RequestModal("AssetContextMenu");
					m_AnItemWasRightClicked = true;
					
				}
			}
			UIWrapper::EndAssetGrid();
			ImGui::Dummy(ImGui::GetContentRegionAvail());
		}
	}

	void Explorer::RenderAssetContextMenu()
	{
		if (ImGui::BeginPopup("AssetContextMenu"))
		{
			if (m_EntryToProcess)
			{
				ImGui::TextDisabled("Options: %s", m_EntryToProcess->Name.c_str());
				ImGui::Separator();

				if (ImGui::MenuItem("Rename"))
				{
					strncpy_s(m_RenameBuffer, m_EntryToProcess->Name.c_str(), sizeof(m_RenameBuffer));
					UIWrapper::RequestModal("RenameModal");
				}

				if (ImGui::MenuItem("Delete"))
				{
					std::error_code ec;
					std::filesystem::remove_all(m_EntryToProcess->Path, ec);
					ScanProjectDirectory(m_CurrentPath);
					m_EntryToProcess = nullptr;
				}
			}
			ImGui::EndPopup();
		}
	}

	void Explorer::RenderEmptySpaceContextMenu()
	{
		if (!m_AnItemWasRightClicked)
		{
			if (ImGui::BeginPopup("EmptySpaceMenu"))
			{
				if (ImGui::MenuItem("New Folder"))
				{
					std::error_code ec;
					std::filesystem::create_directory(m_CurrentPath / "NewFolder", ec);
					ScanProjectDirectory(m_CurrentPath);
				}
				if (ImGui::MenuItem("Cancel"))
				{
					ImGui::CloseCurrentPopup();
				}
				ImGui::EndPopup();
			}
		}
	}

	void Explorer::RenderRenameModal()
	{
		if (ImGui::BeginPopupModal("RenameModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter new name:");
			ImGui::InputText("##newName", m_RenameBuffer, sizeof(m_RenameBuffer));

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				if (m_EntryToProcess)
				{
					std::filesystem::path newPath = m_EntryToProcess->Path.parent_path() / m_RenameBuffer;
					RenameItem(m_EntryToProcess->Path, newPath);
					ScanProjectDirectory(m_CurrentPath);
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Explorer::RenameItem(std::filesystem::path& _oldPath, std::filesystem::path& _newPath)
	{
		std::error_code ec;
		std::filesystem::rename(_oldPath, _newPath);

		if (ec)
		{
			std::cout << "Error during renaming" << ec.message() << std::endl;
		}
	}
}