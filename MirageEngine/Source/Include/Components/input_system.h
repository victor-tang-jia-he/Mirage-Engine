#pragma once
#include <GLFW/glfw3.h>

#include <utility>
#include <unordered_map>

#include <Events/event.h>

#include "component.h"

#include <Components/input_mapping.h>

namespace MirageEngine::Core
{
	namespace Input
	{
		struct InputBind;
		class InputMapping;

		class InputSystem
		{
		public:
		protected:
		private:

			static std::shared_ptr<InputMapping> _currentMap;
			static std::unordered_map<int, std::weak_ptr<InputBind>> m_heldButton;

			InputSystem& operator=(const InputSystem&) = delete;

		public:

			InputSystem() = delete;
			~InputSystem() = delete;
			InputSystem(const InputSystem&) = delete;

			static void Initialize(std::shared_ptr<InputMapping> map = nullptr);

			static void Shutdown();

			static void UpdateHeldInput();

			static void KeyCallback(GLFWwindow*, int key, int, int action, int);

			static CallbackInputInfo* GetKeyStats(int key);

			static InputMapping* GetCurrentMapping() { return _currentMap.get(); }

			static void SetCurrentMapping(std::shared_ptr<InputMapping> map);

			static void Add(int _key, std::shared_ptr<EventSystem::IEvent> _event);
		};
	} // namespace Input
} // namespace MirageEngine
