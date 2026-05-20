#include <Serialization/deserializer.h>
#include <SceneGraph/scene_graph.h>
#include <GameObject/game_object.h>
#include <Components/camera.h>
#include <Components/rigidbody.h>
#include <AI/pathfinding.h>
#include <Scripting/script_manager.h>
#include <Scripting/i_script.h>
#include <fstream>

DEFINE_LOG_CATEGORY(LogDeserializer)

using namespace MirageEngine;
using namespace MirageEngine::Core;
using namespace entt::literals;

std::unordered_map<std::string, json> Deserializer::m_ComponentSearchMap = std::unordered_map<std::string, json>();

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

    m_ComponentSearchMap.clear();

    std::shared_ptr<SceneGraph> savedScene = std::make_shared<SceneGraph>();

    for (json jGameObject : _jSceneGraph["Roots"])
    {
        DeserializeGameObject(jGameObject, nullptr, savedScene);
    }

	savedScene->SetActiveCamera(savedScene->FindComponentInScene<Core::Components::Camera>());

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

void MirageEngine::Deserializer::DeserializeComponent(json _jComponent, std::shared_ptr<GameObject> _owner)
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

    entt::meta_any result = compFunc.invoke({}, _owner);
    if (!result)
    {
        LOG(logError, LogDeserializer, "Meta Invoke failed for type : ", metaType.name());
        return;
    }

    entt::meta_any compInstance = metaType.from_void(_owner->GetLastAddedComponent().get());
    if (!compInstance) {
        LOG(logError, LogDeserializer, "Failed to construct Meta Instance for type : ", metaType.name());
        return;
    }

    for (auto [id, data] : metaType.data())
    {
        // Converts the Json data into corresponding Engine data and adds it to the Components Meta Instance
        DeserializeMetaData(metaType, data, compInstance, _jComponent, _owner);
    }
}

entt::meta_any MirageEngine::Deserializer::DeserializeTransform(json _jComponent, std::shared_ptr<GameObject> _owner)
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
        DeserializeMetaData(metaType, data, compInstance, _jComponent, _owner);
    }

    if (_owner)
    {
        _owner->AddComponentFromMeta(compInstance);
    }

    return compInstance;
}

void MirageEngine::Deserializer::DeserializeMetaData(entt::meta_type _type, entt::meta_data _data, entt::meta_any& _instance, json _jComponent, std::shared_ptr<GameObject> _owner)
{
    entt::meta_any value = _data.get(_instance);

    if (!value)
    {
        LOG(logError, LogDeserializer, "Meta Data value empty!");
        return;
    }

    entt::meta_type actualType = value.type();

	// === Basic types handling ===
    if (actualType == entt::resolve<std::string>())
    {
        std::string stringData = _jComponent[_data.name()].get<std::string>();
        _data.set(_instance, stringData);
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
    // === Specific containers handling ===
    // = Vectors =
    else if (actualType == entt::resolve<std::vector<float>>())
    {
        std::vector<float> floatVectorData = DeserializeVector<float>(_jComponent[_data.name()]);
        _data.set(_instance, floatVectorData);
        return;
    }
    else if (actualType == entt::resolve<std::vector<std::string>>())
    {
        std::vector<std::string> stringVectorData = DeserializeVector<std::string>(_jComponent[_data.name()]);
        _data.set(_instance, stringVectorData);
        return;
    }
	// = Maps =
    else if (actualType == entt::resolve<std::unordered_map<std::string, float>>())
    {
        std::unordered_map<std::string, float> mapData = DeserializeMap<float>(_jComponent[_data.name()]);
        _data.set(_instance, mapData);
        return;
    }
    // === Specific types handling ===
    // = Components =
    else if (actualType == entt::resolve<AI::NavProfile>())
    {
        json navProfileData = _jComponent[_data.name()];
        AI::NavProfile navProfileInstance = DeserializeNavProfile(navProfileData);
        _data.set(_instance, navProfileInstance);
        return;
	}
    else if (actualType == entt::resolve<std::vector<AI::AIAction*>>())
    {
		std::vector<AI::AIAction*> aiActionsData;
        json jAIActionsData = _jComponent[_data.name()];
        for (json jAIAction : jAIActionsData)
        {
            AI::AIAction* aiActionInstance = DeserializeAIAction(jAIAction, _owner);
            
            aiActionsData.emplace_back(aiActionInstance);
		}
        _data.set(_instance, aiActionsData);
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
	// = Math types =
    else if (actualType == entt::resolve<Math::Vector3>())
    {
        Math::Vector3 vec3Data = DeserializeVector3(_jComponent[_data.name()]);
        _data.set(_instance, vec3Data);
        return;
    }
    else if (actualType == entt::resolve<Math::Vector4>())
    {
        Math::Vector4 vec4Data = DeserializeVector4(_jComponent[_data.name()]);
        _data.set(_instance, vec4Data);
        return;
    }
    else if (actualType == entt::resolve<Math::Quaternion>())
    {
        Math::Quaternion quatData = DeserializeQuaternion(_jComponent[_data.name()]);
        _data.set(_instance, quatData);
        return;
    }
	// = Enums =
    else if (actualType.is_enum())
    {
        int enumData = _jComponent[_data.name()].get<int>();
        _data.set(_instance, enumData);
        return;
    }
	// = Pointer-like types =
    else if (actualType.is_pointer_like())
    {
        json jPointer = _jComponent[_data.name()];

        if (!jPointer.is_null())
        {
            if (jPointer.contains("OwnerID"))
            {
                std::string typeName = jPointer["Type"].get<std::string>();

				m_ComponentSearchMap.emplace(typeName, jPointer);
                return;
			}
            else
            {
                std::string typeName = jPointer["Type"].get<std::string>();
                entt::hashed_string hashed{ typeName.c_str() };
                entt::meta_type metaType = entt::resolve(hashed);

                entt::meta_func getPointer = metaType.func("Get"_hs);
                _data.set(_instance, getPointer.invoke({}, jPointer["Name"].get<std::string>()));
            }
            return;
        }
    }

    const char* label = _data.name();
    if (!label)
        label = "Property";

    LOG(logWarning, LogDeserializer, "Meta Data Mismatch : ", label);
}

AI::NavProfile MirageEngine::Deserializer::DeserializeNavProfile(json _jNavProfile)
{
	AI::NavProfile navProfile;

	navProfile.InfluenceWeights = DeserializeVector<float>(_jNavProfile["Weights"]);
	navProfile.CanWalkDiagonal = _jNavProfile["Can Walk Diagonal"].get<bool>();

    return navProfile;
}

AI::AIAction* MirageEngine::Deserializer::DeserializeAIAction(json _jAIAction, std::shared_ptr<GameObject> _owner)
{
    auto script = Scripting::ScriptManager::Instance().CreateScriptByName("ActionMoveTo");
    _owner->AddComponent(std::move(script));
    std::shared_ptr<AI::AIAction> aiAction = _owner->GetComponent<AI::AIAction>();

	std::vector<Consideration> considerations;
    for (json jConsideration : _jAIAction["Considerations"])
    {
        Consideration consideration = DeserializeConsideration(jConsideration);
        considerations.emplace_back(consideration);
	}

    aiAction->SetConsiderations(considerations);
	return aiAction.get();
}

Consideration MirageEngine::Deserializer::DeserializeConsideration(json _jConsideration)
{
	Consideration consideration;

	consideration.source = static_cast<VariableSource>(_jConsideration["Source"].get<int>());
	consideration.variableName = _jConsideration["VariableName"].get<std::string>();

    std::string typeName = typeid(AI::ResponsiveCurve).name();
    entt::hashed_string hashed{ typeName.c_str() };
    entt::meta_type metaType = entt::resolve(hashed);

	AI::ResponsiveCurve* responseCurve = new AI::ResponsiveCurve();

    entt::meta_any responseCurveInstance = metaType.from_void(responseCurve);

    for (auto [id, data] : metaType.data())
    {
        DeserializeMetaData(metaType, data, responseCurveInstance, _jConsideration["ResponseCurve"], nullptr);
	}

    consideration.responseCurve = *responseCurve;

	delete responseCurve;

    return consideration;
}

Core::Math::Vector3 MirageEngine::Deserializer::DeserializeVector3(json _jVector3)
{
    Core::Math::Vector3 savedVector3;

    savedVector3.X = _jVector3["X"].get<float>();
    savedVector3.Y = _jVector3["Y"].get<float>();
    savedVector3.Z = _jVector3["Z"].get<float>();

    return savedVector3;
}

Core::Math::Vector4 MirageEngine::Deserializer::DeserializeVector4(json _jVector4)
{
    Core::Math::Vector4 savedVector4;

    savedVector4.X = _jVector4["X"].get<float>();
    savedVector4.Y = _jVector4["Y"].get<float>();
    savedVector4.Z = _jVector4["Z"].get<float>();
    savedVector4.W = _jVector4["W"].get<float>();

    return savedVector4;
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