#pragma once
#include "../../../../Libraries/GLFW/include/GLFW/glfw3.h"
#include "event.h"
#include <utility>
#include <unordered_map>

namespace MirageEngine
{
	namespace Core
	{
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
				MirageEngine::Core::EventSystem::EventDispatcher dispatcher;
				CallbackInputInfo inputInfo;
			};

			struct InputMapping
			{
				std::unordered_map<int, InputBind> Map;
			};

			class InputSystem
			{
			private:
				static InputMapping* _currentMap;
				static std::unordered_map<int, InputBind*> m_heldButton;

				InputSystem() = delete;
				~InputSystem() = delete;
				InputSystem(const InputSystem&) = delete;
				InputSystem& operator=(const InputSystem&) = delete;

			public:
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
						kv.second->dispatcher.Invoke("InputSystem", kv.second->inputInfo);
					}
				}

				static void KeyCallback(GLFWwindow*, int key, int, int action, int)
				{
					if (_currentMap == nullptr) return;

					auto input = _currentMap->Map.find(key);
					if (input != _currentMap->Map.end())
					{
						InputBind& inputBind = input->second;
						CallbackInputInfo& inputInfo = inputBind.inputInfo;

						if (action == GLFW_PRESS)
						{
							inputInfo.Pressed = true;
							inputInfo.Held = true;
							inputBind.dispatcher.Invoke("InputSystem", inputInfo);
							inputInfo.Pressed = false;
							m_heldButton[key] = &inputBind;
						}
						else if (action == GLFW_RELEASE)
						{
							inputInfo.Released = true;
							inputInfo.Held = false;
							inputBind.dispatcher.Invoke("InputSystem", inputInfo);
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
						return &event->second.inputInfo;
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

				static void Add(int _key, MirageEngine::Core::EventSystem::IEvent* _event)
				{
					if (_currentMap == nullptr) return;

					auto input = _currentMap->Map.find(_key);
					if (input != _currentMap->Map.end())
					{
						InputBind& inputBind = input->second;
						inputBind.dispatcher.Add(_event);
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