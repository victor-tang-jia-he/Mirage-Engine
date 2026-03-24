#include "explorer.h"
#include "UI_wrapper.h"
#include "console.h"
#include <fstream>
#include <iostream>

namespace Mirage
{
	void Explorer::InitPanel()
	{

	}

	void Explorer::DrawPanel()
	{
		UIWrapper::BeginWindow("Explorer");
		UIWrapper::EndWindow();
	}

	void Explorer::ShutdownPanel()
	{

	}

	void Explorer::ScanProjectDirectory(std::filesystem::path)
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