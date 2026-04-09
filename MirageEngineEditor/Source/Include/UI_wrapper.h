#pragma once
#include <string>
#include "vector3.h"

namespace Math = MirageEngine::Core::Math;

namespace Mirage
{
	enum class WidgetAction
	{
		None = 0,
		Clicked,
		DoubleClicked,
		RightClicked
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

		static void RequestModal(const char* _modalName);
		static void UpdateModalRequests();

		static bool BeginAssetGrid(float _cellSize, float _windowSize);
		static WidgetAction DrawAssetTile(const char* _name, bool _isSelected, void* _icon = nullptr);
		static void EndAssetTile();
		static void EndAssetGrid();

		static bool DrawBreadcrumbSegment(const char* _label, bool _bIsLast);

		static bool AssetContextMenu(const char* _id);
		static bool DrawContextMenuItem(const char* _label);


		// Widgets
		static void CreateSeparatorText(const char* _separatorName);
		static void CreateColorPicker(Math::Vector3* _color);
	};
}