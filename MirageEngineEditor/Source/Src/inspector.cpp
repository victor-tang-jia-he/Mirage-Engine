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
#include <application.h>

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
#include <AI/responsive_curve.h>
#include <AI/ai_action.h>
#include <Components/nav_agent.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogInspector, Loggers::Color::CYAN);

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Resources;

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
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TableNextColumn();

						for (auto [id, func] : type.func())
						{
							const char* funcName = func.name();
							std::string btnLabel = (funcName && funcName[0] != '\0')
								? funcName
								: "Invoke " + std::to_string(static_cast<uint32_t>(id));

							float btnWidth = ImGui::GetContentRegionAvail().x;
							if (ImGui::Button(btnLabel.c_str(), ImVec2(btnWidth, 25)))
							{
								entt::meta_any instance = type.from_void(comp.get());
								func.invoke(instance);
							}
						}
						ImGui::EndTable();

						if (auto* agent = dynamic_cast<UtilityAgent*>(comp.get()))
						{
							DrawAIMonitor(agent);
						}
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

		if (ImGui::BeginDragDropTarget())
                {
                    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
                    {
                        std::filesystem::path path = (const char*)payload->Data;
                        std::string name = path.stem().string();
                        std::string ext = path.extension().string();

                        if (ext == ".h")
                        {
                            auto type = entt::resolve(entt::hashed_string{ name.c_str() });
                            if (type)
                            {
                                LOG(logInfo, LogInspector, "Adding Script Component: %s", name.c_str());
                            }
                        }
                    }
                    ImGui::EndDragDropTarget();
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
			ResolveString(value, label, _data, instance);
		else if (actualType == entt::resolve<Math::Vector3>())
			ResolveVec3(value, label, _data, instance);
		else if (actualType == entt::resolve<Math::Quaternion>())
			ResolveQuaternion(value, label, _data, instance);
		else if (actualType == entt::resolve<float>())
			ResolveFloat(value, label, _data, instance);
		else if (actualType == entt::resolve<int>())
			ResolveInt(value, label, _data, instance);
		else if (actualType == entt::resolve<unsigned int>())
			ResolveUnsignedInt(value, label, _data, instance);
		else if (actualType == entt::resolve<bool>())
			ResolveBool(value, label, _data, instance);
		else if (actualType.is_pointer_like())
			ResolvePointer(value, label, _data, instance);
		else if (actualType == entt::resolve<AI::ResponsiveCurve>())
		{
			if (ImGui::TreeNode(label)) {
				AI::ResponsiveCurve* curve = value.try_cast<AI::ResponsiveCurve>();
				if (curve) {
					float samples[50];
					for (int i = 0; i < 50; i++) samples[i] = curve->Evaluate((float)i / 49.0f);
					ImGui::PlotLines("##Preview", samples, 50, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 80));

					auto curveType = entt::resolve<AI::ResponsiveCurve>();
					for (auto [id, childData] : curveType.data()) {
						DrawPropertyDirect(curveType, childData, curve);
					}
				}
				ImGui::TreePop();
			}
		}
		else if (!actualType.is_arithmetic() && !actualType.is_pointer_like() && !value.type().is_sequence_container())
		{
			if (ImGui::TreeNode(label))
			{
				for (auto [id, childData] : actualType.data())
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(childData.name());
					ImGui::TableNextColumn();
					ImGui::PushItemWidth(-1);
					DrawNestedProperty(childData, value);
					ImGui::PopItemWidth();
				}
				_data.set(instance, value);
				ImGui::TreePop();
			}
		}
		else if (value.type().is_sequence_container())
		{
			if (ImGui::TreeNode(label)) {
				auto view = value.as_sequence_container();
				int index = 0;

				if (ImGui::Button("+ Add New Element")) {
					if (view.value_type() == entt::resolve<float>())
						view.insert(view.end(), 0.0f);
					else if (view.value_type() == entt::resolve<std::string>())
						view.insert(view.end(), std::string("NewChannel"));

					_data.set(instance, value);
				}
				ImGui::Separator();

				for (auto it = view.begin(); it != view.end(); )
				{
					ImGui::PushID(index);
					bool elementDeleted = false;

					if (ImGui::Button("X")) {
						it = view.erase(it);
						_data.set(instance, value);
						elementDeleted = true;
					}

					if (!elementDeleted) {
						ImGui::SameLine();
						std::string itemLabel = "##" + std::to_string(index);
						entt::meta_any item = *it;

						if (item.type() == entt::resolve<float>()) {
							float val = item.cast<float>();
							if (ImGui::DragFloat(itemLabel.c_str(), &val, 0.1f)) {
								it = view.erase(it);
								it = view.insert(it, val); 
								_data.set(instance, value);
							}
						}
						else if (item.type() == entt::resolve<std::string>()) {
							std::string str = item.cast<std::string>();
							char buffer[256];
							memset(buffer, 0, sizeof(buffer));
							strncpy(buffer, str.c_str(), sizeof(buffer) - 1);

							if (ImGui::InputText(itemLabel.c_str(), buffer, 256)) {
								it = view.erase(it);
								it = view.insert(it, std::string(buffer));
								_data.set(instance, value);
							}
						}
						++it;
						index++;
					}

					ImGui::PopID();
				}
				ImGui::TreePop();
			}
		}
		else if (actualType.is_enum())
		{
			int currentValue = 0;
			std::string previewName = "Unknown";

			for (auto [id, enumData] : actualType.data())
			{
				if (enumData.get({}).cast<int>() == value.cast<int>())
				{
					previewName = enumData.name();
					break;
				}
			}

			if (ImGui::BeginCombo(label, previewName.c_str()))
			{
				for (auto [id, enumData] : actualType.data())
				{
					bool isSelected = (enumData.get({}).cast<int>() == value.cast<int>());

					if (ImGui::Selectable(enumData.name(), isSelected))
					{
						_data.set(instance, enumData.get({}));
					}

					if (isSelected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}

		}
	}

	void Inspector::DrawNestedProperty(entt::meta_data _childData, entt::meta_any& _parentInstance)
	{
		entt::meta_any childValue = _childData.get(_parentInstance);
		if (!childValue) return;

		auto type = childValue.type();
		const char* name = _childData.name();

		if (type == entt::resolve<float>()) {
			float v = childValue.cast<float>();
			if (ImGui::DragFloat(name, &v, 0.1f)) _childData.set(_parentInstance, v);
		}
		else if (type == entt::resolve<bool>()) {
			bool v = childValue.cast<bool>();
			if (ImGui::Checkbox(name, &v)) _childData.set(_parentInstance, v);
		}
		else if (type.is_sequence_container())
		{
			auto view = childValue.as_sequence_container();
			if (ImGui::TreeNode(name))
			{
				InfluenceGrid* grid = nullptr;
				auto app = Application::Instance();
				if (app) grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();

				int index = 0;
				int indexToModify = -1;
				float newValue = 0.0f;
				bool shouldAdd = false;
				int indexToDelete = -1;

				for (auto item : view)
				{
					ImGui::PushID(index);

					if (ImGui::Button("X")) { indexToDelete = index; }
					ImGui::SameLine();

					float val = item.cast<float>();

					std::string label;
					if (grid && index < (int)grid->GetChannelsNames().size())
						label = grid->GetChannelsNames()[index];
					else
						label = "Weight " + std::to_string(index);

					if (ImGui::DragFloat(label.c_str(), &val, 0.1f)) {
						indexToModify = index;
						newValue = val;
					}

					ImGui::PopID();
					index++;
				}

				if (ImGui::Button("+ Add Weight")) { shouldAdd = true; }
				if (indexToModify != -1) {
					auto it = view.begin();
					for (int i = 0; i < indexToModify; ++i) ++it;
					it = view.erase(it);   
					view.insert(it, newValue);
					_childData.set(_parentInstance, childValue);
				}

				if (indexToDelete != -1) {
					auto it = view.begin();
					for (int i = 0; i < indexToDelete; ++i) ++it;
					view.erase(it);
					_childData.set(_parentInstance, childValue);
				}

				if (shouldAdd) {
					view.insert(view.end(), 0.0f);
					_childData.set(_parentInstance, childValue);
				}

				ImGui::TreePop();
			}
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
		//if (entt::meta_any ref = *_value; ref)
		//{
		//	if (Core::Components::Component* component = ref.try_cast<Core::Components::Component>())
		//	{
		//		const char* componentLabel = component ? "Component Assigned" : "None";
		//		ImGui::TextUnformatted(_label);
		//		ImGui::SameLine();
		//		ImGui::Button(componentLabel, ImVec2(-1, 0));
		//		return true;
		//	}
		//}
		//return false;

		std::string btnLabel = "None (Drop here)";
		entt::meta_any ref = *_value;
		if (ref) btnLabel = "Assigned";

		ImGui::TextUnformatted(_label);
		ImGui::SameLine();
		ImGui::Button((btnLabel + "##" + _label).c_str(), ImVec2(-1, 0));

		return true;
	}

	bool Inspector::ResolveResource(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		auto typePointed = _data.type();
		auto typeHash = typePointed.info().hash();

		bool isMesh = (typeHash == entt::type_id<std::shared_ptr<Mesh>>().hash() || typeHash == entt::type_id<Mesh*>().hash());
		bool isTex = (typeHash == entt::type_id<std::shared_ptr<Texture>>().hash() || typeHash == entt::type_id<Texture*>().hash());
		bool isMat = (typeHash == entt::type_id<std::shared_ptr<MaterialInstance>>().hash() || typeHash == entt::type_id<MaterialInstance*>().hash());

		if (!isMesh && !isTex && !isMat) return false;

		std::string resourceName = "None (Drop here)";

		if (entt::meta_any ref = *_value; ref)
		{
			if (auto* res = ref.try_cast<IResource>()) {
				resourceName = res->GetName();
			}
			else if (void** ptrAddr = ref.try_cast<void*>()) {
				if (*ptrAddr) resourceName = static_cast<IResource*>(*ptrAddr)->GetName();
			}
		}

		ImGui::TextUnformatted(_label);
		ImGui::SameLine();

		if (resourceName != "None (Drop here)") ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.4f, 0.6f, 1.0f));
		ImGui::Button((resourceName + "##" + _label).c_str(), ImVec2(-1, 0));
		if (resourceName != "None (Drop here)") ImGui::PopStyleColor();

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				std::filesystem::path path = (const char*)payload->Data;
				std::string name = path.stem().string();
				std::string ext = path.extension().string();
				auto& rm = Core::Resources::ResourceManager::Instance();

				if (isMesh && ext == ".obj") {
					_data.set(_instance, rm.GetMesh(name));
				}
				else if (isTex && (ext == ".png" || ext == ".jpg")) {
					_data.set(_instance, rm.GetTexture(name));
				}
				else if (isMat && ext == ".mat") {
					_data.set(_instance, rm.GetMaterial(name));
				}
				else {
					LOG(logWarning, LogInspector, "Incompatible extension %s for field %s", ext.c_str(), _label);
				}
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

			bool hasMesh = _selectedObj->GetComponent<MeshRenderer>() != nullptr;
			if (ImGui::MenuItem("Mesh Renderer", nullptr, false, !hasMesh))
			{
				_selectedObj->AddComponent<MeshRenderer>();
				ImGui::CloseCurrentPopup();
			}
			if (hasMesh && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Mesh Renderer");

			bool hasCamera = _selectedObj->GetComponent<Camera>() != nullptr;
			if (ImGui::MenuItem("Camera", nullptr, false, !hasCamera))
			{
				_selectedObj->AddComponent<Camera>();
				ImGui::CloseCurrentPopup();
			}
			if (hasCamera && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Camera");

			bool hasInfluenceGrid = _selectedObj->GetComponent<InfluenceGrid>() != nullptr;
			if (ImGui::MenuItem("Influence Grid", nullptr, false, !hasInfluenceGrid))
			{
				_selectedObj->AddComponent<InfluenceGrid>();
				ImGui::CloseCurrentPopup();
			}
			if (hasInfluenceGrid && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Grid");

			bool hasInfluenceEmitter = _selectedObj->GetComponent<InfluenceEmitter>() != nullptr;
			if (ImGui::MenuItem("Influence Emitter", nullptr, false, !hasInfluenceEmitter))
			{
				_selectedObj->AddComponent<InfluenceEmitter>();
				ImGui::CloseCurrentPopup();
			}
			if (hasInfluenceEmitter && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has an Influence Emitter");

			bool hasInputMapping = _selectedObj->GetComponent<MirageEngine::Core::Input::InputMapping>() != nullptr;
			if (ImGui::MenuItem("Input Mapping", nullptr, false, !hasInputMapping))
			{
				_selectedObj->AddComponent<MirageEngine::Core::Input::InputMapping>();
				ImGui::CloseCurrentPopup();
			}
			if (hasInputMapping && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has an InputMapping");

			bool hasNavAgent = _selectedObj->GetComponent<NavAgent>() != nullptr;
			if (ImGui::MenuItem("Nav Agent", nullptr, false, !hasNavAgent))
			{
				_selectedObj->AddComponent<NavAgent>();
				ImGui::CloseCurrentPopup();
			}
			if (hasNavAgent && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has an Nav Agent");

			bool hasUtilityAgent = _selectedObj->GetComponent<UtilityAgent>() != nullptr;
			if (ImGui::MenuItem("UtilityAgent", nullptr, false, !hasUtilityAgent))
			{
				_selectedObj->AddComponent<UtilityAgent>();
				ImGui::CloseCurrentPopup();
			}
			if (hasUtilityAgent && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has an Utility Agent");

			ImGui::EndPopup();
		}
	}

	void Inspector::DrawAIMonitor(UtilityAgent* _agent)
        {
            if (!_agent)
                return;

            ImGui::Spacing();
            ImGui::TextColored(ImVec4(0, 1, 1, 1), "--- AI REAL-TIME MONITOR ---");

            if (ImGui::CollapsingHeader("Sensors (Grid Perception)", ImGuiTreeNodeFlags_DefaultOpen))
            {
                InfluenceGrid* grid = _agent->GetInfluenceGrid();
                const auto& sensors = _agent->GetSensors();

                for (size_t i = 0; i < sensors.size(); ++i)
                {
                    std::string name = (grid && i < grid->GetChannelsNames().size()) ? grid->GetChannelsNames()[i] : "Channel " + std::to_string(i);

                    ImGui::TextUnformatted(name.c_str());
                    ImGui::SameLine(100);
                    ImGui::ProgressBar(sensors[i], ImVec2(-1, 0));
                }
            }

            if (ImGui::CollapsingHeader("Internal Stats (Blackboard)", ImGuiTreeNodeFlags_DefaultOpen))
            {
                for (auto const& [name, val] : _agent->GetInternalStatsMap())
                {
                    ImGui::Text("%s : %.2f", name.c_str(), val);
                }
            }

            if (ImGui::CollapsingHeader("Decision Scores", ImGuiTreeNodeFlags_DefaultOpen))
            {
                const auto& scores = _agent->GetLastScores();
                const auto& actions = _agent->GetActions();

                if (!scores.empty() && scores.size() == actions.size())
                {
                    for (size_t i = 0; i < actions.size(); ++i)
                    {
                        float s = scores[i];
                        std::string actionName = actions[i]->GetName();

                        bool isActive = (_agent->GetLastDecisionReason().find(actionName) != std::string::npos);
                        if (isActive)
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1, 1, 0, 1));

                        ImGui::Text("%s", actionName.c_str());
                        ImGui::SameLine(100);
                        ImGui::SliderFloat(("##" + actionName).c_str(), &s, 0.0f, 1.2f, "%.3f", ImGuiSliderFlags_NoInput);

                        if (isActive)
                            ImGui::PopStyleColor();
                    }
                }
                ImGui::Separator();
                ImGui::TextWrapped("Reason: %s", _agent->GetLastDecisionReason().c_str());
            }
        }
}