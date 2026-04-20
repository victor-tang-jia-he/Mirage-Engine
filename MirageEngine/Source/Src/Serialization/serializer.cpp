#include "Serialization/serializer.h"

#include <GameObject/game_object.h>
#include <Components/transform.h>

json MirageEngine::Serializer::SerializeGameObject(const GameObject& _gameObject)
{
    json j;

    j["id"] = _gameObject.GetID();
    j["components"] = json::object();

    return j;
}

json MirageEngine::Serializer::SerializeTransform(const Core::Components::Transform& _transform)
{
    return {
        {"Position", {{"X", _transform.GetPosition().X}, {"Y", _transform.GetPosition().Y}, {"Z", _transform.GetPosition().Z}}},
        {"Rotation", {{"W", _transform.GetRotation().W}, {"X", _transform.GetRotation().X}, {"Y", _transform.GetRotation().Y}, {"Z", _transform.GetRotation().Z}}},
		{"Scale",    {{"X", _transform.GetScale().X},    {"Y", _transform.GetScale().Y},    {"Z", _transform.GetScale().Z}}}
    };
}