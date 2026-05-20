#include <SceneGraph/scene_graph.h>
#include <matrix4.h>

#include <Components/mesh_renderer.h>

#include <Resources/material.h>
#include <Resources/texture.h>
#include <Resources/shader.h>
#include <Resources/resource_manager.h>

#include <renderer.h>

#include <Components/camera.h>
#include <i_uniform_buffer.h>

#include <Logs/log.h>
#include <Components/rigidbody.h>

DEFINE_LOG_CATEGORY(LogSceneGraph);

using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Resources;

namespace MirageEngine
{
    std::shared_ptr<GameObject> SceneGraph::CreateGameObject(const std::string& _name, std::shared_ptr<GameObject> _parent)
    {
        std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(nullptr, _name);
        uint64_t objectID = newObject->GetID();
        auto [it, success] = m_Registry.try_emplace(objectID, newObject);
		m_CallGameObjectOnStart.push_back(newObject);
        if (!success) {}

        this->SetParent(_parent, newObject);
        m_LastCreatedObject = newObject;
        return newObject;
    }

    void SceneGraph::AddGameObject(std::shared_ptr<GameObject> _newGameObject)
    {
        uint64_t objectID = _newGameObject->GetID();
        auto [it, success] = m_Registry.try_emplace(objectID, _newGameObject);
        m_CallGameObjectOnStart.push_back(_newGameObject);
        this->SetParent(_newGameObject->GetParent(), _newGameObject);
        m_LastCreatedObject = _newGameObject;
    }

    std::shared_ptr<GameObject> SceneGraph::CreateObject3D(const std::string& _name, const std::string& _mesh, std::shared_ptr<GameObject> _parent, MirageEngine::Renderer* _renderer)
    {
        std::shared_ptr<GameObject> newObject = CreateGameObject(_name, _parent);
        std::shared_ptr<Core::Components::MeshRenderer> meshRenderer = newObject->AddComponent<Core::Components::MeshRenderer>();
        ResourceManager& rm = ResourceManager::Instance();

        meshRenderer->SetMesh(rm.GetMesh(_mesh));

        std::shared_ptr<Texture> texture = rm.GetTexture("White");
        std::shared_ptr<Material> material = rm.GetMaterial("default"); // using default shader (fallback)
        std::shared_ptr<MaterialInstance> matInstance = rm.CreateMaterialInstance("WhiteInstance", material);

        if (texture)
        {
            texture->Bind();
            matInstance->SetTexture("White", texture);
        }

        meshRenderer->SetMaterial(matInstance);
        newObject->Transform.SetPosition(Vector3(0, 0, -10));
        m_LastCreatedObject = newObject;
        return newObject;
    }

    std::shared_ptr<GameObject> SceneGraph::CreateCamera(const std::string& _name, std::shared_ptr<GameObject> _parent)
    {
        std::shared_ptr<GameObject> newObject = CreateGameObject(_name, _parent);
        std::shared_ptr<Core::Components::Camera> camera = newObject->AddComponent<Core::Components::Camera>();
        camera->SetFar(100);
        camera->SetHeight(1080);
        camera->SetWidth(1920);
        camera->SetNear(0.1);
        camera->SetFov(60.f);
        m_LastCreatedObject = newObject;
        return newObject;
    }

    void SceneGraph::RemoveGameObject(std::shared_ptr<GameObject> _obj)
    {
        if (!_obj)
            return;

        std::vector<std::shared_ptr<GameObject>> childrenCopy = _obj->GetChildren();
        for (std::shared_ptr<GameObject> child : childrenCopy)
            RemoveGameObject(child);

        if (_obj->GetParent() == nullptr)
            std::erase(m_RootsList, _obj);
        else
            _obj->GetParent()->RemoveChild(_obj);

        m_Registry.erase(_obj->GetID());
        m_CallGameObjectOnStart.erase(std::remove_if(m_CallGameObjectOnStart.begin(), m_CallGameObjectOnStart.end(),
            [_obj](const std::weak_ptr<GameObject>& weakObj) {
                return weakObj.lock() == _obj;
            }), m_CallGameObjectOnStart.end());
        if (m_SelectedObject.lock() == _obj)
            m_SelectedObject.reset();

        for (std::shared_ptr<Core::Components::Component> comp : _obj->GetComponents())
        {
			_obj->RemoveComponent(comp);
        }
    }

    void SceneGraph::ClearScene()
    {
        //std::vector<std::shared_ptr<GameObject>> rootsCopy = m_RootsList;
        while (!m_RootsList.empty())
        {
            std::shared_ptr<GameObject> obj = m_RootsList.back();
            RemoveGameObject(obj);
        }
        m_Registry.clear();
        m_RootsList.clear();
    }

    std::shared_ptr<GameObject> SceneGraph::FindGameObject(uint64_t _id) const
    {
        return m_Registry.contains(_id) ? m_Registry.at(_id) : nullptr;
    }
    
    void SceneGraph::CallAllOnStart()
    {
        if (m_CallGameObjectOnStart.empty())
			return;
        for (std::weak_ptr<GameObject> obj : m_CallGameObjectOnStart)
        {
            if (std::shared_ptr<GameObject> sharedObj = obj.lock())
            {
                for (std::shared_ptr<MirageEngine::Core::Components::Component> component : sharedObj->GetComponents())
                {
                    component->OnStart();
                }
            }
        }
		// After calling OnStart for all objects, clear the list to avoid calling it again in the future.
        m_CallGameObjectOnStart.clear();
    }

    void SceneGraph::CallAllUpdate(float dt)
    {
        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            CallAllUpdateRecursive(root, dt);
        }
    }

    void SceneGraph::CallAllLateUpdate(float dt)
    {
        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            CallAllLateUpdateRecursive(root, dt);
        }
    }

    void SceneGraph::CallAllFixedUpdate(float dt)
    {
        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            CallAllFixedUpdateRecursive(root, dt);
        }
    }

    void SceneGraph::CallAllDrawLine()
    {
        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            CallAllDrawLineRecursive(root);
        }
    }

    void SceneGraph::CallAllUpdateRecursive(std::shared_ptr<GameObject> _obj, float dt)
    {
        for (auto& component : _obj->GetComponents())
        {
            component->OnUpdate(dt);
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            CallAllUpdateRecursive(child, dt);
        }
    }

    void SceneGraph::CallAllLateUpdateRecursive(std::shared_ptr<GameObject> _obj, float dt)
    {
        for (auto& component : _obj->GetComponents())
        {
            component->OnLateUpdate(dt);
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            CallAllLateUpdateRecursive(child, dt);
        }
    }

    void SceneGraph::CallAllFixedUpdateRecursive(std::shared_ptr<GameObject> _obj, float dt)
    {
        for (auto& component : _obj->GetComponents())
        {
            component->OnFixedUpdate(dt);
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            CallAllFixedUpdateRecursive(child, dt);
        }
    }

    void SceneGraph::CallAllDrawLineRecursive(std::shared_ptr<GameObject> _obj)
    {
        for (auto& component : _obj->GetComponents())
        {
            component->OnDrawLine();
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            CallAllDrawLineRecursive(child);
        }
    }

    void SceneGraph::UpdateAllWorldMatrices()
    {
        MirageEngine::Core::Math::Matrix4 Identity = Core::Math::Matrix4::Identity();

        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            WorldMatrixRecursive(root, Identity, false);
        }
    }

    std::vector<std::shared_ptr<GameObject>> SceneGraph::GetChildren(std::shared_ptr<GameObject> _parent) const
    {
        if (_parent == nullptr)
            return m_RootsList;
        return _parent->GetChildren();
    }

    bool SceneGraph::SetParent(std::shared_ptr<GameObject> _parent, std::shared_ptr<GameObject> _child)
    {
        if (!_child)
            return false;
        if (_child == _parent)
            return false;

        Core::Math::Matrix4 childWorldBefore = _child->Transform.GetWorldMatrix();

        std::shared_ptr<GameObject> current = _parent;
        while (current != nullptr)
        {
            if (current == _child)
            {
                return false;
            }
            current = current->GetParent();
        }

        if (_child->GetParent() == nullptr) 
        {
            std::erase(m_RootsList, _child);
        }
        else 
        { 
            _child->GetParent()->RemoveChild(_child); 
        }

        if (_parent == nullptr)
        {
            m_RootsList.push_back(_child);
            _child->SetParent(nullptr);
        }
        else
        {
            _parent->AddChild(_child);
            _child->SetParent(_parent);
        }

        if (_parent != nullptr)
        {
            Core::Math::Matrix4 invParent = _parent->Transform.GetWorldMatrix().Inverse();
            Core::Math::Matrix4 newLoc = invParent * childWorldBefore;

            //_child->Transform.SetLocalMatrix(newLoc);
            _child->SetPosition(newLoc.GetTranslation());
            _child->SetRotation(newLoc.GetRotation());
            LOG(logTemp, LogSceneGraph, "New Local calculated. Scale: ", newLoc.GetScale().X);
        }
        else
        {
            //_child->Transform.SetLocalMatrix(childWorldBefore);
            _child->SetPosition(childWorldBefore.GetTranslation());
            _child->SetRotation(childWorldBefore.GetRotation());
        }

        _child->SetDirty(true);

        return true;
    }

    void SceneGraph::WorldMatrixRecursive(std::shared_ptr<GameObject> _obj, Core::Math::Matrix4& _parentMatrix, bool _forceUpdate)
    {
        bool mustUpdate = _obj->GetIsDirty() || _forceUpdate;

        Core::Math::Matrix4 worldMatrix;

        if (mustUpdate)
        {
            const Core::Math::Matrix4 localMat = _obj->Transform.GetMatrix();
            worldMatrix = _parentMatrix * localMat;
            _obj->Transform.SetWorldMatrix(worldMatrix);
        }
        else
        {
            worldMatrix = _obj->Transform.GetWorldMatrix();
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            WorldMatrixRecursive(child, worldMatrix, mustUpdate);
        }
    }
} // namespace MirageEngine
