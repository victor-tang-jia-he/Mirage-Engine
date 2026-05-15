#pragma once
#include "i_panel.h"
#include <string>
#include <filesystem>

#include <Resources/texture.h>
#include <vector2.h>

#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(LogExplorer)
DECLARE_LOG_CATEGORY_EXTERN(LogExplorerInfo)

namespace Mirage
{
	enum class AssetType {
		Folder	= 0,
		Texture = 1,
		Model	= 2,
		Script	= 3,
		Shader	= 4,
		Scene	= 5,
		Unknown = 6
	};

	struct AssetEntry
	{
		std::string Name;
		std::filesystem::path Path;
		AssetType Type;
		void* Icon;
	};

	class Explorer : public IPanel
	{
	private:
		std::filesystem::path m_CurrentPath;
		std::filesystem::path m_RootPath = "./Assets";
		std::vector<AssetEntry> m_Entries;
		void* m_SelectedEntry;
		AssetEntry* m_EntryToProcess = nullptr;
		char m_RenameBuffer[256] = "";
		bool m_ShouldOpenAssetPopup = false;
		bool m_AnItemWasRightClicked = false;

		std::string m_NextModalToOpen;

	public:
		Explorer(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel(uint32_t _textureID) override;
		void ShutdownPanel() override;

	private:
		//Navigation
		void ScanProjectDirectory(const std::filesystem::path _rootPath);
		void DrawFolderTree();
		void RenderRecursiveFolder(const std::filesystem::path& _path);
		void ChangeDirectory(const std::filesystem::path& _newPath);

		//Display
		void DrawDirectoryContent();
		void DrawBreadcrumb();

		//Interaction
		void HandleFileClick();

		void DuplicateItem(const std::filesystem::path& _source, const std::filesystem::path& _target);
		std::filesystem::path GenerateUniquePath(const std::filesystem::path& _basePath);

		void CreateItem(const std::filesystem::path& _path, std::string _name, std::string _extension, std::string _content);

		void RenderAssetGrid();
		void RenderAssetContextMenu();
		void RenderEmptySpaceContextMenu();
		void RenderRenameModal();
		void RenderDeleteModal();
		void RenameItem(const std::filesystem::path& _oldPath, const std::filesystem::path& _newPath);
		void MoveItem(const std::filesystem::path& _source, const std::filesystem::path& _target);

		// Texture / Icon
		std::shared_ptr<MirageEngine::Core::Resources::Texture> GetIconTexture(unsigned int _index);
	};
}