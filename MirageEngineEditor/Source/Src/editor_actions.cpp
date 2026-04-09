#include "editor_actions.h"
#define WIN32_LEAN_AND_MEAN 
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#pragma comment(lib, "shell32.lib")

namespace Mirage
{
	void EditorActions::OpenExternalEditor(const std::filesystem::path& _file)
	{
		std::wstring pathStr = _file.wstring();
		ShellExecuteW(NULL, L"open", pathStr.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
}