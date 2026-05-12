#include "Serialization/serializer.h"

#include <SceneGraph/scene_graph.h>

#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>

#include <Physics/common_enum.h>

#include <Resources/mesh.h>
#include <Resources/material.h>

#include <vector3.h>
#include <quaternion.h>
#include <Logs/log.h>

#include <fstream>

#include <fstream>

DEFINE_LOG_CATEGORY(LogSerializer)

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace entt::literals;

void MirageEngine::Serializer::SaveScene(SceneGraph _sceneGraph)
{
    LOG(logInfo, LogSerializer, "Saving Scene...");
    json jSceneGraph = SerializeScene(_sceneGraph);

    if (ConvertJsonVarToJsonFile(jSceneGraph))
        LOG(logInfo, LogSerializer, "Scene saved successfully");
    else
        LOG(logError, LogSerializer, "Failed to save scene");
}

json Serializer::SerializeScene(SceneGraph _sceneGraph)
{
    json jScene;

    // Serialize all GameObjects recursively from Roots
    for (std::shared_ptr<GameObject> root : _sceneGraph.GetRootObjects())
    {
        jScene["Roots"].emplace_back(SerializeGameObject(root, nullptr));
    }

    return jScene;
}

json Serializer::SerializeGameObject(std::shared_ptr<GameObject> _gameObject, json _jParent)
{
    json jGameObject;

    jGameObject["ObjName"] = _gameObject->GetName();
    jGameObject["ID"] = _gameObject->GetID();
    jGameObject["Transform"] = SerializeTransform(_gameObject->Transform);
    jGameObject["Components"] = json::array();
    jGameObject["Children"] = json::array();

    // Recursivly serialize all gameobjects children
    for (std::shared_ptr<GameObject> child : _gameObject->GetChildren()) {
        jGameObject["Children"].emplace_back(SerializeGameObject(child, jGameObject));
    }

    // Serialize all gameobjects components
    for (std::shared_ptr<Components::Component> component : _gameObject->GetComponents()) {
        jGameObject["Components"].emplace_back(SerializeComponent(component));
    }

    LOG(logTemp, LogSerializer, "Serialized GameObject : ", _gameObject->GetName().c_str());

    return jGameObject;
}

json MirageEngine::Serializer::SerializeComponent(std::shared_ptr<Components::Component> _component)
{
    json jComponent;

    jComponent["CompName"] = _component->GetName().c_str();

    // Serializing other Component specifice variables
    entt::meta_type type = entt::resolve(_component->GetType());
    if (type)
    {
        for (auto [id, data] : type.data())
        {
            jComponent[data.name()] = (SerializeMetaData(type, data, (void*)_component.get()));
        }
    }

    return jComponent;
}

json Serializer::SerializeTransform(Components::Transform _transform)
{
    json jTransform;

    entt::hashed_string hashed{ typeid(Components::Transform).name() };
    entt::meta_type type = entt::resolve(hashed);
    if (type)
    {
        for (auto [id, data] : type.data())
        {
            jTransform[data.name()] = (SerializeMetaData(type, data, (void*)&_transform));
        }
    }

    return jTransform;
}

json MirageEngine::Serializer::SerializeMetaData(entt::meta_type _type, entt::meta_data _data, void* _instancePtr)
{
    json jMetaData;

    entt::meta_any instance = _type.from_void(_instancePtr);
    entt::meta_any value = _data.get(instance);

    if (!value)
        return jMetaData;

    entt::meta_type actualType = value.type();

    const char* label = _data.name();
    if (!label) 
        label = "Property";

    if (actualType == entt::resolve<std::string>())
    {
        if (std::string* stringData = value.try_cast<std::string>())
        {
            jMetaData = *stringData;
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<Math::Vector3>())
    {
        if (Math::Vector3* vector3Data = value.try_cast<Math::Vector3>())
        {
            jMetaData = SerializeVector3(*vector3Data);
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<Math::Quaternion>())
    {
        if (Math::Quaternion* quaternionData = value.try_cast<Math::Quaternion>())
        {
            jMetaData = SerializeQuaternion(*quaternionData);
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<float>())
    {
        if (float* floatData = value.try_cast<float>())
        {
            jMetaData = *floatData;
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<int>())
    {
        if (int* intData = value.try_cast<int>())
        {
            jMetaData = *intData;
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<unsigned int>())
    {
        if (unsigned int* unsignedIntData = value.try_cast<unsigned int>())
        {
            jMetaData = *unsignedIntData;
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<bool>())
    {
        if (bool* boolData = value.try_cast<bool>())
        {
            jMetaData = *boolData;
            return jMetaData;
        }
    }
    else if (actualType.is_enum())
    {
        if (value.allow_cast<int>())
        {
            jMetaData = value.cast<int>();
            return jMetaData;
        }
    }
    else if (actualType == entt::resolve<Core::Components::Transform>())
    {
        if (Core::Components::Transform* transformData = value.try_cast<Core::Components::Transform>())
        {
            jMetaData = SerializeTransform(*transformData);
            return jMetaData;
        }
    }
    else if (actualType.is_pointer_like())
    {
        if (entt::meta_any ref = *value; ref)
        {
            json jPointer;

            if (Core::Components::Component* component = ref.try_cast<Core::Components::Component>())
            {
                jPointer["Type"] = ref.type().name();
                jPointer["Name"] = component->GetName();
            }
            else if (Core::Resources::IResource* resource = ref.try_cast<Core::Resources::IResource>())
            {
                jPointer["Type"] = ref.type().name();
                jPointer["Name"] = resource->GetName();
            }

            jMetaData = jPointer;
            return jMetaData;
        }
    }

    LOG(logWarning, LogSerializer, "Ignored Meta Data : ", label);

    return jMetaData;
}

json MirageEngine::Serializer::SerializeVector3(Core::Math::Vector3 _vector3)
{
    json jVector3;

    jVector3["X"] = _vector3.X;
    jVector3["Y"] = _vector3.Y;
    jVector3["Z"] = _vector3.Z;

    return jVector3;
}

json MirageEngine::Serializer::SerializeQuaternion(Core::Math::Quaternion _quaternion)
{
    json jQuaternion;

    jQuaternion["W"] = _quaternion.W;
    jQuaternion["X"] = _quaternion.X;
    jQuaternion["Y"] = _quaternion.Y;
    jQuaternion["Z"] = _quaternion.Z;

    return jQuaternion;
}

bool MirageEngine::Serializer::ConvertJsonVarToJsonFile(json _jTarget)
{
    std::ofstream file("scene.json");
    file.clear();
    file << _jTarget.dump(4);
    file.flush();
    return true;
}
