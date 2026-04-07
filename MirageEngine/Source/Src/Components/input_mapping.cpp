#include <Components/input_mapping.h>
#include <Components/input_system.h>

namespace MirageEngine::Core::Input
{
    void MirageEngine::Core::Input::InputMapping::BindMapping() { InputSystem::SetCurrentMapping(this); }
} // namespace MirageEngine::Core::Input
