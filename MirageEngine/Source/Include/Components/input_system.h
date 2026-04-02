#pragma once
#include <GLFW/glfw3.h>

#include <utility>
#include <unordered_map>

#include <Events/event.h>

#include "component.h"

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
			struct CallbackInputInfo
			{
				bool Pressed = false;
				bool Held = false;
				bool Released = false;
			};

			struct InputBind
			{
				EventSystem::EventDispatcher Dispatcher;
				CallbackInputInfo InputInfo;
			};

			struct InputMapping// : public Components::Component
			{
				std::unordered_map<int, InputBind> Map;
				void Add(int key, EventSystem::IEvent* event)
				{
					if (Map.find(key) != Map.end())
					{
						Map[key].Dispatcher.Add(event);
					}
					else
					{
						Map[key] = InputBind{};
						Map[key].Dispatcher.Add(event);
					}
				}
				InputMapping() = default;
			};

			class InputSystem
			{
			public:
			protected:
			private:
				static InputMapping* _currentMap;
				static std::unordered_map<int, InputBind*> m_heldButton;

				InputSystem& operator=(const InputSystem&) = delete;

			public:
				InputSystem() = delete;
				~InputSystem() = delete;
				InputSystem(const InputSystem&) = delete;

				static void Initialize(InputMapping* map = nullptr)
				{
					if (_currentMap != nullptr)
					{
						delete _currentMap;
					}
					_currentMap = map;
				}

				static void Shutdown()
				{
					delete _currentMap;
					_currentMap = nullptr;
					m_heldButton.clear();
				}

				static void UpdateHeldInput()
				{
					for (auto& kv : m_heldButton)
					{
						kv.second->Dispatcher.Invoke("InputSystem", kv.second->InputInfo);
					}
				}

				static void KeyCallback(GLFWwindow*, int key, int, int action, int)
				{
					if (_currentMap == nullptr) return;

					auto input = _currentMap->Map.find(key);
					if (input != _currentMap->Map.end())
					{
						InputBind& inputBind = input->second;
						CallbackInputInfo& inputInfo = inputBind.InputInfo;

						if (action == GLFW_PRESS)
						{
							inputInfo.Pressed = true;
							inputInfo.Held = true;
							inputBind.Dispatcher.Invoke("InputSystem", inputInfo);
							inputInfo.Pressed = false;
							m_heldButton[key] = &inputBind;
						}
						else if (action == GLFW_RELEASE)
						{
							inputInfo.Released = true;
							inputInfo.Held = false;
							inputBind.Dispatcher.Invoke("InputSystem", inputInfo);
							inputInfo.Released = false;
							m_heldButton.erase(key);
						}
					}
				}

				static CallbackInputInfo* GetKeyStats(int key)
				{
					if (_currentMap == nullptr) return nullptr;

					auto event = _currentMap->Map.find(key);
					if (event != _currentMap->Map.end())
					{
						return &event->second.InputInfo;
					}
					return nullptr;
				}

				static InputMapping* GetCurrentMapping()
				{
					return _currentMap;
				}

				static void SetCurrentMapping(InputMapping* map)
				{
					//if (_currentMap != nullptr && _currentMap != map)
					//{
					//	delete _currentMap;
					//}
					_currentMap = map;
				}

				static void Add(int _key, EventSystem::IEvent* _event)
				{
					if (_currentMap == nullptr) return;

					auto input = _currentMap->Map.find(_key);
					if (input != _currentMap->Map.end())
					{
						InputBind& inputBind = input->second;
						inputBind.Dispatcher.Add(_event);
					}
					else
					{
						// key missing, not bind in the map
					}
				}
			};
		}
	}
}