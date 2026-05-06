#include <Components/camera.h>
#include <GameObject/game_object.h>
#include <Input/input_system.h>
#include <Input/input_mapping.h>
#include <Events/event.h>
#include <iostream>
#include <algorithm>
#include <time_manager.h>
#include <GLFW/glfw3.h>

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace MirageEngine::Core::Components;
using namespace MirageEngine::Core::EventSystem;
using namespace MirageEngine::Core::Input;
using namespace MirageEngine::Core::Math;

Camera::Camera(unsigned int _width, unsigned int _height, float _fov, float _near, float _far, const Vector3 _position)
    : ICamera(_width, _height, _fov, _near, _far, _position)
{}

void MirageEngine::Core::Components::Camera::AttachMeta(std::shared_ptr<GameObject> _Owner)
{
    std::shared_ptr<Camera> compPtr = std::make_shared<Camera>(*this);
    _Owner->AddComponent<Camera>(compPtr);
}
