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
	public:
		Explorer(const char* _name) : IPanel(_name) {};
		
		void InitPanel() override;
		void DrawPanel() override;
		void ShutdownPanel() override;

	private:
		//Navigation
		void ScanProjectDirectory();
		void DrawFolderTree();
		void DrawBreadCrumbs(); //Shows the path of the seleccted object with every segment clickable
		void ChangeDirectory();
		void SearchFilter();

		//Display
		void DrawDirectoryContent();
		void DrawFileIcon();

		//Interaction
		void HandleFileClick();
		void BeginFileDrag();
		void ShowContextMenu();
		void SyncWithOS(); //Watches if you add a file through Windows

		void RenameItem();
		void MoveItem();
	};
}