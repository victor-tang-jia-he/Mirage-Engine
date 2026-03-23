#include "input_system.h"

namespace MirageEngine::Core::Input
{
	InputMapping* InputSystem::_currentMap = nullptr;
	std::unordered_map<int, InputBind*> InputSystem::m_heldButton;
}