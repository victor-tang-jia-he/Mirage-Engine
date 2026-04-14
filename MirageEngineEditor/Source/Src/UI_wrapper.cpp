#include "UI_wrapper.h"

#include "GLFW/glfw3.h"

#include "vector3.h"
#include <string>

#include "imgui.h"
#include "ImGuizmo.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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

		ImGuizmo::SetImGuiContext(context);

		GLFWwindow* window = static_cast<GLFWwindow*>(_windowHandle);
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
	}

	void UIWrapper::Shutdown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void UIWrapper::BeginFrame()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
		ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);
		//ImGui::ShowDemoWindow();
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

	static std::string s_NextModalToOpen = "";

	void UIWrapper::RequestModal(const char* _modalName) {
		s_NextModalToOpen = _modalName;
	}

	void UIWrapper::UpdateModalRequests() {
		if (!s_NextModalToOpen.empty()) {
			ImGui::OpenPopup(s_NextModalToOpen.c_str());
			s_NextModalToOpen = "";
		}
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

	//WidgetAction UIWrapper::DrawAssetTile(const char* _name, bool _isSelected, void* _icon)
	//{
	//	ImGui::TableNextColumn();
	//	ImGui::PushID(_name);

	//	WidgetAction action = WidgetAction::None;

	//	if (_icon == nullptr)
	//	{

	//	}

	//	ImGui::BeginGroup();
	//	if (_isSelected)
	//	{

	//	}
	//	ImGui::Button("##icon", ImVec2(64, 64));
	//	ImGui::TextWrapped(_name);
	//	ImGui::EndGroup();

	//	if (ImGui::IsItemHovered())
	//	{
	//		if (ImGui::IsMouseDoubleClicked(0))
	//			action = WidgetAction::DoubleClicked;
	//		else if (ImGui::IsItemClicked(0))
	//			action = WidgetAction::Clicked;
	//		else if (ImGui::IsItemClicked(1))
	//			action = WidgetAction::RightClicked;
	//	}

	//	//ImGui::PopID();
	//	return action;
	//}

	WidgetAction UIWrapper::DrawAssetTile(const char* _name, bool _isSelected, void* _icon)
	{
		ImGui::TableNextColumn();
		ImGui::PushID(_name);

		if (_icon == nullptr)
		{

		}

		WidgetAction action = WidgetAction::None;
		ImVec2 startPos = ImGui::GetCursorPos();
		float columnWidth = ImGui::GetContentRegionAvail().x;
		ImVec2 tileSize = ImVec2(columnWidth, 110.0f);

		ImGui::BeginGroup();

		float iconSize = 64.0f;
		float iconPadding = (columnWidth - iconSize) * 0.5f;
		ImVec2 screenPos = ImGui::GetCursorScreenPos();

		ImGui::GetWindowDrawList()->AddRectFilled(
			ImVec2(screenPos.x + iconPadding, screenPos.y),
			ImVec2(screenPos.x + iconPadding + iconSize, screenPos.y + iconSize),
			ImColor(60, 60, 70),
			4.0f 
		);

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + iconSize + 8.0f);

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
			if (ImGui::IsMouseDoubleClicked(0)) action = WidgetAction::DoubleClicked;
			else if (ImGui::IsItemClicked(0)) action = WidgetAction::Clicked;
			else if (ImGui::IsItemClicked(1)) action = WidgetAction::RightClicked;
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
}