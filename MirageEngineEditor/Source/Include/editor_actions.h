#pragma once
#include <filesystem>

namespace Mirage
{
	class EditorActions
	{
	public:
		static void OpenExternalEditor(const std::filesystem::path& _path);
	};
}