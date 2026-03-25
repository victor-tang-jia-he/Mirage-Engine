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
					else if (ext == ".cs" || ext == ".cpp" || ext == ".h" || ext == ".py") newEntry.Type = AssetType::Script;
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
		m_CurrentPath = _newPath;
		ScanProjectDirectory(m_CurrentPath);
	}

	void Explorer::DrawDirectoryContent()
	{

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