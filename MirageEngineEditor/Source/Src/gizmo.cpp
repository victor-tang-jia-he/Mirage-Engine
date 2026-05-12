#include <gizmo.h>

#include <SceneGraph/scene_graph.h>
#include <Components/camera.h>
#include <Camera/i_camera.h>

#include <vector3.h>
#include <matrix4.h>
#include <array>

#include <imgui.h>
#include <ImGuizmo.h>

DEFINE_LOG_CATEGORY(LogGizmo)

using namespace MirageEngine::Core;

static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);
static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);

void MirageEditor::Gizmo::EditTransform(MirageEngine::SceneGraph* _sceneGraph, std::shared_ptr<ICamera> _camera)
{
	if (!_sceneGraph->GetSelectedObject())
		return;

	MirageEngine::Core::Math::Matrix4 worldMatrix(_sceneGraph->GetSelectedObject()->Transform.GetWorldMatrix());

	if (ImGui::IsKeyPressed(ImGuiKey_T))
	{
		mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
		LOG(logTemp, LogGizmo, "Gizmo Operation: TRANSLATE");
	}
	if (ImGui::IsKeyPressed(ImGuiKey_R))
	{
		mCurrentGizmoOperation = ImGuizmo::ROTATE;
		LOG(logTemp, LogGizmo, "Gizmo Operation: ROTATE");
	}
	if (ImGui::IsKeyPressed(ImGuiKey_Y))
	{
		mCurrentGizmoOperation = ImGuizmo::SCALE;
		LOG(logTemp, LogGizmo, "Gizmo Operation: SCALE");
	}

	if (ImGui::IsKeyPressed(ImGuiKey_L))
	{
		if (mCurrentGizmoMode == ImGuizmo::WORLD)
		{
			mCurrentGizmoMode = ImGuizmo::LOCAL;
			LOG(logTemp, LogGizmo, "Gizmo Mode: LOCAL");
		}
		else
		{
			mCurrentGizmoMode = ImGuizmo::WORLD;
			LOG(logTemp, LogGizmo, "Gizmo Mode: WORLD");
		}
	}

	bool manipulated = ImGuizmo::Manipulate(
		_camera->GetView().ToArray().data(),
		_camera->GetPerspective().ToArray().data(),
		mCurrentGizmoOperation,
		mCurrentGizmoMode,
		worldMatrix.M); 

	if (manipulated)
	{
        std::shared_ptr<MirageEngine::GameObject> parent = _sceneGraph->GetSelectedObject()->GetParent();
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

void MirageEditor::Gizmo::EditView(MirageEngine::SceneGraph* _sceneGraph)
{
	/*std::shared_ptr<Components::Camera> mainCamera = _sceneGraph->GetActiveCamera();
	float* view = mainCamera->GetView().ToArray().data();

	ImVec2 size = ImGui::GetContentRegionAvail();
	ImVec2 pos = ImVec2(200, 50);

	ImGuizmo::ViewManipulate(view, 1, pos, size, 0);

	Math::Matrix4 viewMatrix(view);
	
	mainCamera->Transform().SetLocalMatrix(viewMatrix.Inverse().Transpose());*/
}
