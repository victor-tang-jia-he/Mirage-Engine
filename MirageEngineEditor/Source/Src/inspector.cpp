#include "inspector.h"
#include "UI_wrapper.h"
#include "matrix4.h"
#include "imgui.h"
#include "GameObject/game_object.h"
#include "SceneGraph/scene_graph.h"

using namespace MirageEngine::Core::Math;
using namespace MirageEngine;
using namespace entt::literals;

namespace Mirage
{
	void Inspector::InitPanel()
	{

	}

	void Inspector::DrawPanel([[maybe_unused]] uint32_t _textureID)
	{
		using namespace entt::literals;
		UIWrapper::BeginWindow("Inspector");

		GameObject* selectedObj = m_Context->GetSelectedObject();
		if (selectedObj == nullptr) {
			ImGui::TextDisabled("No object selected in Hierarchy");
			UIWrapper::EndWindow();
			return;
		}

		ImGui::Text("Entity: %s", selectedObj->GetName().c_str());
		ImGui::Separator();

		auto transType = entt::resolve("Transform"_hs);
		if (transType) {
			if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
				//std::cout << "\n[DEBUG] Transform address in GameObject: " << (void*)&selectedObj->Transform << "\n" << std::flush;
				for (auto [id, data] : transType.data()) {
					DrawPropertyDirect(transType, data, (void*)&selectedObj->Transform);
				}
			}
		}

		for (auto* comp : selectedObj->GetComponents()) {
			auto type = entt::resolve(comp->GetType());
			if (type) {
				if (ImGui::CollapsingHeader(type.info().name().data(), ImGuiTreeNodeFlags_DefaultOpen)) {
					ImGui::PushID(static_cast<int>(comp->GetType()));

				for (auto [id, data] : type.data()) {
						entt::meta_any compInstance = entt::forward_as_meta(*comp);
						DrawProperty(data, compInstance);
					}
					ImGui::PopID();
				}
			}
		}
		UIWrapper::EndWindow();
	}

	void Inspector::ShutdownPanel()
	{

	}

	void Inspector::DrawProperty(entt::meta_data _data, entt::meta_any _instance)
	{
		entt::meta_any value = _data.get(_instance);

		if (!value) {
			return;
		}
		
		auto actualType = value.type();

		const char* label = _data.name();
		if (!label) label = "Property";

		if (actualType == entt::resolve<Math::Vector3>())
		{
			Math::Vector3 vec = value.cast<Math::Vector3>();
			if (ImGui::DragFloat3(label, &vec.X, 0.1f))
			{
				std::cout << "\n===== SLIDER CHANGED =====\n";
				std::cout << "Property: " << label << "\n";
				std::cout << "New Value: (" << vec.X << ", " << vec.Y << ", " << vec.Z << ")\n";
				std::cout << "Calling setter...\n" << std::flush;
				
				bool success = _data.set(_instance, vec);
				
				if (!success) {
					std::cout << "FAIL: Setter returned false\n";
					std::cout << "===========================\n\n" << std::flush;
				}
				else {
					std::cout << "SUCCESS: Setter worked\n";
					std::cout << "===========================\n\n" << std::flush;
				}
			}
		}
		else if (actualType == entt::resolve<Math::Quaternion>())
		{
			auto quat = value.cast<Math::Quaternion>();
			auto euler = quat.ToEulerAngle();
			if (ImGui::DragFloat3(label, &euler.X, 0.1f))
			{
				Math::Quaternion newQuat(euler);
				std::cout << "\n===== SLIDER CHANGED =====\n";
				std::cout << "Property: " << label << " (Quaternion)\n";
				std::cout << "New Euler: (" << euler.X << ", " << euler.Y << ", " << euler.Z << ")\n";
				std::cout << "Calling setter...\n" << std::flush;
				
				bool success = _data.set(_instance, newQuat);
				
				if (!success) {
					std::cout << "FAIL: Setter returned false\n";
					std::cout << "===========================\n\n" << std::flush;
				}
				else {
					std::cout << "SUCCESS: Setter worked\n";
					std::cout << "===========================\n\n" << std::flush;
				}
			}
		}
	}

	void Inspector::DrawPropertyDirect(entt::meta_type _type, entt::meta_data _data, void* _instancePtr)
	{
		entt::meta_any instance = _type.from_void(_instancePtr);
		entt::meta_any value = _data.get(instance);

		if (!value) {
			return;
		}
		
		auto actualType = value.type();
		const char* label = _data.name();
		if (!label) label = "Property";

		if (actualType == entt::resolve<Math::Vector3>())
		{
			Math::Vector3 vec = value.cast<Math::Vector3>();
			if (ImGui::DragFloat3(label, &vec.X, 0.1f))
			{
				/*std::cout << "\n===== SLIDER CHANGED (DIRECT) =====\n";
				std::cout << "Property: " << label << "\n";
				std::cout << "New Value: (" << vec.X << ", " << vec.Y << ", " << vec.Z << ")\n";
				std::cout << "Instance pointer: " << _instancePtr << "\n";
				std::cout << "Calling setter...\n" << std::flush;*/
				
				bool success = _data.set(instance, vec);
				/*std::cout << "SUCCESS: " << success << "\n";
				std::cout << "===========================\n\n" << std::flush;*/
			}
		}
		else if (actualType == entt::resolve<Math::Quaternion>())
		{
			auto quat = value.cast<Math::Quaternion>();
			auto euler = quat.ToEulerAngle();
			if (ImGui::DragFloat3(label, &euler.X, 0.1f))
			{
				Math::Quaternion newQuat(euler);
				/*std::cout << "\n===== SLIDER CHANGED (DIRECT) =====\n";
				std::cout << "Property: " << label << " (Quaternion)\n";
				std::cout << "New Euler: (" << euler.X << ", " << euler.Y << ", " << euler.Z << ")\n";
				std::cout << "Instance pointer: " << _instancePtr << "\n";
				std::cout << "Calling setter...\n" << std::flush;*/
				
				bool success = _data.set(instance, newQuat);
				/*std::cout << "SUCCESS: " << success << "\n";
				std::cout << "===========================\n\n" << std::flush;*/
			}
		}
	}
}