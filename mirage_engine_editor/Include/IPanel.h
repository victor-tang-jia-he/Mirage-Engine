#pragma once

namespace Mirage
{
	class IPanel
	{
	public:
		virtual ~IPanel() = default;
		IPanel(const char* name) : Title(name), IsActive(true) {};

		virtual void InitPanel() {};
		virtual void DrawPanel() = 0;
		virtual void ShutdownPanel() {};

		const char* GetTitle() const { return Title;  }
		bool IsPanelActive() const { return IsActive; }
		void SetActive(bool state) { IsActive = state; }

	protected:
		const char* Title;
		bool IsActive;
	};
}