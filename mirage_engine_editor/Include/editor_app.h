#pragma once
#include "i_panel.h"
#include <iostream>
#include <vector>
#include <memory>

class GameObject;

namespace Mirage
{
	class EditorApp
	{
	private:
		GameObject* m_SelectedObject = nullptr;
		std::vector<std::unique_ptr<IPanel>> m_PanelList;
		void* m_WindowHandle = nullptr;

	public:
		void Init(void* _windowHandle);
		void Update();
		void Shutdown();

		void SetSelectedObject(GameObject* _object) { m_SelectedObject = _object; };
		GameObject* GetSelectedObject() const { return m_SelectedObject; };
	};
}