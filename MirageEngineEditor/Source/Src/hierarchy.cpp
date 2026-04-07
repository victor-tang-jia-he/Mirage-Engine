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
		UIWrapper::BeginWindow("Hierarchy");
		DrawHierarchyTree();
		if (ImGui::BeginDragDropTarget())
		{
			void* droppedPointer = DragDrop::GetPointerPayload(DragDropTag::HIERARCHY_ITEM);
			if (droppedPointer != nullptr)
			{
				auto* draggedChild = static_cast<MirageEngine::GameObject*>(droppedPointer);
				m_Context->SetParent(nullptr, draggedChild);
			}
			ImGui::EndDragDropTarget();
		}
		UIWrapper::EndWindow();
	}

	void Hierarchy::ShutdownPanel()
	{

	}

	void Hierarchy::DrawHierarchyTree()
	{
		std::vector<MirageEngine::GameObject*> objs = m_Context->GetChildren(nullptr);

		for (MirageEngine::GameObject* obj : objs)
		{
			RenderRecursiveNode(obj);
		}
	}

	void Hierarchy::RenderRecursiveNode(MirageEngine::GameObject* _obj)
	{
		std::string name = _obj->GetName();
		uint64_t id = _obj->GetID();
		std::string idStr = std::to_string(id);
		bool bisLeaf = _obj->GetChildren().empty();
		bool bisSelected = (m_Context->GetSelectedObject() == _obj);

		if (UIWrapper::BeginFolderNode(name.c_str(), idStr.c_str(), bisSelected, bisLeaf))
		{
			if (ImGui::BeginDragDropSource())
			{
				DragDrop::SetPointerPayload(DragDropTag::HIERARCHY_ITEM, _obj);
				ImGui::Text("Moving Object: %s", name.c_str());
				ImGui::EndDragDropSource();
			}

			if (ImGui::BeginDragDropTarget())
			{	
				void* droppedPointer = DragDrop::GetPointerPayload(DragDropTag::HIERARCHY_ITEM);
				if (droppedPointer != nullptr)
				{
					MirageEngine::GameObject* draggedChild = static_cast<MirageEngine::GameObject*>(droppedPointer);
					bool success = m_Context->SetParent(_obj, draggedChild);
					ImGui::EndDragDropTarget();

					if (success) {
						return;
					}
				}
			}

			if (UIWrapper::IsItemClicked())
			{
				m_Context->SetSelectedObject(_obj);
			}
			if (!bisLeaf)
			{
				std::vector<MirageEngine::GameObject*> objChildren = _obj->GetChildren();
				for (MirageEngine::GameObject* obj : objChildren)
				{
					RenderRecursiveNode(obj);
				}
			}

			UIWrapper::EndFolderNode();
		}
	}
}