#include "UI_wrapper.h"

#include "vector3.h"
#include <string>

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <Resources/texture.h>

#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogUIWrapper, Loggers::Color::CYAN);

namespace MirageEngine::Core::Resources
{
	class Texture;
}

namespace Math = MirageEngine::Core::Math;

namespace Mirage
{
	void UIWrapper::Init(void* _windowHandle)
	{
		IMGUI_CHECKVERSION();
		ImGuiContext* context = ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO();
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		ImGui::StyleColorsDark();
		ApplyThemeColors();

		// Load custom font
		ImFont* font = io.Fonts->AddFontFromFileTTF("./Assets/TextFonts/ZenDots-Regular.ttf", 16);
		if (font == nullptr)
		{
			LOG(logWarning, LogUIWrapper, "Failed to load custom font, using default");
		}
		else
		{
			io.FontDefault = font;
			LOG(logTemp, LogUIWrapper, "Custom font loaded successfully");
		}

		ImGuizmo::SetImGuiContext(context);

		GLFWwindow* window = static_cast<GLFWwindow*>(_windowHandle);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		LOG(logTemp, LogUIWrapper, "UI Wrapper Init");
	}

	void UIWrapper::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		LOG(logTemp, LogUIWrapper, "UI Wrapper Shutdown");
	}

	void UIWrapper::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		//ImGui::ShowDemoWindow();
		ShowThemeColorEditor();
	}

	void UIWrapper::EndFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		//ImGuiIO& io = ImGui::GetIO();
		//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		//{
		//	GLFWwindow* backup_current_context = glfwGetCurrentContext();

		//	ImGui::UpdatePlatformWindows();
		//	ImGui::RenderPlatformWindowsDefault();

		//	glfwMakeContextCurrent(backup_current_context);
		//}
	}

	bool UIWrapper::BeginWindow(const char* _windowName, bool* _isOpen)
	{
		return ImGui::Begin(_windowName, _isOpen);
	}

	void UIWrapper::EndWindow()
	{
		ImGui::End();
	}

	bool UIWrapper::BeginFolderNode(const char* _name, const char* _id, bool _isSelected, bool _isLeaf)
	{
		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAvailWidth | 
											 ImGuiTreeNodeFlags_OpenOnArrow |
											 ImGuiTreeNodeFlags_OpenOnDoubleClick | 
											 ImGuiTreeNodeFlags_NavLeftJumpsBackHere;
		if (_isSelected)
			tree_node_flags |= ImGuiTreeNodeFlags_Selected;
		if (_isLeaf)
			tree_node_flags |= ImGuiTreeNodeFlags_Leaf;

		std::string id = std::string(_name) + "##" + _id;

		bool isOpen = ImGui::TreeNodeEx(id.c_str(), tree_node_flags);
		return isOpen;
	}

	void UIWrapper::EndFolderNode()
	{
		ImGui::TreePop();
	}

	bool UIWrapper::IsItemClicked()
	{
		return ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen();
	}

	void UIWrapper::CreateSeparatorText(const char* _separatorName)
	{
		ImGui::SeparatorText(_separatorName);
	}

	void UIWrapper::CreateColorPicker(Math::Vector3* _color)
	{
		float tempColor[3] = { _color->X / 255.0f, _color->Y / 255.0f, _color->Z / 255.0f };
		ImVec4 refVecColor(1.0f, 0.0f, 1.0f, 1.0f);
		ImGuiColorEditFlags color_picker_flags = ImGuiColorEditFlags_NoAlpha;
		ImGuiColorEditFlags base_flags = ImGuiColorEditFlags_None;

		ImGuiColorEditFlags flags = base_flags | color_picker_flags;
		if (ImGui::ColorPicker3("MyColor##4", tempColor, flags))
		{
			_color->X = tempColor[0] * 255.0f;
			_color->Y = tempColor[1] * 255.0f;
			_color->Z = tempColor[2] * 255.0f;
		}
	}

	bool UIWrapper::BeginAssetGrid(float _cellSize, float _windowSize)
	{
		int colomnNbr = static_cast<int>(_windowSize / _cellSize);
		if (colomnNbr < 1) colomnNbr = 1;
		return ImGui::BeginTable("Content", colomnNbr);
	}

	WidgetAction UIWrapper::DrawAssetTile(const char* _name, bool _isSelected, std::shared_ptr<MirageEngine::Core::Resources::Texture> _icon)
	{
		MirageEngine::Core::Resources::ResourceManager rm;

		ImGui::TableNextColumn();
		ImGui::PushID(_name);
		
		WidgetAction action = WidgetAction::NONE;

		// Get the starting position of the tile
		ImVec2 startPos = ImGui::GetCursorPos();

		// Calculate the size of the tile based on the available width and a fixed height
		float iconSize = 64;
		float columnWidth = ImGui::GetContentRegionAvail().x;
		ImVec2 tileSize = ImVec2(columnWidth, iconSize);

		ImGui::BeginGroup();

		// Calculate the offset to center the icon with the tile
		float iconOffset = (columnWidth - iconSize) * 0.5f;
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + iconOffset);

		if (_icon)
		{
			// Standard UV coordinates (0, 0) bottom-left to (1, 1) top-right for OpenGL
			ImGui::Image((ImTextureID)(intptr_t)_icon->GetTextureID(), ImVec2(iconSize, iconSize), ImVec2(0, 1), ImVec2(1, 0));
		}
		else
		{
			// Display placeholder for missing texture
			ImGui::Dummy(ImVec2(iconSize, iconSize));
			ImGui::GetWindowDrawList()->AddRect(
				ImGui::GetItemRectMin(),
				ImGui::GetItemRectMax(),
				ImGui::GetColorU32(ImGuiCol_TextDisabled),
				2.0f
			);
		}

		float textWidth = ImGui::CalcTextSize(_name).x;
		if (textWidth < columnWidth)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (columnWidth - textWidth) * 0.5f);

		ImGui::TextWrapped(_name);
		ImGui::EndGroup();

		ImGui::SetCursorPos(startPos);
		if (ImGui::Selectable("##selectable", _isSelected, ImGuiSelectableFlags_AllowDoubleClick, tileSize))
		{
			// Clic
		}

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseDoubleClicked(0)) action = WidgetAction::DOUBLE_CLICKED;
			else if (ImGui::IsItemClicked(0)) action = WidgetAction::CLICKED;
			else if (ImGui::IsItemClicked(1)) action = WidgetAction::RIGHT_CLICKED;
		}

		ImGui::SetCursorPos(ImVec2(startPos.x, startPos.y + tileSize.y));

		return action;
	}

	void UIWrapper::EndAssetTile()
	{
		ImGui::PopID();
	}

	void UIWrapper::EndAssetGrid()
	{
		ImGui::EndTable();
	}

	bool UIWrapper::DrawBreadcrumbSegment(const char* _label, bool _bIsLast)
	{
		bool clicked = ImGui::SmallButton(_label);
		ImGui::SameLine();
		if (!_bIsLast)
		{
			ImGui::Text(" > ");
			ImGui::SameLine();
		}
		return clicked;
	}

	void UIWrapper::ApplyThemeColors()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		// Primary colors
		style.Colors[ImGuiCol_WindowBg]						= Color(33, 21, 64, 255);
		style.Colors[ImGuiCol_ChildBg]						= Color(21, 13, 40, 255);
		style.Colors[ImGuiCol_PopupBg]						= Color(20, 20, 20, 255);
		// style.Colors[ImGuiCol_Border];
		// style.Colors[ImGuiCol_BorderShadow];
		style.Colors[ImGuiCol_FrameBg]						= Color(29, 0, 76, 255);
		// style.Colors[ImGuiCol_MenuBarBg];
		style.Colors[ImGuiCol_ScrollbarBg];
		style.Colors[ImGuiCol_TitleBg]						= Color(43, 27, 39, 255);
		style.Colors[ImGuiCol_TitleBgActive];
		style.Colors[ImGuiCol_TitleBgCollapsed];

		// Interactive elements - hover/active states
		style.Colors[ImGuiCol_FrameBgHovered]				= Color(250, 150, 250, 102);
		style.Colors[ImGuiCol_FrameBgActive];
		style.Colors[ImGuiCol_ScrollbarGrab]				= Color(81, 79, 96, 255);
		style.Colors[ImGuiCol_ScrollbarGrabHovered]			= Color(81, 79, 96, 255);
		style.Colors[ImGuiCol_ScrollbarGrabActive];
		style.Colors[ImGuiCol_SliderGrab];
		style.Colors[ImGuiCol_SliderGrabActive];

		// Buttons
		style.Colors[ImGuiCol_Button];
		style.Colors[ImGuiCol_ButtonHovered];
		style.Colors[ImGuiCol_ButtonActive];

		// Headers and separators
		style.Colors[ImGuiCol_Header];
		style.Colors[ImGuiCol_HeaderHovered];
		style.Colors[ImGuiCol_HeaderActive];
		style.Colors[ImGuiCol_Separator];
		style.Colors[ImGuiCol_SeparatorHovered];
		style.Colors[ImGuiCol_SeparatorActive];

		// Tabs
		style.Colors[ImGuiCol_Tab];
		// style.Colors[ImGuiCol_TabHovered]					= Color(255, 128, 16, 185);
		// style.Colors[ImGuiCol_TabSelected]					= Color(228, 35, 31, 211);
		style.Colors[ImGuiCol_TabUnfocusedActive];
		style.Colors[ImGuiCol_TabSelectedOverline];
		style.Colors[ImGuiCol_TabDimmed];
		style.Colors[ImGuiCol_TabDimmedSelected]			= Color(47, 11, 146, 180);
		style.Colors[ImGuiCol_TabDimmedSelectedOverline];

		// Resize grip
		style.Colors[ImGuiCol_ResizeGrip];
		style.Colors[ImGuiCol_ResizeGripHovered];
		style.Colors[ImGuiCol_ResizeGripActive];

		// Plots and tables
		style.Colors[ImGuiCol_PlotLines];
		style.Colors[ImGuiCol_PlotLinesHovered];
		style.Colors[ImGuiCol_PlotHistogram];
		style.Colors[ImGuiCol_PlotHistogramHovered];
		style.Colors[ImGuiCol_TableHeaderBg];
		style.Colors[ImGuiCol_TableBorderStrong];
		style.Colors[ImGuiCol_TableBorderLight];
		style.Colors[ImGuiCol_TableRowBg];
		style.Colors[ImGuiCol_TableRowBgAlt];

		// Docking
		style.Colors[ImGuiCol_DockingPreview];
		style.Colors[ImGuiCol_DockingEmptyBg];

		// Navigation and modals
		style.Colors[ImGuiCol_CheckMark];
		style.Colors[ImGuiCol_DragDropTarget];
		style.Colors[ImGuiCol_DragDropTargetBg];
		style.Colors[ImGuiCol_NavHighlight];
		style.Colors[ImGuiCol_NavCursor];
		style.Colors[ImGuiCol_NavWindowingHighlight];
		style.Colors[ImGuiCol_NavWindowingDimBg];
		style.Colors[ImGuiCol_ModalWindowDimBg];

		// Text colors
		style.Colors[ImGuiCol_Text];
		style.Colors[ImGuiCol_TextDisabled];
		style.Colors[ImGuiCol_TextLink];
		style.Colors[ImGuiCol_TextSelectedBg];
		style.Colors[ImGuiCol_TreeLines];
		style.Colors[ImGuiCol_UnsavedMarker];
		style.Colors[ImGuiCol_InputTextCursor];
	}

	void UIWrapper::ShowThemeColorEditor()
	{
        UIWrapper::BeginWindow("ImGui Color Editor");

		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// Array of all ImGuiCol enum names
		static const char* colorNames[] = {
			"Text",
			"TextDisabled",
			"WindowBg",
			"ChildBg",
			"PopupBg",
			"Border",
			"BorderShadow",
			"FrameBg",
			"FrameBgHovered",
			"FrameBgActive",
			"TitleBg",
			"TitleBgActive",
			"TitleBgCollapsed",
			"MenuBarBg",
			"ScrollbarBg",
			"ScrollbarGrab",
			"ScrollbarGrabHovered",
			"ScrollbarGrabActive",
			"CheckMark",
			"CheckboxSelectedBg",
			"SliderGrab",
			"SliderGrabActive",
			"Button",
			"ButtonHovered",
			"ButtonActive",
			"Header",
			"HeaderHovered",
			"HeaderActive",
			"Separator",
			"SeparatorHovered",
			"SeparatorActive",
			"ResizeGrip",
			"ResizeGripHovered",
			"ResizeGripActive",
			"InputTextCursor",
			"TabHovered",
			"Tab",
			"TabSelected",
			"TabSelectedOverline",
			"TabDimmed",
			"TabDimmedSelected",
			"TabDimmedSelectedOverline",
			"DockingPreview",
			"DockingEmptyBg",
			"PlotLines",
			"PlotLinesHovered",
			"PlotHistogram",
			"PlotHistogramHovered",
			"TableHeaderBg",
			"TableBorderStrong",
			"TableBorderLight",
			"TableRowBg",
			"TableRowBgAlt",
			"TextLink",
			"TextSelectedBg",
			"TreeLines",
			"DragDropTarget",
			"DragDropTargetBg",
			"UnsavedMarker",
			"NavCursor",
			"NavWindowingHighlight",
			"NavWindowingDimBg",
			"ModalWindowDimBg",
		};

		if (ImGui::BeginTable("ColorEditorTable", 2, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg))
		{
			ImGui::TableSetupColumn("Color Name", ImGuiTableColumnFlags_WidthStretch, 0.4f);
			ImGui::TableSetupColumn("Color Picker", ImGuiTableColumnFlags_WidthStretch, 0.6f);
			ImGui::TableHeadersRow();

			for (int i = 0; i < ImGuiCol_COUNT; i++)
			{
				ImGui::TableNextRow();
				ImGui::TableSetColumnIndex(0);
				ImGui::Text("%s", colorNames[i]);

				ImGui::TableSetColumnIndex(1);
				ImGui::PushID(i);
				ImGui::ColorEdit4(
					"##color_edit",
					(float*)&colors[i],
					ImGuiColorEditFlags_AlphaBar |
					ImGuiColorEditFlags_DisplayRGB |
					ImGuiColorEditFlags_NoLabel
				);
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		// Reset button
		ImGui::Spacing();
		ImGui::Separator();
		if (ImGui::Button("Reset to Dark Theme", ImVec2(-1, 0)))
		{
			ImGui::StyleColorsDark();
			ApplyThemeColors();
		}

		ImGui::End();
	}

	ImVec4 UIWrapper::Color(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a)
	{
		return ImVec4(
			float(_r) / 255.f,
			float(_g) / 255.f,
			float(_b) / 255.f,
			float(_a) / 255.f
		);
	}
}