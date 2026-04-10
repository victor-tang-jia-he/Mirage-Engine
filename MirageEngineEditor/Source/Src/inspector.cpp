#include "inspector.h"
#include "UI_wrapper.h"
#include "matrix4.h"
#include "imgui.h"
#include "GameObject/game_object.h"
#include "SceneGraph/scene_graph.h"
#include <cstring>

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;

namespace Mirage
{
	void Inspector::InitPanel() {}

	void Inspector::DrawPanel([[maybe_unused]] uint32_t _textureID)
	{
		UIWrapper::BeginWindow("Inspector");

		GameObject* selectedObj = m_Context->GetSelectedObject();
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
			auto goType = entt::resolve("GameObject"_hs);
			if (goType) {
				// Position and Rotation from GameObject (physics-aware)
				for (const char* propName : {"Position", "Rotation"}) 
				{
					auto data = goType.data(entt::hashed_string{propName});
					if (data) 
					{
						DrawPropertyDirect(goType, data, (void*)selectedObj);
					}
				}
			}

			// Scale from Transform
			auto transType = entt::resolve("Transform"_hs);
			if (transType) 
			{
				auto data = transType.data(entt::hashed_string{"Scale"});
				if (data) 
				{
					DrawPropertyDirect(transType, data, (void*)&selectedObj->Transform);
				}
			}
		}

		// Components
		for (auto* comp : selectedObj->GetComponents()) 
		{
			auto type = entt::resolve(comp->GetType());
			if (type) {
				if (ImGui::CollapsingHeader(type.info().name().data(), ImGuiTreeNodeFlags_DefaultOpen)) 
				{
					ImGui::PushID(static_cast<int>(comp->GetType()));
					for (auto [id, data] : type.data()) 
					{
						DrawPropertyDirect(type, data, (void*)comp);
					}
					ImGui::PopID();
				}
			}
		}
		UIWrapper::EndWindow();
	}

	void Inspector::ShutdownPanel() {}

	//void Inspector::DrawProperty(entt::meta_data _data, entt::meta_any _instance)
	//{
	//	entt::meta_any value = _data.get(_instance);
	//	if (!value) return;
	//	
	//	auto actualType = value.type();
	//	const char* label = _data.name();
	//	if (!label) label = "Property";

	//	if (actualType == entt::resolve<Math::Vector3>()) {
	//		Math::Vector3 vec = value.cast<Math::Vector3>();
	//		if (ImGui::DragFloat3(label, &vec.X, 0.1f)) {
	//			_data.set(_instance, vec);
	//		}
	//	}
	//	else if (actualType == entt::resolve<Math::Quaternion>()) {
	//		auto quat = value.cast<Math::Quaternion>();
	//		auto euler = quat.ToEulerAngle();
	//		if (ImGui::DragFloat3(label, &euler.X, 0.1f)) {
	//			_data.set(_instance, Math::Quaternion(euler));
	//		}
	//	}
	//}

	void Inspector::DrawPropertyDirect(entt::meta_type _type, entt::meta_data _data, void* _instancePtr)
	{
		entt::meta_any instance = _type.from_void(_instancePtr);
		entt::meta_any value = _data.get(instance);

		if (!value) return;
		
		auto actualType = value.type();
		const char* label = _data.name();
		if (!label) label = "Property";

		if (actualType == entt::resolve<Math::Vector3>()) 
		{
			Math::Vector3 vec = value.cast<Math::Vector3>();
			if (ImGui::DragFloat3(label, &vec.X, 0.1f)) {
				_data.set(instance, vec);
			}
		}
		else if (actualType == entt::resolve<Math::Quaternion>()) 
		{
			auto quat = value.cast<Math::Quaternion>();
			auto euler = quat.ToEulerAngle();
			if (ImGui::DragFloat3(label, &euler.X, 0.1f)) {
				_data.set(instance, Math::Quaternion(euler));
			}
		}
	}
}
