#pragma once
#include "IPanel.h"
#include <iostream>
#include <vector>
#include <memory>

class GameObject;

namespace Mirage
{
	class EditorApp
	{
	private:
		GameObject* SelectedObject = nullptr;
		std::vector<std::unique_ptr<IPanel>> PanelList;
		void* WindowHandle = nullptr;

	public:
		void Init(void* windowHandle);
		void Update();
		void Shutdown();

		void SetSelectedObject(GameObject* object) { SelectedObject = object; };
		GameObject* GetSelectedObject() const { return SelectedObject; };
	};
}