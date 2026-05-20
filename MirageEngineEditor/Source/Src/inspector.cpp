//#include <dragdrop.h>
#include "inspector.h"
#include "UI_wrapper.h"
#include "GameObject/game_object.h"
#include "SceneGraph/scene_graph.h"
#include <application.h>

// RESOURCES HEADERS
#include <Resources/mesh.h>
#include <Resources/material.h>

// COMPONENTS HEADERS
#include "Components/camera.h"
#include "Components/utility_agent.h"
#include "Components/influence_emitter.h"
#include <AI/influence_grid.h>
#include <AI/responsive_curve.h>
#include <AI/ai_action.h>
#include <Components/nav_agent.h>
#include <Components/rigidbody.h>
#include <Components/box_collider.h>
#include <Components/sphere_collider.h>
#include <Components/capsule_collider.h>
#include <Logs/log.h>

DEFINE_LOG_CATEGORY_WITH_COLOR(LogInspector, Loggers::Color::CYAN);

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::Resources;

namespace Mirage
{
	void Inspector::InitPanel()
	{
		LOG(logTemp, LogInspector, "Inspector Panel Initialized");
	}

	void Inspector::DrawPanel([[maybe_unused]] uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Inspector");

		std::shared_ptr<GameObject> selectedObj = m_Context->GetSelectedObject().lock();
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
						for (auto [base_id, base_meta] : type.base())
						{
							auto base_type = entt::resolve(base_id);
							if (base_meta && base_type)
							{
								for (auto [id_data, data] : base_type.data())
								{
									ImGui::TableNextRow();
									ImGui::TableNextColumn();

									ImGui::TextDisabled("[B] %s", data.name());

									ImGui::TableNextColumn();
									ImGui::PushItemWidth(-1);

									DrawPropertyDirect(base_type, data, (void*)comp.get());

									ImGui::PopItemWidth();
								}
							}
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
		ImGui::SetCursorPosX(width * 0.2f);
		if (ImGui::Button("Component", ImVec2(width * 0.6f, 30)))
		{
			ImGui::OpenPopup("AddComponentPopup");
		}
		ImGui::SetCursorPosX(width * 0.2f);
		if (ImGui::Button("Remove Component", ImVec2(width * 0.6f, 30)))
		{
			ImGui::OpenPopup("RemoveComponentPopup");
		}

		RenderAddComponentPopup(selectedObj);
		RenderRemoveComponentPopup(selectedObj);

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
				auto itemType = view.value_type();

				std::vector<entt::meta_any> tempItems;
				for (auto item : view) { tempItems.push_back(item); }

				int indexToModify = -1;
				int indexToDelete = -1;
				entt::meta_any newValue;
				bool shouldAdd = false;

				if (ImGui::Button("+ Add New Element")) { shouldAdd = true; }
				ImGui::Separator();

				for (int i = 0; i < (int)tempItems.size(); ++i)
				{
					ImGui::PushID(i);
					entt::meta_any& item = tempItems[i];

					if (ImGui::Button("X")) { indexToDelete = i; }
					ImGui::SameLine();

					if (itemType == entt::resolve<float>()) {
						float val = item.cast<float>();
						if (ImGui::DragFloat("##v", &val, 0.1f)) {
							item = val; indexToModify = i;
						}
					}
					else if (itemType == entt::resolve<std::string>()) {
						std::string str = item.cast<std::string>();
						char buffer[256]; memset(buffer, 0, 256); strncpy(buffer, str.c_str(), 255);
						if (ImGui::InputText("##v", buffer, 256)) {
							item = std::string(buffer); indexToModify = i;
						}
					}
					else if (itemType.is_pointer_like()) {
						void* rawPtr = nullptr;
						if (item) { if (auto* p = item.try_cast<void*>()) rawPtr = *p; }

						std::string ptrLabel = rawPtr ? "Action Assigned" : "None (Drop)";
						if (rawPtr) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.3f, 1.0f));
						ImGui::Button(ptrLabel.c_str(), ImVec2(-1, 0));
						if (rawPtr) ImGui::PopStyleColor();

						if (ImGui::BeginDragDropTarget()) {
							if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM")) {
								GameObject* droppedGO = *(GameObject**)payload->Data;
								for (auto& comp : droppedGO->GetComponents()) {
									if (entt::resolve(comp->GetType()) == itemType.remove_pointer() || dynamic_cast<AI::AIAction*>(comp.get())) {
										item = comp.get(); indexToModify = i; break;
									}
								}
							}
							else if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM")) {
								if (payload->Data && payload->DataSize > 0) {
									std::filesystem::path scriptPath((const char*)payload->Data);
									auto scriptType = entt::resolve(entt::hashed_string{ scriptPath.stem().string().c_str() });
									if (scriptType) {
										UtilityAgent* agent = instance.try_cast<UtilityAgent>();
										if (agent && agent->GetOwner()) {
											auto newComp = agent->GetOwner()->AddComponentByMeta(scriptType);
											if (newComp) {
												if (auto* actionPtr = dynamic_cast<AI::AIAction*>(newComp.get())) {
													item = actionPtr; indexToModify = i;
												}
											}
										}
									}
								}
							}
							ImGui::EndDragDropTarget();
						}
					}
					else if (!itemType.is_arithmetic()) {
						std::string typeName = itemType.info().name().data();
						size_t lastColon = typeName.find_last_of(':');
						if (lastColon != std::string::npos) typeName = typeName.substr(lastColon + 1);

						std::string nodeLabel = typeName + " " + std::to_string(i);
							
						if (ImGui::TreeNodeEx(nodeLabel.c_str(), ImGuiTreeNodeFlags_DefaultOpen)) {
							bool ruleChanged = false;
							for (auto [id, childData] : itemType.data()) {
								ImGui::TableNextRow(); ImGui::TableNextColumn();
								ImGui::TextUnformatted(childData.name());
								ImGui::TableNextColumn(); ImGui::PushItemWidth(-1);
								if (DrawNestedProperty(childData, item)) ruleChanged = true;
								ImGui::PopItemWidth();
							}
							if (ruleChanged) indexToModify = i;
							ImGui::TreePop();
						}
					}
					ImGui::PopID();
				}

				if (shouldAdd || indexToDelete != -1 || indexToModify != -1) {
					view.clear();
					if (shouldAdd) {
						if (itemType == entt::resolve<float>()) tempItems.push_back(0.0f);
						else if (itemType == entt::resolve<std::string>()) tempItems.push_back(std::string("New"));
						else if (itemType.is_pointer_like()) tempItems.push_back(nullptr);
						else tempItems.push_back(itemType.construct());
					}
					if (indexToDelete != -1) tempItems.erase(tempItems.begin() + indexToDelete);

					for (auto& itm : tempItems) view.insert(view.end(), itm);
					_data.set(instance, value); 
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

	bool Inspector::DrawNestedProperty(entt::meta_data _childData, entt::meta_any& _parentInstance)
	{
		bool changed = false;
		ImGui::PushID(_childData.name());

		entt::meta_any childValue = _childData.get(_parentInstance);
		if (!childValue) { ImGui::PopID(); return false; };

		auto type = childValue.type();
		const char* name = _childData.name();

		if (type == entt::resolve<float>()) {
			float v = childValue.cast<float>();
			if (ImGui::DragFloat(name, &v, 0.1f)) 
			{
				_childData.set(_parentInstance, v);
				changed = true;
			}
		}
		else if (type == entt::resolve<bool>()) {
			bool v = childValue.cast<bool>();
			if (ImGui::Checkbox(name, &v)) {
				_childData.set(_parentInstance, v);
				changed = true;
			}
		}
		else if (type.is_enum()) {
			std::string preview = "Unknown";
			for (auto [id, enumData] : type.data()) {
				if (enumData.get({}) == childValue) {
					preview = enumData.name();
					break;
				}
			}
				
			if (ImGui::BeginCombo("##enum", preview.c_str())) {
				for (auto [id, enumData] : type.data()) {
					bool isSelected = (enumData.get({}) == childValue);
					if (ImGui::Selectable(enumData.name(), isSelected)) {
						_childData.set(_parentInstance, enumData.get({}));
						changed = true;
					}
				}
				ImGui::EndCombo();
			}
		}
		else if (type == entt::resolve<std::string>())
		{
			std::string currentVal = childValue.cast<std::string>();
			if (name && std::strcmp(name, "Variable Name") == 0)
			{
				std::vector<std::string> options;
				auto sourceData = _parentInstance.type().data("Source"_hs);
				int sourceEnumVal = 0;
				if (sourceData) {
					auto srcAny = sourceData.get(_parentInstance);
					sourceEnumVal = (int)srcAny.cast<VariableSource>();
				}

				if (sourceEnumVal == 0) { 
					auto app = Application::Instance();
					if (app) {
						auto grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>();
						if (grid) options = grid->GetChannelsNames();
					}
				}

				const char* comboPreview = currentVal.empty() ? "Select Channel..." : currentVal.c_str();

				if (!options.empty()) {
					if (ImGui::BeginCombo("##varCombo", comboPreview)) {
						for (const auto& opt : options) 
						{
							if (ImGui::Selectable(opt.c_str(), currentVal == opt)) 
							{
								_childData.set(_parentInstance, opt);
								changed = true; 
							}
						}
						ImGui::EndCombo();
					}
				}
				else 
				{
					char buffer[256]; memset(buffer, 0, 256);
					strncpy(buffer, currentVal.c_str(), 255);
					if (ImGui::InputText("##varInput", buffer, 256)) 
					{
						_childData.set(_parentInstance, std::string(buffer));
						changed = true;
					}
				}
			}
			else
			{
				char buffer[256];
				memset(buffer, 0, 256);
				strncpy(buffer, currentVal.c_str(), 255);

				if (ImGui::InputText("##normalString", buffer, 256))
				{
					_childData.set(_parentInstance, std::string(buffer));
					changed = true;
				}
			}
		}
		else if (type == entt::resolve<AI::ResponsiveCurve>()) {
			AI::ResponsiveCurve* curve = childValue.try_cast<AI::ResponsiveCurve>();
			if (curve) {
				float samples[50];
				for (int i = 0; i < 50; i++) samples[i] = curve->Evaluate((float)i / 49.0f);
				ImGui::PlotLines("##Curve", samples, 50, 0, nullptr, 0.0f, 1.0f, ImVec2(0, 60));

				if (ImGui::BeginTable("CurveTable", 2)) {
					ImGui::TableSetupColumn("L", ImGuiTableColumnFlags_WidthFixed, 80.0f);
					ImGui::TableSetupColumn("F", ImGuiTableColumnFlags_WidthStretch);

					for (auto [id, curveData] : type.data()) {
						ImGui::TableNextRow();
						ImGui::TableNextColumn();
						ImGui::TextUnformatted(curveData.name());
						ImGui::TableNextColumn();
						ImGui::PushItemWidth(-1);

						if (DrawNestedProperty(curveData, childValue)) {
							changed = true;
						}

						ImGui::PopItemWidth();
					}
					ImGui::EndTable();
				}
				if (changed) {
					_childData.set(_parentInstance, childValue);
				}
			}
		}
		else if (type.is_sequence_container())
		{
			auto view = childValue.as_sequence_container();
			if (ImGui::TreeNode(name))
			{
				InfluenceGrid* grid = nullptr;
				auto app = Application::Instance();
				if (app) grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>().get();

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
					changed = true;
				}

				if (indexToDelete != -1) {
					auto it = view.begin();
					for (int i = 0; i < indexToDelete; ++i) ++it;
					view.erase(it);
					_childData.set(_parentInstance, childValue);
					changed = true;
				}

				if (shouldAdd) {
					view.insert(view.end(), 0.0f);
					_childData.set(_parentInstance, childValue);
					changed = true;
				}

				ImGui::TreePop();
			}
		}
		ImGui::PopID();
		return changed;
	}

	void Inspector::ResolveVec3(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		Math::Vector3 vec = _value.cast<Math::Vector3>();
		if (ImGui::DragFloat3(_label, &vec.X, 0.1f))
		{
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
		ImGui::PushID(_label);

		std::string currentVal = _value.cast<std::string>();
		bool drewCombo = false;

		if (std::strcmp(_label, "Type") == 0 || std::strcmp(_label, "Variable Name") == 0)
		{
			InfluenceGrid* grid = nullptr;
			if (auto* emitter = _instance.try_cast<InfluenceEmitter>()) {
				grid = emitter->GetTargetGrid();
			}

			if (!grid) {
				auto app = Application::Instance();
				if (app) grid = app->GetSceneGraph()->FindComponentInScene<InfluenceGrid>().get();
			}

			if (grid)
			{
				std::string comboLabel = std::string(_label) + "###Combo";
				if (ImGui::BeginCombo(comboLabel.c_str(), currentVal.c_str()))
				{
					for (const auto& channelName : grid->GetChannelsNames())
					{
						bool isSelected = (currentVal == channelName);
						if (ImGui::Selectable(channelName.c_str(), isSelected))
						{
							_data.set(_instance, channelName);
						}
					}
					ImGui::EndCombo();
				}
				drewCombo = true;
			}
		}

		if (!drewCombo)
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			std::strncpy(buffer, currentVal.c_str(), 255);

			std::string inputLabel = std::string(_label) + "###Input";
			if (ImGui::InputText(inputLabel.c_str(), buffer, sizeof(buffer)))
			{
				_data.set(_instance, std::string(buffer));
			}
		}

		ImGui::PopID();
	}

	void Inspector::ResolveInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		int val = _value.cast<int>();
		ImGui::SetCursorPosX(120);
		ImGui::SetNextItemWidth(120);
		if (ImGui::DragInt(_label, &val, 1))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveFloat(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		float val = _value.cast<float>();
		ImGui::SetCursorPosX(120);
		ImGui::SetNextItemWidth(120);
		if (ImGui::DragFloat(_label, &val, 0.1f))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveBool(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		bool val = _value.cast<bool>();
		ImGui::SetCursorPosX(120);
		if (ImGui::Checkbox(_label, &val))
		{
			_data.set(_instance, val);
		}
	}

	void Inspector::ResolveUnsignedInt(entt::meta_any _value, const char* _label, entt::meta_data _data, entt::meta_any& _instance)
	{
		unsigned int val = _value.cast<unsigned int>();
		ImGui::SetCursorPosX(120);
		ImGui::SetNextItemWidth(120);
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
            bool hasValue = false;
            std::string btnLabel = "None (Drop GameObject)";

            if (_value)
            {
                auto ptrType = _value.type();
                if (ptrType.is_pointer())
                {
                    entt::meta_any inner = *_value;                    
                    hasValue = (bool)inner;                          

                    if (hasValue)
                    {
                        btnLabel = std::string(ptrType.info().name());
                    }
                }
            }

            ImGui::TextUnformatted(_label);
            ImGui::SameLine();
            if (hasValue)
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.3f, 0.5f, 0.3f, 1.0f));
            ImGui::Button((btnLabel + "##" + _label).c_str(), ImVec2(-1, 0));
            if (hasValue)
                ImGui::PopStyleColor();

            if (ImGui::BeginDragDropTarget())
            {
                if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ITEM"))
                {
                    GameObject* droppedGO = *(GameObject**)payload->Data;
                    auto targetPointerType = _data.type();
                    auto baseType = targetPointerType.is_pointer() ? targetPointerType.remove_pointer() : targetPointerType;

                    for (auto& comp : droppedGO->GetComponents())
                    {
                        auto compMetaType = entt::resolve(comp->GetType());
                        if (compMetaType == baseType)
                        {
                            entt::meta_any typedPointer = targetPointerType.from_void(comp.get());
                            if (_data.set(_instance, typedPointer))
                                std::cout << "[DROP] SUCCESS: Linked to " << _label << std::endl;
                            else
                                std::cout << "[DROP] ERROR: Setter failed." << std::endl;
                            break;
                        }
                    }
                }
                ImGui::EndDragDropTarget();
            }

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

			bool hasCollider = _selectedObj->GetComponent<ICCollider>() != nullptr;
			if (ImGui::MenuItem("BoxCollider", nullptr, false, !hasCollider))
			{
				_selectedObj->AddComponent<BoxCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (hasCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Collider");

			if (ImGui::MenuItem("SphereCollider", nullptr, false, !hasCollider))
			{
				_selectedObj->AddComponent<SphereCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (hasCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Collider");

			if (ImGui::MenuItem("CapsuleCollider", nullptr, false, !hasCollider))
			{
				_selectedObj->AddComponent<CapsuleCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (hasCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Collider");

			bool hasRigidbody = _selectedObj->GetComponent<Rigidbody>() != nullptr;
			if (ImGui::MenuItem("Rigidbody", nullptr, false, !hasRigidbody))
			{
				std::shared_ptr<Rigidbody> rb = std::make_shared<Rigidbody>(_selectedObj.get());
				if (rb)
				{
					_selectedObj->AddComponent<Rigidbody>(rb);
				}
				ImGui::CloseCurrentPopup();
			}
			if (hasRigidbody && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject already has a Rigidbody");

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

	void Inspector::RenderRemoveComponentPopup(std::shared_ptr<MirageEngine::GameObject> _selectedObj)
	{
		if (ImGui::BeginPopup("RemoveComponentPopup"))
		{
			ImGui::TextDisabled("Remove Component");
			ImGui::Separator();

			bool hasMesh = _selectedObj->GetComponent<MeshRenderer>() != nullptr;
			if (ImGui::MenuItem("Mesh Renderer", nullptr, false, hasMesh))
			{
				_selectedObj->RemoveComponent<MeshRenderer>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasMesh && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a Mesh Renderer");

			bool hasCamera = _selectedObj->GetComponent<Camera>() != nullptr;
			if (ImGui::MenuItem("Camera", nullptr, false, hasCamera))
			{
				_selectedObj->RemoveComponent<Camera>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasCamera && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a Camera");

			bool hasBoxCollider = _selectedObj->GetComponent<BoxCollider>() != nullptr;
			if (ImGui::MenuItem("BoxCollider", nullptr, false, hasBoxCollider))
			{
				_selectedObj->RemoveComponent<BoxCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasBoxCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a BoxCollider");

			bool hasSphereCollider = _selectedObj->GetComponent<SphereCollider>() != nullptr;
			if (ImGui::MenuItem("SphereCollider", nullptr, false, hasSphereCollider))
			{
				_selectedObj->RemoveComponent<SphereCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasSphereCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a SphereCollider");

			bool hasCapsuleCollider = _selectedObj->GetComponent<CapsuleCollider>() != nullptr;
			if (ImGui::MenuItem("CapsuleCollider", nullptr, false, hasCapsuleCollider))
			{
				_selectedObj->RemoveComponent<CapsuleCollider>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasCapsuleCollider && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a CapsuleCollider");

			bool hasRigidbody = _selectedObj->GetComponent<Rigidbody>() != nullptr;
			if (ImGui::MenuItem("Rigidbody", nullptr, false, hasRigidbody))
			{
				_selectedObj->RemoveComponent<Rigidbody>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasRigidbody && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a Rigidbody");

			bool hasInfluenceGrid = _selectedObj->GetComponent<InfluenceGrid>() != nullptr;
			if (ImGui::MenuItem("Influence Grid", nullptr, false, hasInfluenceGrid))
			{
				_selectedObj->RemoveComponent<InfluenceGrid>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasInfluenceGrid && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have an Influence Grid");

			bool hasInfluenceEmitter = _selectedObj->GetComponent<InfluenceEmitter>() != nullptr;
			if (ImGui::MenuItem("Influence Emitter", nullptr, false, hasInfluenceEmitter))
			{
				_selectedObj->RemoveComponent<InfluenceEmitter>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasInfluenceEmitter && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have an Influence Emitter");

			bool hasInputMapping = _selectedObj->GetComponent<MirageEngine::Core::Input::InputMapping>() != nullptr;
			if (ImGui::MenuItem("Input Mapping", nullptr, false, hasInputMapping))
			{
				_selectedObj->RemoveComponent<MirageEngine::Core::Input::InputMapping>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasInputMapping && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have an InputMapping");

			bool hasNavAgent = _selectedObj->GetComponent<NavAgent>() != nullptr;
			if (ImGui::MenuItem("Nav Agent", nullptr, false, hasNavAgent))
			{
				_selectedObj->RemoveComponent<NavAgent>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasNavAgent && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a Nav Agent");

			bool hasUtilityAgent = _selectedObj->GetComponent<UtilityAgent>() != nullptr;
			if (ImGui::MenuItem("UtilityAgent", nullptr, false, hasUtilityAgent))
			{
				_selectedObj->RemoveComponent<UtilityAgent>();
				ImGui::CloseCurrentPopup();
			}
			if (!hasUtilityAgent && ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
				ImGui::SetTooltip("GameObject does not have a Utility Agent");

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
						ImGui::PushID((int)i);
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

						ImGui::PopID();
                    }
                }
                ImGui::Separator();
                ImGui::TextWrapped("Reason: %s", _agent->GetLastDecisionReason().c_str());
            }
        }
}