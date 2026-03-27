#pragma once

namespace Mirage
{
	class IPanel
	{
	protected:
		const char* p_Title;
		bool p_IsActive;

	public:
		virtual ~IPanel() = default;
		IPanel(const char* _name) : p_Title(_name), p_IsActive(true) {};

		virtual void InitPanel() {};
		virtual void DrawPanel() = 0;
		virtual void ShutdownPanel() {};

		const char* GetTitle() const { return p_Title;  }
		bool IsPanelActive() const { return p_IsActive; }
		void SetActive(bool _state) { p_IsActive = _state; }
	};
}