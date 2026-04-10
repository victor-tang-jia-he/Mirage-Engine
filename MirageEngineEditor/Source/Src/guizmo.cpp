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

	if (ImGui::IsKeyPressed(ImGuiKey_L))
	{
		if (mCurrentGizmoMode == ImGuizmo::WORLD)
			mCurrentGizmoMode = ImGuizmo::LOCAL;
		else
			mCurrentGizmoMode = ImGuizmo::WORLD;
	}

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
			_sceneGraph->GetSelectedObject()->SetScale(parentWorld.GetScale());
		}
		else
		{
			_sceneGraph->GetSelectedObject()->SetPosition(worldMatrix.GetTranslation());
			_sceneGraph->GetSelectedObject()->SetRotation(worldMatrix.GetRotation());
			_sceneGraph->GetSelectedObject()->SetScale(worldMatrix.GetScale());

		}
	}
}

void MirageEditor::Guizmo::ViewGuizmo(MirageEngine::SceneGraph* _sceneGraph)
{
	Components::Camera* mainCamera = _sceneGraph->GetActiveCamera();
	float* view = mainCamera->GetView().M;

	ImGuizmo::ViewManipulate(view, 1, ImVec2(0, 0), ImVec2(128, 128), 0);

	Math::Matrix4 viewMatrix(view);
	
	mainCamera->Transform().SetLocalMatrix(viewMatrix.Inverse().Transpose());
}
