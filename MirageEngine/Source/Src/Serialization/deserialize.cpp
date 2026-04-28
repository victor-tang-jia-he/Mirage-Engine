#include "Serialization/deserializer.h"
#include "SceneGraph/scene_graph.h"
#include "GameObject/game_object.h"
#include "Components/component.h"

DEFINE_LOG_CATEGORY(LogDeserializer)

using namespace MirageEngine;
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
    //std::shared_ptr<Core::Components::Component> _savedComponent = std::make_shared<Core::Components::Component>();

    std::string typeName = _jComponent["CompName"].get<std::string>();
    entt::hashed_string hashed{ typeName.c_str() };
    auto metaType = entt::resolve(hashed);

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
        // TODO : Add Components Meta Data Deserialization
    }

    _Owner->AddComponentFromMeta(compInstance);
}

json MirageEngine::Deserializer::ConvertJsonFileToJsonVar(std::string _fileName)
{
    std::ifstream saveFile(_fileName);

    json jSavedScene;

    if (saveFile)
        saveFile >> jSavedScene;

    return jSavedScene;
}