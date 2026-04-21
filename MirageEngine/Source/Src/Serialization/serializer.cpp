#include "Serialization/serializer.h"

#include <SceneGraph/scene_graph.h>
#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>
#include <vector3.h>
#include <quaternion.h>

DEFINE_LOG_CATEGORY(LogSerializer)

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace entt::literals;

json Serializer::SerializeScene(SceneGraph _sceneGraph)
{
    LOG(logInfo, LogSerializer, "Saving Scene...");

    json jScene;

    // Serialize all scene roots
    for (std::shared_ptr<GameObject> root : _sceneGraph.GetRootObjects())
    {
        jScene["children"].push_back(SerializeGameObject(root, nullptr));
    }

    LOG(logInfo, LogSerializer, "Scene saved successfully");

    return jScene;
}

json Serializer::SerializeGameObject(std::shared_ptr<GameObject> _gameObject, json _jParent)
{
    json jGameObject;

    jGameObject["name"] = _gameObject->GetName();
    jGameObject["id"] = _gameObject->GetID();
    jGameObject["components"] = json::array();
    jGameObject["children"] = json::array();
    jGameObject["parent"] = _jParent;
    jGameObject["transform"] = SerializeTransform(_gameObject->Transform);

    // Serialize all gameobjects components
    for (std::shared_ptr<Components::Component> component : _gameObject->GetComponents()) {
        jGameObject["components"].push_back(SerializeComponent(component));
    }

    // Recursivly serialize all gameobjects children
    for (std::shared_ptr<GameObject> child : _gameObject->GetChildren()) {
        jGameObject["children"].push_back(SerializeGameObject(child, jGameObject));
    }

    return jGameObject;
}

json MirageEngine::Serializer::SerializeComponent(std::shared_ptr<Components::Component> _component, json _Owner)
{
    json jComponent;

    jComponent["componentName"] = _component->GetName();
    jComponent["owner"] = _Owner;

    // Serializing other Component specifice variables
    auto type = entt::resolve(_component->GetType());
    if (type)
    {
        for (auto [id, data] : type.data())
        {
            jComponent[data.name()].push_back(SerializeMetaData(type, data, (void*)_component.get()));
        }
    }

    return jComponent;
}

json Serializer::SerializeTransform(Components::Transform _transform)
{
    json jTransform;

    jTransform["position"] = SerializeVector3(_transform.GetPosition());
    jTransform["rotation"] = SerializeQuaternion(_transform.GetRotation());
    jTransform["scale"] = SerializeVector3(_transform.GetScale());

    return jTransform;
}

json MirageEngine::Serializer::SerializeMetaData(entt::meta_type _type, entt::meta_data _data, void* _instancePtr)
{
    json jMetaData;

    entt::meta_any instance = _type.from_void(_instancePtr);
    entt::meta_any value = _data.get(instance);

    if (!value)
        return jMetaData;

    auto actualType = value.type();

    const char* label = _data.name(); // The string name we gave in .data("Name")
    if (!label) label = "Property";

    if (actualType == entt::resolve<std::string>())
    {
        if (std::string* stringData = value.try_cast<std::string>())
        {
            jMetaData[label] = *stringData;
        }
    }
    else if (actualType == entt::resolve<Math::Vector3>())
    {
        if (Math::Vector3* vector3Data = value.try_cast<Math::Vector3>())
        {
            jMetaData[label] = SerializeVector3(*vector3Data);
        }
    }
    else if (actualType == entt::resolve<Math::Quaternion>())
    {
        if (Math::Quaternion* quaternionData = value.try_cast<Math::Quaternion>())
        {
            jMetaData[label] = SerializeQuaternion(*quaternionData);
        }
    }
    else if (actualType == entt::resolve<float>())
    {
        if (float* floatData = value.try_cast<float>())
        {
            jMetaData[label] = *floatData;
        }
    }
    else if (actualType == entt::resolve<int>())
    {
        if (int* intData = value.try_cast<int>())
        {
            jMetaData[label] = *intData;
        }
    }
    else if (actualType == entt::resolve<unsigned int>())
    {
        if (unsigned int* unsignedIntData = value.try_cast<unsigned int>())
        {
            jMetaData[label] = *unsignedIntData;
        }
    }
    else if (actualType == entt::resolve<bool>())
    {
        if (bool* boolData = value.try_cast<bool>())
        {
            jMetaData[label] = *boolData;
        }
    }
    // Add Pointers

    return jMetaData;
}

json MirageEngine::Serializer::SerializeVector3(Core::Math::Vector3 _vector3)
{
    json jVector3;

    jVector3["x"] = _vector3.X;
    jVector3["y"] = _vector3.Y;
    jVector3["z"] = _vector3.Z;

    return jVector3;
}

json MirageEngine::Serializer::SerializeQuaternion(Core::Math::Quaternion _quaternion)
{
    json jQuaternion;

    jQuaternion["w"] = _quaternion.W;
    jQuaternion["x"] = _quaternion.X;
    jQuaternion["y"] = _quaternion.Y;
    jQuaternion["z"] = _quaternion.Z;

    return jQuaternion;
}
