#include "hierarchy.h"
#include "UI_wrapper.h"
#include "dragdrop.h"
#include "SceneGraph/scene_graph.h"
#include "GameObject/game_object.h"
#include "imgui.h"

namespace Mirage
{
	void Hierarchy::InitPanel()
	{

	}

	void Hierarchy::DrawPanel([[maybe_unused]] uint32_t _textureID)
	{
		m_AnItemWasRightClicked = false;
		UIWrapper::BeginWindow("Hierarchy");

		DrawHierarchyTree();

		ImGui::Dummy(ImGui::GetContentRegionAvail());

		if (ImGui::IsWindowHovered() && !m_AnItemWasRightClicked && ImGui::IsMouseClicked(1))
		{
			m_Context->SetSelectedObject(nullptr);

			UIWrapper::RequestModal("HierarchyEmptyMenu");
		}

		if (ImGui::BeginDragDropTarget())
		{
			void* droppedPointer = DragDrop::GetPointerPayload(DragDropTag::HIERARCHY_ITEM);
			if (droppedPointer != nullptr)
			{
				std::shared_ptr<MirageEngine::GameObject> draggedChild = *static_cast<std::shared_ptr<MirageEngine::GameObject>*>(droppedPointer);
				m_Context->SetParent(nullptr, draggedChild);
			}
			ImGui::EndDragDropTarget();
		}

		UIWrapper::UpdateModalRequests();

		RenderHierarchyEmptyMenu();
		RenderHierarchyItemMenu();
		UIWrapper::EndWindow();
	}

	void Hierarchy::ShutdownPanel()
	{

	}

	void Hierarchy::DrawHierarchyTree()
	{
        std::vector<std::shared_ptr<MirageEngine::GameObject>> objs = m_Context->GetChildren(nullptr);

		for (std::shared_ptr<MirageEngine::GameObject> obj : objs)
		{
			RenderRecursiveNode(obj);
		}
	}

	void Hierarchy::RenderRecursiveNode(std::shared_ptr<MirageEngine::GameObject> _obj)
	{
		std::string name = _obj->GetName();
		uint64_t id = _obj->GetID();
		std::string idStr = std::to_string(id);
		bool bisLeaf = _obj->GetChildren().empty();
		bool bisSelected = (m_Context->GetSelectedObject() == _obj);

		if (UIWrapper::BeginFolderNode(name.c_str(), idStr.c_str(), bisSelected, bisLeaf))
		{
			bool bHierarchyDirty = false;
			if (ImGui::BeginDragDropSource())
			{
				DragDrop::SetPointerPayload(DragDropTag::HIERARCHY_ITEM, &_obj);
				ImGui::Text("Moving Object: %s", name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{
				void* droppedPointer = DragDrop::GetPointerPayload(DragDropTag::HIERARCHY_ITEM);
				if (droppedPointer != nullptr)
				{
                    std::shared_ptr<MirageEngine::GameObject> draggedChild = *static_cast<std::shared_ptr<MirageEngine::GameObject>*>(droppedPointer);
					bool success = m_Context->SetParent(_obj, draggedChild);

					if (success)
						bHierarchyDirty = true;
				}
				ImGui::EndDragDropTarget();
			}

			if (UIWrapper::IsItemClicked())
			{
				m_Context->SetSelectedObject(_obj);
			}
			if (ImGui::IsItemClicked(1))
			{
				m_Context->SetSelectedObject(_obj);
				UIWrapper::RequestModal("HierarchyItemMenu");
				m_AnItemWasRightClicked = true;
			}

			if (!bisLeaf)
			{
				std::vector<std::shared_ptr<MirageEngine::GameObject>> objChildren = _obj->GetChildren();
				if (!bHierarchyDirty)
				{
					for (std::shared_ptr<MirageEngine::GameObject> obj : objChildren)
					{
						RenderRecursiveNode(obj);
					}
				}
			}

			UIWrapper::EndFolderNode();
		}
	}

	void Hierarchy::RenderHierarchyItemMenu()
	{
		if (ImGui::BeginPopup("HierarchyItemMenu"))
		{
			std::shared_ptr<MirageEngine::GameObject> selected = m_Context->GetSelectedObject();
			if (selected) {
				if (ImGui::MenuItem("Create Child")) {
					m_Context->CreateGameObject("New Child", selected);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Delete", "Del")) {
					m_Context->RemoveGameObject(selected);
					ImGui::CloseCurrentPopup();
				}
			}
			ImGui::EndPopup();
		}
	}

	void Hierarchy::RenderHierarchyEmptyMenu()
	{
		if (!m_AnItemWasRightClicked)
		{
			if (ImGui::BeginPopup("HierarchyEmptyMenu"))
			{
				if (ImGui::MenuItem("Create Empty Object")) {
					m_Context->CreateGameObject("New GameObject", nullptr);
				}
				ImGui::EndPopup();
			}
		}
	}
}