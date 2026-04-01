#pragma once
#include "i_panel.h"
#include <string>
#include <filesystem>

namespace Mirage
{
	enum class AssetType {
		Folder,
		Texture,
		Model,
		Script,
		Shader,
		Scene,
		Unknown
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
		std::filesystem::path m_RootPath;
		std::vector<AssetEntry> m_Entries;
		void* m_SelectedEntry;
		AssetEntry* m_EntryToProcess = nullptr;
		char m_RenameBuffer[256] = "";
		bool m_ShouldOpenAssetPopup = false;
		bool m_AnItemWasRightClicked = false;

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
		void DrawBreadCrumbs(); //Shows the path of the selected object with every segment clickable
		void ChangeDirectory(const std::filesystem::path& _newPath);
		void SearchFilter();

		//Display
		void DrawDirectoryContent();
		void DrawBreadcrumb();
		void DrawFileIcon();

		//Interaction
		void HandleFileClick();
		void BeginFileDrag();
		void SyncWithOS(); //Watches if you add a file through Windows

		void RenderAssetGrid();
		void RenderAssetContextMenu();
		void RenderEmptySpaceContextMenu();
		void RenderRenameModal();
		void RenderDeleteModal();
		void RenameItem(std::filesystem::path& _oldPath, std::filesystem::path& _newPath);
		void MoveItem(const std::filesystem::path& _source, const std::filesystem::path& _target);
	};
}