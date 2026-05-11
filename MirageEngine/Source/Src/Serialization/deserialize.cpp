#include "Serialization/deserializer.h"
#include "SceneGraph/scene_graph.h"
#include "GameObject/game_object.h"
#include "Components/component.h"
#include "Components/camera.h"
#include "Components/rigidbody.h"
#include "vector3.h"
#include "quaternion.h"

DEFINE_LOG_CATEGORY(LogDeserializer)

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace entt::literals;

std::shared_ptr<SceneGraph> MirageEngine::Deserializer::LoadSceneFromJson(json _jSavedScene)
{
    return DeserializeScene(_jSavedScene);
}

std::shared_ptr<SceneGraph> MirageEngine::Deserializer::LoadSceneFromFile(std::string _saveFileName)
{
    json jSavedScene = ConvertJsonFileToJsonVar(_saveFileName);

    return LoadSceneFromJson(jSavedScene);
}

std::shared_ptr<SceneGraph> MirageEngine::Deserializer::DeserializeScene(json _jSceneGraph)
{
    LOG(logInfo, LogDeserializer, "Loading Scene...");

    std::shared_ptr<SceneGraph> savedScene = std::make_shared<SceneGraph>();

    for (json jGameObject : _jSceneGraph["Roots"])
    {
        DeserializeGameObject(jGameObject, nullptr, savedScene);
    }

    LOG(logInfo, LogDeserializer, "Scene loaded successfully");

    return savedScene;
}

void MirageEngine::Deserializer::DeserializeGameObject(json _jGameObject, std::shared_ptr<GameObject> _parent, std::shared_ptr<SceneGraph> _savedScene)
{
    std::shared_ptr<GameObject> savedGameObject = std::make_shared<GameObject>(_parent, _jGameObject["ObjName"].get<std::string>());

    savedGameObject->SetID(_jGameObject["ID"]);
    
    for (json jComponent : _jGameObject["Components"])
    {
        DeserializeComponent(jComponent, savedGameObject);
    }

    for (json jChild : _jGameObject["Children"])
    {
        DeserializeGameObject(jChild, savedGameObject, _savedScene);
    }

    _savedScene->AddGameObject(savedGameObject);

    DeserializeTransform(_jGameObject["Transform"], savedGameObject);

    auto rigidbody = savedGameObject->GetComponent<Core::Components::Rigidbody>();
    if (rigidbody)
    {
        rigidbody->OnTransformChanged(savedGameObject->Transform);
    }
}

void MirageEngine::Deserializer::DeserializeComponent(json _jComponent, std::shared_ptr<GameObject> _Owner)
{
    std::string typeName = _jComponent["CompName"].get<std::string>();
    entt::hashed_string hashed{ typeName.c_str() };
    entt::meta_type metaType = entt::resolve(hashed);

    if (!metaType)
    {
        LOG(logError, LogDeserializer, "Saved Component not recognised!");
        return;
    }

    entt::meta_func compFunc = metaType.func("AttachNew"_hs);
    if (!compFunc)
    {
        LOG(logError, LogDeserializer, "No Meta AttachNew function registered for type : ", metaType.name());
        return;
    }

    entt::meta_any result = compFunc.invoke({}, _Owner);
    if (!result)
    {
        LOG(logError, LogDeserializer, "Meta Invoke failed for type : ", metaType.name());
        return;
    }

    entt::meta_any compInstance = metaType.from_void(_Owner->GetLastAddedComponent().get());
    if (!compInstance) {
        LOG(logError, LogDeserializer, "Failed to construct Meta Instance for type : ", metaType.name());
        return;
    }

    for (auto [id, data] : metaType.data())
    {
        // Converts the Json data into corresponding Engine data and adds it to the Components Meta Instance
        DeserializeMetaData(metaType, data, compInstance, _jComponent);
    }
}

entt::meta_any MirageEngine::Deserializer::DeserializeTransform(json _jComponent, std::shared_ptr<GameObject> _Owner)
{
    std::string typeName = typeid(Core::Components::Transform).name();
    entt::hashed_string hashed{ typeName.c_str() };
    entt::meta_type metaType = entt::resolve(hashed);
    entt::meta_any compInstance = metaType.construct();

    if (!metaType)
    {
        LOG(logError, LogDeserializer, "Saved Transform not recognised!");
        return compInstance;
    }
    if (!compInstance) 
    {
        LOG(logError, LogDeserializer, "Failed to construct Transform Meta Instance!");
        return compInstance;
    }

    for (auto [id, data] : metaType.data())
    {
        // Converts the Json data into corresponding Engine data and adds it to the Components Meta Instance
        DeserializeMetaData(metaType, data, compInstance, _jComponent);
    }

    if (_Owner)
    {
        _Owner->AddComponentFromMeta(compInstance);
    }

    return compInstance;
}

void MirageEngine::Deserializer::DeserializeMetaData(entt::meta_type _type, entt::meta_data _data, entt::meta_any& _instance, json _jComponent)
{
    entt::meta_any value = _data.get(_instance);

    if (!value)
    {
        LOG(logError, LogDeserializer, "Meta Data value empty!");
        return;
    }

    entt::meta_type actualType = value.type();

    if (actualType == entt::resolve<std::string>())
    {
        std::string stringData = _jComponent[_data.name()].get<std::string>();
        _data.set(_instance, stringData);
        return;
    }
    else if (actualType == entt::resolve<Math::Vector3>())
    {
        Math::Vector3 vec3Data = DeserializeVector3(_jComponent[_data.name()]);
        _data.set(_instance, vec3Data);
        return;
    }
    else if (actualType == entt::resolve<Math::Quaternion>())
    {
        Math::Quaternion quatData = DeserializeQuaternion(_jComponent[_data.name()]);
        _data.set(_instance, quatData);
        return;
    }
    else if (actualType == entt::resolve<float>())
    {
        float floatData = _jComponent[_data.name()].get<float>();
        _data.set(_instance, floatData);
        return;
    }
    else if (actualType == entt::resolve<int>())
    {
        int intData = _jComponent[_data.name()].get<int>();
        _data.set(_instance, intData);
        return;
    }
    else if (actualType == entt::resolve<unsigned int>())
    {
        unsigned int unsignedIntData = _jComponent[_data.name()].get<unsigned int>();
        _data.set(_instance, unsignedIntData);
        return;
    }
    else if (actualType == entt::resolve<bool>())
    {
        bool booleanData = _jComponent[_data.name()].get<bool>();
        _data.set(_instance, booleanData);
        return;
    }
    else if (actualType.is_enum())
    {
        int enumData = _jComponent[_data.name()].get<int>();
        _data.set(_instance, enumData);
        return;
    }
    else if (actualType == entt::resolve<Core::Components::Transform>())
    {
        json transformData = _jComponent[_data.name()];
        entt::meta_any transformInstance = DeserializeTransform(transformData, nullptr);
        Core::Components::Transform* savedTransform = transformInstance.try_cast<Core::Components::Transform>();
        if (savedTransform)
        {
            _data.set(_instance, *savedTransform);
            return;
        }
    }
    else if (actualType.is_pointer_like())
    {
        json jPointer = _jComponent[_data.name()];

        if (!jPointer.is_null())
        {
            std::string typeName = jPointer["Type"].get<std::string>();
            entt::hashed_string hashed{ typeName.c_str() };
            entt::meta_type metaType = entt::resolve(hashed);

            entt::meta_func getPointer = metaType.func("Get"_hs);
            _data.set(_instance, getPointer.invoke({}, jPointer["Name"].get<std::string>()));
            return;
        }
    }

    const char* label = _data.name();
    if (!label)
        label = "Property";

    LOG(logWarning, LogDeserializer, "Meta Data Mismatch : ", label);
}

Core::Math::Vector3 MirageEngine::Deserializer::DeserializeVector3(json _jVector3)
{
    Core::Math::Vector3 savedVector3;

    savedVector3.X = _jVector3["X"].get<float>();
    savedVector3.Y = _jVector3["Y"].get<float>();
    savedVector3.Z = _jVector3["Z"].get<float>();

    return savedVector3;
}

Core::Math::Quaternion MirageEngine::Deserializer::DeserializeQuaternion(json _jQuaternion)
{
    Core::Math::Quaternion savedQuaternion;

    savedQuaternion.W = _jQuaternion["W"].get<float>();
    savedQuaternion.X = _jQuaternion["X"].get<float>();
    savedQuaternion.Y = _jQuaternion["Y"].get<float>();
    savedQuaternion.Z = _jQuaternion["Z"].get<float>();

    return savedQuaternion;
}

json MirageEngine::Deserializer::ConvertJsonFileToJsonVar(std::string _fileName)
{
    std::ifstream saveFile(_fileName);

    json jSavedScene;

    if (saveFile)
        saveFile >> jSavedScene;

    return jSavedScene;
}