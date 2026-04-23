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
#include <Resources/texture.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogInspector, Loggers::Color::CYAN);

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;

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

	void Inspector::ResolveVec3(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveQuaternion(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveString(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveFloat(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveBool(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolveUnsignedInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
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

	void Inspector::ResolvePointer(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any _instance)
	{
		if (entt::meta_any ref = *_value; ref) 
		{
			if (ResolveComponent(ref))
			{
				return;
			}
			else if (ResolveResource(ref))
			{
				return;
			}
		}
	}

	bool Inspector::ResolveComponent(entt::meta_any _value)
	{
		if (Core::Components::Component* component = _value.try_cast<Core::Components::Component>())
		{
			const char* componentLabel = component ? "Assigned" : "None";

			if (ImGui::Button((std::string(componentLabel) + "##" + _value.type().name()).c_str(), ImVec2(-1, 0)))
			{
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
					}
					ImGui::EndDragDropTarget();
				}
			}
			return true;
		}
		return false;
	}

	bool Inspector::ResolveResource(entt::meta_any _value)
	{
		if (Core::Resources::IResource* resource = _value.try_cast<Core::Resources::IResource>())
		{
			const char* resourceLabel = resource ? "Assigned" : "None";

			if (ImGui::Button((std::string(resourceLabel) + "##" + _value.type().name()).c_str(), ImVec2(-1, 0)))
			{
				if (ImGui::BeginDragDropTarget())
				{
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
					{
					}
					ImGui::EndDragDropTarget();
				}
			}
			return true;
		}
		return false;
	}

	void Inspector::ResolveMesh(entt::meta_any _value)
	{
		Core::Resources::Mesh* meshRawPtr = nullptr;
		if (_value)
		{
			std::shared_ptr<Core::Resources::Mesh> ptr = _value.cast<std::shared_ptr<Core::Resources::Mesh>>();
			meshRawPtr = ptr.get();
		}

		const char* meshLabelPtr = meshRawPtr ? "Assigned" : "None";
		ImGui::TextUnformatted(meshLabelPtr);
		ImGui::SameLine();

		if (ImGui::Button((std::string(meshLabelPtr) + "##Mesh").c_str(), ImVec2(-1, 0)))
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	void Inspector::ResolveMaterial(entt::meta_any _value)
	{
		Core::Resources::Material* materialRawPtr = nullptr;
		if (_value)
		{
			std::shared_ptr<Core::Resources::Material> ptr = _value.cast<std::shared_ptr<Core::Resources::Material>>();
			materialRawPtr = ptr.get();
		}

		const char* materialLabelPtr = materialRawPtr ? "Assigned" : "None";
		ImGui::TextUnformatted(materialLabelPtr);
		ImGui::SameLine();
		if (ImGui::Button((std::string(materialLabelPtr) + "##Material").c_str(), ImVec2(-1, 0)))
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
					// TODO: Handle the dropped asset payload here
					// Extract asset path from payload and assign it
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	void Inspector::ResolveTexture(entt::meta_any _value)
	{
		static bool printed = false;
		if (!printed)
		{
			LOG(logTemp, LogInspector, "Texture Resource detected in component");
			printed = true;
		}

		std::shared_ptr<Core::Resources::Texture> texturePtr = _value.cast<std::shared_ptr<Core::Resources::Texture>>();
		const char* textureLabelPtr = texturePtr ? "Assigned" : "None";
		ImGui::TextUnformatted(textureLabelPtr);
		ImGui::SameLine();
		if (ImGui::Button(textureLabelPtr, ImVec2(-1, 0)))
		{
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
				{
				}
				ImGui::EndDragDropTarget();
			}
		}
	}

	void Inspector::ResolveShader(entt::meta_any _value)
	{
	}

}