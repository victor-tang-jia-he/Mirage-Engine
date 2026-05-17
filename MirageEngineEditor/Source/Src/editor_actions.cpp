#include "editor_actions.h"

#include <filesystem>

#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <shellapi.h>
#elif defined(__linux__)
#include <cstdlib>
#endif

namespace Mirage
{
    void EditorActions::OpenExternalEditor(const std::filesystem::path& _file)
    {
#if defined(_WIN32)

        std::wstring pathStr = _file.wstring();
        ShellExecuteW(NULL, L"open", pathStr.c_str(), NULL, NULL, SW_SHOWNORMAL);

#elif defined(__linux__)

        std::string cmd = "xdg-open \"" + _file.string() + "\"";
        std::system(cmd.c_str());

#endif
    }

    void EditorActions::ShowInExplorer(const std::filesystem::path& _file)
    {
#if defined(_WIN32)

        std::wstring pathStr = _file.wstring();
        std::wstring args = L"/select,\"" + pathStr + L"\"";

        ShellExecuteW(NULL, L"open", L"explorer.exe", args.c_str(), NULL, SW_SHOWNORMAL);

#elif defined(__linux__)

        std::string cmd = "xdg-open \"" + _file.parent_path().string() + "\"";
        std::system(cmd.c_str());

#endif
    }
}