#pragma once

#include <unordered_map>

#include <Events/event.h>
#include "component.h"
#include <Logs/log_macro.h>

DECLARE_LOG_CATEGORY_EXTERN(logInputMapping)

namespace MirageEngine
{
	namespace Core
	{
		namespace Components
		{
			class Component; // forward declaration
		}

		namespace Input
		{
			class InputSystem;
			struct CallbackInputInfo
			{
				bool Pressed = false;
				bool Held = false;
				bool Released = false;
			};

			struct InputBind
			{
				EventSystem::EventDispatcher Dispatcher;
				CallbackInputInfo            InputInfo;
			};

			struct InputMapping : public Components::Component, public std::enable_shared_from_this<InputMapping>
			{
				std::unordered_map<int, std::shared_ptr<InputBind>> Map;

				InputMapping() = default;	

				~InputMapping() override;

				entt::id_type GetType() const override { return entt::type_id<InputMapping>().hash(); }

				template <typename... Args> void Add(int key, std::function<void(Args...)> func)
				{
					std::shared_ptr<EventSystem::Event<Args...>> event = std::make_shared<EventSystem::Event<Args...>>("InputSystem", func);

					if (Map.find(key) != Map.end()) { Map[key]->Dispatcher.Add(event); }
					else
					{
						Map[key] = std::make_shared<InputBind>();
						Map[key]->Dispatcher.Add(event);
					}
				}

				template <typename T, typename... Args> void Add(int key, T* instance, void (T::* method)(Args...))
				{
					auto func = [instance, method](Args... args) { (instance->*method)(args...); };

					Add<Args...>(key, func);
				}

				void Add(int key, std::shared_ptr<EventSystem::IEvent> event);

				void Remove(int key) { Map.erase(key); }

				void Clear() { Map.clear(); }

				void BindMapping();

				void Init() override {}

				void Update() override {}
			};
		} // namespace Input
	} // namespace Core
} // namespace MirageEngine
