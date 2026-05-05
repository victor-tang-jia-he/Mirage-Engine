#include "explorer.h"
#include "UI_wrapper.h"
#include "editor_actions.h"
#include "console.h"
#include "dragdrop.h"

#include <Resources/resource_manager.h>
#include <Resources/texture.h>

#include <fstream>
#include <iostream>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogExplorer, Loggers::Color::CYAN);
DEFINE_LOG_CATEGORY(LogExplorerInfo);

namespace Mirage
{
	void Explorer::InitPanel()
	{
		ShowFolderContent();
		m_CurrentPath = m_RootPath;
		ScanProjectDirectory(m_CurrentPath);

		LOG(logTemp, LogExplorer, "Root Path detected: ", m_RootPath.string());
		LOG(logTemp, LogExplorer, "Explorer Panel Initialized");
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
		LOG(logTemp, LogExplorer, "Explorer Panel Shutdown");
	}

	void Explorer::ScanProjectDirectory(const std::filesystem::path _rootPath)
	{
		LOG(logTemp, LogExplorer, "Scanning directory: ", _rootPath.string());
		m_Entries.clear();
		if (std::filesystem::exists(_rootPath) && std::filesystem::is_directory(_rootPath))
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
					if (ext == ".png" || ext == ".jpg")
						newEntry.Type = AssetType::Texture;
					else if (ext == ".obj")
						newEntry.Type = AssetType::Model;	
					else if (ext == ".c" || ext == ".cpp" || ext == ".cs" || ext == ".py")
						newEntry.Type = AssetType::Script;
					else if (ext == ".glsl" || ext == ".frag" || ext == ".vert")
						newEntry.Type = AssetType::Shader;
					else
						newEntry.Type = AssetType::Unknown;
				}
				m_Entries.push_back(newEntry);
			}
		}
		LOG(logTemp, LogExplorer, "Found ", m_Entries.size(), " entries in directory.");
	}

	void Explorer::DrawFolderTree()
	{
		std::string name = "Assets";
		std::string id = m_RootPath.string();

		bool isLeaf = true;
		for (const auto& sub : std::filesystem::directory_iterator(m_RootPath))
		{
			if (sub.is_directory())
			{
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
			if (UIWrapper::IsItemClicked())
				this->ChangeDirectory(m_RootPath);
		}
	}

	void Explorer::RenderRecursiveFolder(const std::filesystem::path& _path)
	{
		std::error_code ec;
		auto it = std::filesystem::directory_iterator(_path, ec);

		if (ec)
			return;

		for (const auto& entry : it)
		{
			if (entry.is_directory())
			{
				std::string name = entry.path().filename().string();
				std::string id = entry.path().string();

				bool bisLeaf = true;
				for (const auto& sub : std::filesystem::directory_iterator(entry.path()))
				{
					if (sub.is_directory())
					{
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
						// Display a mini message when drag an object
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
					if (UIWrapper::IsItemClicked())
					{
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

	void Explorer::DuplicateItem(const std::filesystem::path& _source, const std::filesystem::path& _target)
	{
		std::error_code ec;

		std::filesystem::copy(_source, _target, std::filesystem::copy_options::recursive, ec);

		if (ec)
			LOG(logError, LogExplorerInfo, "Error during duplication: ", ec.message());

		ScanProjectDirectory(m_CurrentPath);
	}

	std::filesystem::path Explorer::GenerateUniquePath(const std::filesystem::path& _basePath)
	{
		std::filesystem::path folder = _basePath.parent_path();
		std::string name = _basePath.stem().string();
		std::string extension = _basePath.extension().string();

		std::filesystem::path newPath = folder / (name + "_Copy" + extension);

		int counter = 1;
		while (std::filesystem::exists(newPath))
		{
			newPath = folder / (name + "_Copy (" + std::to_string(counter) + ")" + extension);
			counter++;
		}
		return newPath;
	}

	void Explorer::CreateItem(const std::filesystem::path& _path, std::string _name, std::string _extension, std::string _content)
	{
		std::filesystem::path newFilePath = _path / (_name + _extension);
		int counter = 1;
		while (std::filesystem::exists(newFilePath))
		{
			newFilePath = _path / (_name + " (" + std::to_string(counter) + ")" + _extension);
			counter++;
		}
		std::ofstream outfile(newFilePath);
		if (outfile.is_open())
		{
			outfile << _content;
			outfile.close();
		}
		outfile.close();
		ScanProjectDirectory(m_CurrentPath);
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

				unsigned int id = (int)AssetType::Unknown;
				if (entry.Type == AssetType::Folder)
				{
					id = (int)AssetType::Folder;
				}
				else if (entry.Type == AssetType::Texture)
				{
					id = (int)AssetType::Texture;
				}
				else if (entry.Type == AssetType::Model)
				{
					id = (int)AssetType::Model;
				}
				else if (entry.Type == AssetType::Script)
				{
					id = (int)AssetType::Script;
				}
				else if (entry.Type == AssetType::Shader)
				{
					id = (int)AssetType::Shader;
				}
				else if (entry.Type == AssetType::Scene)
				{
					id = (int)AssetType::Scene;
				}
				else
				{
					id = (int)AssetType::Unknown;
				}

				// Determine correct icon index based on asset type
				WidgetAction content = UIWrapper::DrawAssetTile(entry.Name.c_str(), isSelected, GetIconTexture(id));

				if (content == WidgetAction::Clicked)
				{
					m_SelectedEntry = (void*)&entry;
				}

				if (content == WidgetAction::DoubleClicked)
				{
					if (entry.Type == AssetType::Folder)
					{
						directoryToChange = entry.Path;
					}
					else
					{
						EditorActions::OpenExternalEditor(entry.Path);
					}
				}

				if (content == WidgetAction::RightClicked)
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

				if (ImGui::MenuItem("Show in Explorer"))
				{
					EditorActions::ShowInExplorer(m_EntryToProcess->Path);
					ImGui::CloseCurrentPopup();
				}
				if (ImGui::MenuItem("Duplicate"))
				{
					if (m_EntryToProcess)
					{
						std::filesystem::path sourcePath = m_EntryToProcess->Path;
						std::filesystem::path uniquePath = GenerateUniquePath(sourcePath);
						DuplicateItem(m_EntryToProcess->Path, uniquePath);
						m_EntryToProcess = nullptr;
						ImGui::CloseCurrentPopup();
					}
				}
				if (ImGui::MenuItem("Delete"))
				{
					m_NextModalToOpen = "DeleteModal";
				}
				if (ImGui::MenuItem("Rename"))
				{
					//strncpy_s(m_RenameBuffer, m_EntryToProcess->Name.c_str(), sizeof(m_RenameBuffer));
					m_NextModalToOpen = "RenameModal";
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
				if (ImGui::BeginMenu("Create"))
				{
					if (ImGui::MenuItem("New Folder"))
					{
						std::error_code ec;
						std::filesystem::path newFolderPath = m_CurrentPath / "NewFolder";
						int counter = 1;
						while (std::filesystem::exists(newFolderPath))
						{
							newFolderPath = m_CurrentPath / ("NewFolder (" + std::to_string(counter) + ")");
							counter++;
						}
						std::filesystem::create_directory(newFolderPath, ec);
						ScanProjectDirectory(m_CurrentPath);
					}

					ImGui::Separator();

					if (ImGui::MenuItem("Scripts")) {}

					ImGui::Separator();

					if (ImGui::BeginMenu("Shaders")) {
						if (ImGui::MenuItem("Simple GLSL"))
						{
							CreateItem(m_CurrentPath, "NewShader", ".glsl", "// Shader Code\n");
						}
						if (ImGui::MenuItem("Vertex Shader (.vert)")) {
							std::string tmp = "#version 330 core\nlayout (location = 0) in vec3 aPos;\nvoid main() {\n    gl_Position = vec4(aPos, 1.0);\n}";
							CreateItem(m_CurrentPath, "NewVertex", ".vert", tmp);
						}
						if (ImGui::MenuItem("Fragment Shader (.frag)")) {
							std::string tmp = "#version 330 core\nout vec4 FragColor;\nvoid main() {\n    FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n}";
							CreateItem(m_CurrentPath, "NewFragment", ".frag", tmp);
						}
						ImGui::EndMenu();
					}

					ImGui::Separator();

					if (ImGui::BeginMenu("Document"))
					{
						if (ImGui::MenuItem("Text File (.txt)"))
						{
							CreateItem(m_CurrentPath, "NewText", ".txt", "");
						}
						if (ImGui::MenuItem("JSON File (.json)"))
						{
							CreateItem(m_CurrentPath, "NewJSON", ".json", "{\n\t\"version\": 1\n}");
						}
						if (ImGui::MenuItem("Readme (.md)"))
						{
							CreateItem(m_CurrentPath, "README", ".md", "# Project Notes\n");
						}
						ImGui::EndMenu();
					}
					ImGui::EndMenu();
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
			LOG(logError, LogExplorerInfo, "Error during renaming: ", ec.message());
	}

	void Explorer::MoveItem(const std::filesystem::path& _source, const std::filesystem::path& _target)
	{
		std::filesystem::path newPath = _target / _source.filename();
		if (newPath == _source) return;
		std::error_code ec;
		std::filesystem::rename(_source, newPath, ec);

		if (ec)
			LOG(logError, LogExplorerInfo, "Move Error: ", ec.message());
		else
			ScanProjectDirectory(m_CurrentPath);
	}

	std::shared_ptr<MirageEngine::Core::Resources::Texture> Explorer::GetIconTexture(unsigned int _index)
	{
		MirageEngine::Core::Resources::ResourceManager& rm = MirageEngine::Core::Resources::ResourceManager::Instance();

		std::shared_ptr<MirageEngine::Core::Resources::Texture> folder = rm.GetTexture("folder_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> texture = rm.GetTexture("texture_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> model = rm.GetTexture("model_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> script = rm.GetTexture("script_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> shader = rm.GetTexture("shader_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> scene = rm.GetTexture("scene_icon");
		std::shared_ptr<MirageEngine::Core::Resources::Texture> file = rm.GetTexture("file_icon");

		folder->Bind();
		texture->Bind();
		model->Bind();
		script->Bind();
		shader->Bind();
		scene->Bind();
		file->Bind();
		switch (_index)
		{
		case 0: return folder;   // Folder
		case 1: return texture;  // Texture
		case 2: return model;    // Model
		case 3: return script;   // Script
		case 4: return shader;   // Shader
		case 5: return scene;    // Scene
		case 6: return file;     // Unknown/Generic
		default: return nullptr;
		}
	}
}