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

			m_NextModalToOpen = "HierarchyEmptyMenu";
		}

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

		if (!m_NextModalToOpen.empty()) {
			ImGui::OpenPopup(m_NextModalToOpen.c_str());
			m_NextModalToOpen = "";
		}

		RenderHierarchyEmptyMenu();
		RenderHierarchyItemMenu();
		RenderRenameModal();
		RenderDeleteModal();
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
			bool bHierarchyDirty = false;
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
				m_NextModalToOpen = "HierarchyItemMenu";
				m_AnItemWasRightClicked = true;
			}

			if (!bisLeaf)
			{
				std::vector<MirageEngine::GameObject*> objChildren = _obj->GetChildren();
				if (!bHierarchyDirty)
				{
					for (MirageEngine::GameObject* obj : objChildren)
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
			MirageEngine::GameObject* selected = m_Context->GetSelectedObject();
			if (selected) {
				if (ImGui::MenuItem("Rename"))
				{
					m_NextModalToOpen = "RenameModal";
					ImGui::CloseCurrentPopup();
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Create Empty")) {
					m_Context->CreateGameObject("New Child", selected);
				}
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Create Cube"))
					{
						m_Context->CreateObject3D("New Cube", "cube", selected, m_Renderer);
					}
					if (ImGui::MenuItem("Create Sphere"))
					{
						m_Context->CreateObject3D("New Sphere", "sphere", selected, m_Renderer);
					}
					if (ImGui::MenuItem("Create Cylinder"))
					{
						m_Context->CreateObject3D("New Cylinder", "cylinder", selected, m_Renderer);
					}
					if (ImGui::MenuItem("Create Plane"))
					{
						m_Context->CreateObject3D("New Plane", "plane", selected, m_Renderer);
					}
					if (ImGui::MenuItem("Create Cone"))
					{
						m_Context->CreateObject3D("New Cone", "cone", selected, m_Renderer);
					}
					ImGui::EndMenu();
				}
				if(ImGui::MenuItem("Create Camera"))
				{
					m_Context->CreateCamera("New Camera", selected);
				}
				ImGui::Separator();
				if (ImGui::MenuItem("Delete", "Del")) 
				{
					m_NextModalToOpen = "DeleteModal";
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
				if (ImGui::MenuItem("Create Empty")) {
					m_Context->CreateGameObject("New GameObject", nullptr);
				}
				if (ImGui::BeginMenu("3D Object"))
				{
					if (ImGui::MenuItem("Create Cube"))
					{
						m_Context->CreateObject3D("New Cube", "cube", nullptr, m_Renderer);
					}
					if (ImGui::MenuItem("Create Sphere"))
					{
						m_Context->CreateObject3D("New Sphere", "sphere", nullptr, m_Renderer);
					}
					if (ImGui::MenuItem("Create Cylinder"))
					{
						m_Context->CreateObject3D("New Cylinder", "cylinder", nullptr, m_Renderer);
					}
					if (ImGui::MenuItem("Create Plane"))
					{
						m_Context->CreateObject3D("New Plane", "plane", nullptr, m_Renderer);
					}
					if (ImGui::MenuItem("Create Cone"))
					{
						m_Context->CreateObject3D("New Cone", "cone", nullptr, m_Renderer);
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Create Camera"))
				{
					m_Context->CreateCamera("New Camera", nullptr);
				}
				ImGui::EndPopup();
			}
		}
	}

	void Hierarchy::RenderRenameModal()
	{
		if (ImGui::BeginPopupModal("RenameModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Enter new name:");
			ImGui::InputText("##newName", m_RenameBuffer, sizeof(m_RenameBuffer));

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				m_Context->GetSelectedObject()->SetName(m_RenameBuffer);
				m_RenameBuffer[0] = '\0';
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}

	void Hierarchy::RenderDeleteModal()
	{
		std::error_code ec;
		if (ImGui::BeginPopupModal("DeleteModal", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Are you sure you want to delete?");

			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				if (m_Context->GetSelectedObject())
				{
					m_Context->RemoveGameObject(m_Context->GetSelectedObject());
					std::cout << "Deleted" << std::endl;
					m_Context->SetSelectedObject(nullptr);
				}
				ImGui::CloseCurrentPopup();
			}

			ImGui::SameLine();

			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}