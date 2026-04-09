#include <guizmo.h>

#include <SceneGraph/scene_graph.h>
#include <Components/camera.h>

#include <vector3.h>
#include <matrix4.h>

#include <imgui.h>
#include <ImGuizmo.h>

DEFINE_LOG_CATEGORY(LogGuizmo)

using namespace MirageEngine::Core;

void MirageEditor::Guizmo::EditTransform(MirageEngine::SceneGraph* _sceneGraph)
{
	if (!_sceneGraph->GetSelectedObject())
		return;

	MirageEngine::Core::Math::Matrix4 worldMatrix(_sceneGraph->GetSelectedObject()->Transform.GetWorldMatrix());
	Components::Camera* mainCamera = _sceneGraph->GetActiveCamera();

	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
	static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
	if (ImGui::IsKeyPressed(ImGuiKey_T))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	if (ImGui::IsKeyPressed(ImGuiKey_R))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	if (ImGui::IsKeyPressed(ImGuiKey_Y))
		mCurrentGizmoOperation = ImGuizmo::SCALE;

	/*if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
	ImGui::SameLine();
	if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
		mCurrentGizmoOperation = ImGuizmo::SCALE;
	float matrixTranslation[3], matrixRotation[3], matrixScale[3];
	ImGuizmo::DecomposeMatrixToComponents(localMatrix, matrixTranslation, matrixRotation, matrixScale);
	ImGui::InputFloat3("Tr", matrixTranslation, "%.3f", 3);
	ImGui::InputFloat3("Rt", matrixRotation, "%.3f", 3);
	ImGui::InputFloat3("Sc", matrixScale, "%.3f", 3);
	ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, localMatrix);*/

	if (ImGui::IsKeyPressed(ImGuiKey_L))
	{
		if (mCurrentGizmoMode == ImGuizmo::WORLD)
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		else
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

	/*if (mCurrentGizmoOperation != ImGuizmo::SCALE)
	{
		if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		ImGui::SameLine();
		if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}
	static bool useSnap(false);
	if (ImGui::IsKeyPressed(ImGuiKey_U))
		useSnap = !useSnap;
	ImGui::SameLine();

	ImGuiIO& io = ImGui::GetIO();
	ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.y);*/

	if (mCurrentGizmoOperation == ImGuizmo::SCALE) // Desabled Scale Temporarily (TODO: Fix Scale)
		return;
	bool manipulated = ImGuizmo::Manipulate(
		mainCamera->GetView().ToArray().data(),
		mainCamera->GetPerspective().ToArray().data(),
		mCurrentGizmoOperation,
		mCurrentGizmoMode,
		worldMatrix.M); 

	if (manipulated)
	{
		MirageEngine::GameObject* parent = _sceneGraph->GetSelectedObject()->GetParent();
		if (parent)
		{
			Math::Matrix4 parentWorld = parent->Transform.GetWorldMatrix().Inverse() *worldMatrix;
			_sceneGraph->GetSelectedObject()->SetPosition(parentWorld.GetTranslation());
			_sceneGraph->GetSelectedObject()->SetRotation(parentWorld.GetRotation());
		}
		else
		{
			_sceneGraph->GetSelectedObject()->SetPosition(worldMatrix.GetTranslation());
			_sceneGraph->GetSelectedObject()->SetRotation(worldMatrix.GetRotation());
		}
	}
}
