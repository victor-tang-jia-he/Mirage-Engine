#include <dragdrop.h>
#include "inspector.h"
#include "UI_wrapper.h"
#include "matrix4.h"
#include "imgui.h"
#include "GameObject/game_object.h"
#include "SceneGraph/scene_graph.h"
#include <string>
#include <cmath>
#include <limits>
#include <fstream>
#include <Resources/resource_manager.h>

// RESOURCES HEADERS
#include <Resources/mesh.h>
#include <Resources/texture.h>
#include <Resources/material.h>
#include <Resources/shader.h>

// COMPONENTS HEADERS
#include "Components/camera.h"
#include "Components/mesh_renderer.h"
#include "Components/utility_agent.h"
#include "Components/influence_emitter.h"
#include <Input/input_mapping.h>
#include <AI/influence_grid.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogInspector, Loggers::Color::CYAN);

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;
using namespace MirageEngine::Core::Components;

namespace Mirage
{
	void Inspector::InitPanel() {}

	void Inspector::DrawPanel([[maybe_unused]] uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Inspector");

		std::shared_ptr<GameObject> selectedObj = m_Context->GetSelectedObject();
		if (selectedObj == nullptr)
		{
			ImGui::TextDisabled("No object selected in Hierarchy");
			UIWrapper::EndWindow();
			return;
		}

		ImGui::Text("Entity: %s", selectedObj->GetName().c_str());
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			auto goType = entt::resolve(entt::type_id<GameObject>().hash());
			if (goType) {
				// Position and Rotation from GameObject (physics-aware)
				for (const char* propName : { "Position", "Rotation", "Scale" })
				{
					auto data = goType.data(entt::hashed_string{ propName });
					if (data)
					{
						DrawPropertyDirect(goType, data, (void*)selectedObj.get());
					}
				}
			}
		}

		// Components
		// 1. Loop through all components attached to the selected object
		for (std::shared_ptr<MirageEngine::Core::Components::Component> comp : selectedObj->GetComponents())
		{
			// 2. Ask EnTT if it has meta-data for this component's Type ID
			auto type = entt::resolve(comp->GetType());
			if (type)
			{
				const char* displayName = type.name();
				// Create a CollapsingHeader using the class name registered in Meta
				if (ImGui::CollapsingHeader(displayName, ImGuiTreeNodeFlags_DefaultOpen))
				{
					ImGui::PushID(static_cast<int>(comp->GetType()));

					if (ImGui::BeginTable("CompTable", 2, ImGuiTableFlags_SizingStretchProp))
					{
						ImGui::TableSetupColumn("Label", ImGuiTableColumnFlags_WidthFixed, 80.0f);
						ImGui::TableSetupColumn("Field", ImGuiTableColumnFlags_WidthStretch);

						// 3. Loop through every variable (.data()) registered for this type
						for (auto [id, data] : type.data())
						{
							ImGui::TableNextRow();
							ImGui::TableNextColumn();
							ImGui::TextUnformatted(data.name());

							ImGui::TableNextColumn();
							ImGui::PushItemWidth(-1);
							// 4. Call our "Dispatcher" to draw the appropriate ImGui widget
							DrawPropertyDirect(type, data, (void*)comp.get());
							ImGui::PopItemWidth();
						}
						ImGui::EndTable();
					}
					ImGui::PopID();
				}
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();

		float width = ImGui::GetWindowWidth();
		ImGui::SetCursorPosX(width * 0.1f);
		if (ImGui::Button("Add Component", ImVec2(width * 0.8f, 30)))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}

		RenderAddComponentPopup(selectedObj);

		UIWrapper::EndWindow();
	}

	void Inspector::ShutdownPanel() {}

	void Inspector::DrawPropertyDirect(entt::meta_type _type, entt::meta_data _data, void* _instancePtr)
	{
		// entt::meta_any is a generic container (like a smart void*). 
		// from_void() links our raw memory pointer to the EnTT reflection logic and creates a handle that lets EnTT know which object to modify.
		entt::meta_any instance = _type.from_void(_instancePtr);

		// _data.get(instance) invokes the registered "Getter" function automatically
		entt::meta_any value = _data.get(instance);

		if (!value) return;

		auto actualType = value.type();

		const char* label = _data.name(); // The string name we gave in .data("Name")
		if (!label) label = "Property";

		// Dispatcher: Choose the ImGui widget based on the Data Type
		if (actualType == entt::resolve<std::string>())
		{
			ResolveString(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<Math::Vector3>())
		{
			ResolveVec3(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<Math::Quaternion>())
		{
			ResolveQuaternion(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<float>())
		{
			ResolveFloat(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<int>())
		{
			ResolveInt(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<unsigned int>())
		{
			ResolveUnsignedInt(value, label, _data, instance);
		}
		else if (actualType == entt::resolve<bool>())
		{
			ResolveBool(value, label, _data, instance);
		}
		else if (actualType.is_pointer_like())
		{
			// Checks if the component contains refs to other components or ressources
			ResolvePointer(value, label, _data, instance);
		}
	}

	void Inspector::ResolveVec3(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Vector 3 detected in component");
			printed = true;
		}

		Math::Vector3 vec = _value.cast<Math::Vector3>();
		if (ImGui::DragFloat3(_label, &vec.X, 0.1f))
		{
			// If label is Scale
			if (_label && std::strcmp(_label, "Scale") == 0)
			{
				const float minAbs = EPSILON;
				if (std::fabs(vec.X) < minAbs) vec.X = (vec.X >= 0.0f) ? minAbs : -minAbs;
				if (std::fabs(vec.Y) < minAbs) vec.Y = (vec.Y >= 0.0f) ? minAbs : -minAbs;
				if (std::fabs(vec.Z) < minAbs) vec.Z = (vec.Z >= 0.0f) ? minAbs : -minAbs;
			}
			_data.set(_instance, vec);
		}
	}

	void Inspector::ResolveQuaternion(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Quaternion detected in component");
			printed = true;
		}

		Math::Quaternion quat = _value.cast<Math::Quaternion>();
		Math::Vector3 euler = quat.ToEulerAngle() * 180.0f / PI;
		if (ImGui::DragFloat3(_label, &euler.X, 0.1f))
		{
			euler = euler * PI / 180.0f;
			Math::Quaternion qx = Math::Quaternion(euler.X, 0, 0);
			Math::Quaternion qy = Math::Quaternion(0, euler.Y, 0);
			Math::Quaternion qz = Math::Quaternion(0, 0, euler.Z);
			Math::Quaternion eulerQuat = qx * qy * qz;
			_data.set(_instance, eulerQuat);
		}
	}

	void Inspector::ResolveString(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "String detected in component");
			printed = true;
		}
		std::string val = _value.cast<std::string>();
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		std::strncpy(buffer, val.c_str(), sizeof(buffer) - 1);
		if (ImGui::InputText(_label, buffer, sizeof(buffer)))
		{
			_data.set(_instance, std::string(buffer));
		}
	}

	void Inspector::ResolveInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Int detected in component");
			printed = true;
		}

		int val = _value.cast<int>();
		if (ImGui::DragInt(_label, &val, 1))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveFloat(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Float detected in component");
			printed = true;
		}

		float val = _value.cast<float>();
		if (ImGui::DragFloat(_label, &val, 0.1f))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveBool(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Bool detected in component");
			printed = true;
		}

		bool val = _value.cast<bool>();
		if (ImGui::Checkbox(_label, &val))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveUnsignedInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Unsign Int detected in component");
			printed = true;
		}

		unsigned int val = _value.cast<unsigned int>();
		if (ImGui::DragScalar(_label, ImGuiDataType_U32, &val, 1.0f))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolvePointer(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		if (ResolveResource(_value, _label, _data, _instance))
			return;

		if (ResolveComponent(_value, _label, _data, _instance))
			return;
	}

	bool Inspector::ResolveComponent(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		if (entt::meta_any ref = *_value; ref)
		{
			if (Core::Components::Component* component = ref.try_cast<Core::Components::Component>())
			{
				const char* componentLabel = component ? "Component Assigned" : "None";
				ImGui::TextUnformatted(_label);
				ImGui::SameLine();
				ImGui::Button(componentLabel, ImVec2(-1, 0));
				return true;
			}
		}
		return false;
	}

	bool Inspector::ResolveResource(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		auto typePointed = _data.type();

		bool isResource = false;
		for (auto [id, baseType] : typePointed.base()) {
			if (id == entt::type_id<Core::Resources::IResource>().hash()) {
				isResource = true;
				break;
			}
		}

		if (!isResource && !typePointed.is_pointer_like()) return false;

		std::string resourceName = "None (Drop here)";
		if (entt::meta_any ref = *_value; ref)
		{
			if (auto* res = ref.try_cast<Core::Resources::IResource>()) {
				resourceName = res->GetName();
			}
		}

		ImGui::TextUnformatted(_label);
		ImGui::SameLine();

		if (ImGui::Button((resourceName + "##" + _label).c_str(), ImVec2(-1, 0))) {
			// Action clic
		}

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::filesystem::path path = (const char*)payload->Data;
				std::string name = path.stem().string();

				if (typePointed == entt::resolve<std::shared_ptr<Core::Resources::Mesh>>() || typePointed == entt::resolve<Core::Resources::Mesh*>()) {
					_data.set(_instance, Core::Resources::ResourceManager::Instance().GetMesh(name));
				}
				else if (typePointed == entt::resolve<std::shared_ptr<Core::Resources::Texture>>() || typePointed == entt::resolve<Core::Resources::Texture*>()) {
					_data.set(_instance, Core::Resources::ResourceManager::Instance().GetTexture(name));
				}
				else if (typePointed == entt::resolve<std::shared_ptr<Core::Resources::Material>>() || typePointed == entt::resolve<Core::Resources::Material*>()) {
					_data.set(_instance, Core::Resources::ResourceManager::Instance().GetMaterial(name));
				}

				LOG(logInfo, LogInspector, "Resource %s dropped onto %s", name.c_str(), _label);
			}
			ImGui::EndDragDropTarget();
		}

		return true;
	}

	void Inspector::RenderAddComponentPopup(std::shared_ptr<MirageEngine::GameObject> _selectedObj)
	{
		if (ImGui::BeginPopup("AddComponentPopup"))
		{
			ImGui::TextDisabled("Add Component");
			ImGui::Separator();

			if (ImGui::MenuItem("Mesh Renderer"))
			{
				_selectedObj->AddComponent<MeshRenderer>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Camera"))
			{
				_selectedObj->AddComponent<Camera>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Influence Grid"))
			{
				_selectedObj->AddComponent<InfluenceGrid>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Influence Emitter"))
			{
				_selectedObj->AddComponent<InfluenceEmitter>();
				ImGui::CloseCurrentPopup();
			}

			if (ImGui::MenuItem("Input Mapping"))
			{
				_selectedObj->AddComponent<MirageEngine::Core::Input::InputMapping>();
				ImGui::CloseCurrentPopup();
			}

			ImGui::EndPopup();
		}
	}
}