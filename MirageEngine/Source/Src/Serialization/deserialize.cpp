#include "Serialization/deserializer.h"
#include "SceneGraph/scene_graph.h"
#include "GameObject/game_object.h"
#include "Components/component.h"
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
    std::shared_ptr<SceneGraph> savedScene = std::make_shared<SceneGraph>();

    for (json jGameObject : _jSceneGraph["roots"])
    {
        DeserializeGameObject(jGameObject, nullptr, savedScene);
    }

    return savedScene;
}

void MirageEngine::Deserializer::DeserializeGameObject(json _jGameObject, std::shared_ptr<GameObject> _parent, std::shared_ptr<SceneGraph> _savedScene)
{
    std::shared_ptr<GameObject> savedGameObject = _savedScene->CreateGameObject(_jGameObject["ObjName"], _parent);

    savedGameObject->SetID(_jGameObject["id"]);
    //DeserializeComponent(_jGameObject["transform"], savedGameObject);
    
    for (json jComponent : _jGameObject["components"])
    {
        DeserializeComponent(jComponent, savedGameObject);
    }

    for (json jChild : _jGameObject["children"])
    {
        DeserializeGameObject(jChild, savedGameObject, _savedScene);
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

    entt::meta_any compInstance = metaType.construct();

    if (!compInstance) {
        LOG(logError, LogDeserializer, "Failed to construct Meta Instance for type : ", metaType.name());
        return;
    }

    for (auto [id, data] : metaType.data())
    {
        // Converts the Json data into corresponding Engine data and adds it to the Components Meta Instance
        DeserializeMetaData(metaType, data, compInstance, _jComponent);
    }

    _Owner->AddComponentFromMeta(compInstance);
}

void MirageEngine::Deserializer::DeserializeMetaData(entt::meta_type _type, entt::meta_data _data, entt::meta_any& _instance, json _jComponent)
{
    entt::meta_type actualType = _data.type();

    if (actualType == entt::resolve<std::string>())
    {
        std::string stringData = _jComponent[_data.name()].get<std::string>();
        _data.set(_instance, stringData);
        return;
    }
    else if (actualType == entt::resolve<Math::Vector3>())
    {
        Math::Vector3 vec3Data; // TODO : Vector3 Deserialization
        _data.set(_instance, vec3Data);
        return;
    }
    else if (actualType == entt::resolve<Math::Quaternion>())
    {
        Math::Quaternion quatData; // TODO : Quaternion Deserialization
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
    else if (actualType.is_pointer_like())
    {
        // TODO : Add Pointer (ressources + components) Deserialization
        return;
    }
    else
    {
        const char* label = _data.name();
        if (!label)
            label = "Property";

        LOG(logTemp, LogDeserializer, "Meta Data Mismatch : ", label);
    }
}

json MirageEngine::Deserializer::ConvertJsonFileToJsonVar(std::string _fileName)
{
    std::ifstream saveFile(_fileName);

    json jSavedScene;

    if (saveFile)
        saveFile >> jSavedScene;

    return jSavedScene;
}