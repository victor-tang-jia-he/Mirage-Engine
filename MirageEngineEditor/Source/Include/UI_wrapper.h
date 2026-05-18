#pragma once
#include <string>
#include "vector3.h"
#include <imgui.h>

#include <explorer.h>

#include <Resources/resource_manager.h>

DECLARE_LOG_CATEGORY_EXTERN(LogUIWrapper)

namespace Math = MirageEngine::Core::Math;

namespace Mirage
{
	enum class WidgetAction
	{
		NONE = 0,
		CLICKED,
		DOUBLE_CLICKED,
		RIGHT_CLICKED,
		LEFT_CLICKED
	};

	class UIWrapper
	{
	public:
		static void Init(void* _windowHandle);
		static void Shutdown();

		static void BeginFrame();
		static void EndFrame();

		// window
		static bool BeginWindow(const char* _windowName, bool* _isOpen = nullptr);
		static void EndWindow();

		static bool BeginFolderNode(const char* _name, const char* _id, bool _isSelected, bool _isLeaf);
		static void EndFolderNode();
		static bool IsItemClicked();

		static bool BeginAssetGrid(float _cellSize, float _windowSize);
		static WidgetAction DrawAssetTile(const char* _name, bool _isSelected, std::shared_ptr<MirageEngine::Core::Resources::Texture> _icon);
		static void EndAssetTile();
		static void EndAssetGrid();

		static bool DrawBreadcrumbSegment(const char* _label, bool _bIsLast);

		static bool AssetContextMenu(const char* _id);
		static bool DrawContextMenuItem(const char* _label);

		// Widgets
		static void CreateSeparatorText(const char* _separatorName);
		static void CreateColorPicker(Math::Vector3* _color);

	private:
		// Design
		static void ApplyThemeColors();
		static ImVec4 Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a = 255);
	};
}