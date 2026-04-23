#pragma once
#include <filesystem>

namespace Mirage
{
	class EditorActions
	{
	public:
		static void OpenExternalEditor(const std::filesystem::path& _path);
		static void ShowInExplorer(const std::filesystem::path& _path);
	};
}