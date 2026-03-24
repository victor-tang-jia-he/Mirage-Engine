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
		UIWrapper::EndWindow();
	}

	void Explorer::ShutdownPanel()
	{

	}

	void Explorer::ScanProjectDirectory(std::filesystem::path _rootPath)
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