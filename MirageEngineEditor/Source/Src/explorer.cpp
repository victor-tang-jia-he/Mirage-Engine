#include "explorer.h"
#include "UI_wrapper.h"
#include "editor_actions.h"
#include "console.h"
#include "dragdrop.h"
#include <fstream>
#include <iostream>

namespace Mirage
{
	void Explorer::InitPanel()
	{
		ShowFolderContent();
		m_CurrentPath = m_RootPath;
		ScanProjectDirectory(m_CurrentPath);

		std::cout << "[Explorer] Root Path detecte : " << m_RootPath << std::endl;
	}

	void Explorer::DrawPanel(uint32_t _textureID)
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
		std::string name = "Assets";
		std::string id = m_RootPath.string();

		bool isLeaf = true;
		for (const auto& sub : std::filesystem::directory_iterator(m_RootPath)) {
			if (sub.is_directory()) {
				isLeaf = false;
				break;
			}
		}

		bool isSelected = (m_CurrentPath == m_RootPath);

		if (UIWrapper::BeginFolderNode(name.c_str(), id.c_str(), isSelected, isLeaf))
		{
			if (ImGui::BeginDragDropTarget())
			{
				auto droppedPath = DragDrop::GetPathPayload(DragDropTag::CONTENT_BROWSER_ITEM);
				if (!droppedPath.empty())
				{
					this->MoveItem(droppedPath, m_RootPath);
				}
				ImGui::EndDragDropTarget();
			}

			if (UIWrapper::IsItemClicked())
			{
				this->ChangeDirectory(m_RootPath);
			}

			RenderRecursiveFolder(m_RootPath);

			UIWrapper::EndFolderNode();
		}
		else
		{
			if (UIWrapper::IsItemClicked()) {
				this->ChangeDirectory(m_RootPath);
			}
		}
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

				bool bisLeaf = true;
				for (const auto& sub : std::filesystem::directory_iterator(entry.path())) {
					if (sub.is_directory()) {
						bisLeaf = false;
						break;
					}
				}

				bool bisSelected = (m_CurrentPath == entry.path());

				if (UIWrapper::BeginFolderNode(name.c_str(), id.c_str(), bisSelected, bisLeaf))
				{
					if (ImGui::BeginDragDropTarget())
					{
						auto droppedPath = DragDrop::GetPathPayload(DragDropTag::CONTENT_BROWSER_ITEM);
						if (!droppedPath.empty())
						{
							this->MoveItem(droppedPath, entry.path());
						}
						ImGui::EndDragDropTarget();
					}

					if (ImGui::BeginDragDropSource())
					{
						DragDrop::SetPathPayload(DragDropTag::CONTENT_BROWSER_ITEM, entry.path());
						ImGui::Text("Moving folder: %s", name.c_str());
						ImGui::EndDragDropSource();
					}

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

	void Explorer::ShowFolderContent()
	{
		std::filesystem::path content = std::filesystem::current_path();
		bool found = false;

		for (int i = 0; i < 5; ++i)
		{
			if (std::filesystem::exists(content / "MirageEngine" / "Assets"))
			{
				m_RootPath = std::filesystem::absolute(content / "MirageEngine" / "Assets");
				found = true;
				break;
			}

			if (content.filename() == "MirageEngine" && std::filesystem::exists(content / "Assets"))
			{
				m_RootPath = std::filesystem::absolute(content / "Assets");
				found = true;
				break;
			}
			if (content.has_parent_path())
			{
				content = content.parent_path();
			}
			else
			{
				break;
			}
		}
		if (!found)
		{
			content = std::filesystem::current_path();
			for (int i = 0; i < 3; ++i)
			{
				if (std::filesystem::exists(content / "Assets"))
				{
					m_RootPath = std::filesystem::absolute(content / "Assets");
					found = true;
					break;
				}
				content = content.parent_path();
			}
		}
	}

	void Explorer::DrawDirectoryContent()
	{
		m_AnItemWasRightClicked = false;

		ImGui::BeginGroup();
		DrawBreadcrumb();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::EndGroup();

		if (ImGui::BeginChild("AssetGridArea", ImVec2(0, 0), false))
		{
			RenderAssetGrid();
			if (ImGui::IsWindowHovered() && !m_AnItemWasRightClicked && ImGui::IsMouseClicked(1))
				m_NextModalToOpen = "EmptySpaceMenu";
		}
		ImGui::EndChild();

		if (!m_NextModalToOpen.empty())
		{
			ImGui::OpenPopup(m_NextModalToOpen.c_str());
			m_NextModalToOpen = "";
		}

		RenderAssetContextMenu();
		RenderEmptySpaceContextMenu();
		RenderRenameModal();
		RenderDeleteModal();
	}

	void Explorer::DrawBreadcrumb()
	{
		if (UIWrapper::DrawBreadcrumbSegment("Assets", m_CurrentPath == m_RootPath))
		{
			this->ChangeDirectory(m_RootPath);
		}

		std::filesystem::path relativePath = std::filesystem::relative(m_CurrentPath, m_RootPath);
		std::filesystem::path currentStep = m_RootPath;

		for (const auto& segment : relativePath)
		{
			if (segment == ".") continue;

			currentStep /= segment;
			bool bisLast = (currentStep == m_CurrentPath);
			if (UIWrapper::DrawBreadcrumbSegment(segment.string().c_str(), bisLast))
			{
				this->ChangeDirectory(currentStep);
				break;
			}
		}
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
		std::filesystem::path directoryToChange = "";
		bool needsRefresh = false;

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

				if (action == WidgetAction::DoubleClicked)
				{
					if(entry.Type == AssetType::Folder)
					{
						directoryToChange = entry.Path;
					}
					else
					{
						EditorActions::OpenExternalEditor(entry.Path);
					}
				}

				if (action == WidgetAction::RightClicked)
				{
					m_EntryToProcess = const_cast<AssetEntry*>(&entry);
					m_NextModalToOpen = "AssetContextMenu";
					m_AnItemWasRightClicked = true;
				}

				if (ImGui::BeginDragDropSource())
				{
					DragDrop::SetPathPayload(DragDropTag::CONTENT_BROWSER_ITEM, entry.Path);
					ImGui::Text("Moving: %s", entry.Name.c_str());
					ImGui::EndDragDropSource();
				}

				if (entry.Type == AssetType::Folder && ImGui::BeginDragDropTarget()) {
					auto droppedPath = DragDrop::GetPathPayload(DragDropTag::CONTENT_BROWSER_ITEM);
					if (!droppedPath.empty()) {
						this->MoveItem(droppedPath, entry.Path);
						needsRefresh = true;
					}
					ImGui::EndDragDropTarget();
				}

				UIWrapper::EndAssetTile();

				if (!directoryToChange.empty() || needsRefresh)
					break;
			}
			UIWrapper::EndAssetGrid();
		}
		if (!directoryToChange.empty())
		{
			this->ChangeDirectory(directoryToChange);
		}
		else if (needsRefresh)
		{
			ScanProjectDirectory(m_CurrentPath);
		}
		ImGui::Dummy(ImGui::GetContentRegionAvail());
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
					m_NextModalToOpen = "RenameModal";
				}

				if (ImGui::MenuItem("Delete"))
				{
					m_NextModalToOpen = "DeleteModal";
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
				if (ImGui::MenuItem("New Text"))
				{
					std::filesystem::path newFilePath = m_CurrentPath / "NewFile.json";
					int counter = 1;
					while (std::filesystem::exists(newFilePath))
					{
						newFilePath = m_CurrentPath / ("NewFile (" + std::to_string(counter) + ").json");
						counter++;
					}
					std::ofstream outfile(newFilePath);
					outfile.close();
					ScanProjectDirectory(m_CurrentPath);
				}
				ImGui::Separator();
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

	void Explorer::RenderDeleteModal()
	{
		std::error_code ec;
		if (ImGui::BeginPopupModal("DeleteModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete?");

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				if (m_EntryToProcess)
				{
					std::filesystem::remove_all(m_EntryToProcess->Path, ec);
					ScanProjectDirectory(m_CurrentPath);
					m_EntryToProcess = nullptr;
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

	void Explorer::RenameItem(const std::filesystem::path& _oldPath, const std::filesystem::path& _newPath)
	{
		std::error_code ec;
		std::filesystem::rename(_oldPath, _newPath, ec);

		if (ec)
		{
			std::cout << "Error during renaming" << ec.message() << std::endl;
		}
	}

	void Explorer::MoveItem(const std::filesystem::path& _source, const std::filesystem::path& _target)
	{
		std::filesystem::path newPath = _target / _source.filename();
		if (newPath == _source) return;
		std::error_code ec;
		std::filesystem::rename(_source, newPath, ec);

		if (ec) 
		{
			std::cerr << "Move Error: " << ec.message() << std::endl;
		}
		else
		{
			ScanProjectDirectory(m_CurrentPath);
		}
	}
}