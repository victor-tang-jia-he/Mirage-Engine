#include "Serialization/serializer.h"

#include <SceneGraph/scene_graph.h>
#include <GameObject/game_object.h>
#include <Components/component.h>
#include <Components/transform.h>

using namespace MirageEngine;
using namespace entt::literals;

json Serializer::SerializeScene(SceneGraph _sceneGraph)
{
    json jScene;

    // Serialize all scene roots
    for (std::shared_ptr<GameObject> root : _sceneGraph.GetRootObjects())
    {
        jScene["children"].push_back(SerializeGameObject(root));
    }

    return jScene;
}

json Serializer::SerializeGameObject(std::shared_ptr<GameObject> _gameObject)
{
    json jGameObject;

    jGameObject["id"] = _gameObject->GetID();

    // Serialize all gameobjects components
    for (std::shared_ptr<Core::Components::Component> component : _gameObject->GetComponents()) {
        jGameObject["components"].push_back(SerializeComponent(component));
    }

    // Recursivly serialize all gameobjects children
    for (std::shared_ptr<GameObject> child : _gameObject->GetChildren()) {
        jGameObject["children"].push_back(SerializeGameObject(child));
    }

    return jGameObject;
}

json MirageEngine::Serializer::SerializeComponent(std::shared_ptr<Core::Components::Component> _component)
{
    json jComponent;

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

json Serializer::SerializeTransform(std::shared_ptr <Core::Components::Transform> _transform)
{
    json jTransform;



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

    /*if (actualType == entt::resolve<std::string>())
    {
        if (auto* s = value.try_cast<std::string>()) {
            char buffer[256];
            strncpy(buffer, s->c_str(), sizeof(buffer) - 1);
            if (ImGui::InputText(label, buffer, sizeof(buffer))) {
                _data.set(instance, std::string(buffer));
            }
        }
    }*/
}
