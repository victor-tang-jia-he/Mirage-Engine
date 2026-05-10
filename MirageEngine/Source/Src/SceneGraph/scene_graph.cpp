#include "SceneGraph/scene_graph.h"
#include <matrix4.h>
#include <vector3.h>

#include "Resources/resource_manager.h"
#include "Components/mesh_renderer.h"
#include <Resources/material.h>
#include <Resources/texture.h>
#include <Resources/shader.h>

#include "renderer.h"

#include "Components/camera.h"
#include <i_uniform_buffer.h>

using namespace MirageEngine::Core::Math;
using namespace MirageEngine::Core::Resources;

namespace MirageEngine
{
    std::shared_ptr<GameObject> SceneGraph::CreateGameObject(const std::string& _name, std::shared_ptr<GameObject> _parent)
    {
        std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(nullptr, _name);
        uint64_t objectID = newObject->GetID();
        auto [it, success] = m_Registry.try_emplace(objectID, newObject);

        if (!success) {}

        this->SetParent(_parent, newObject);
        m_LastCreatedObject = newObject;
        return newObject;
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

        if (m_SelectedObject == _obj)
            m_SelectedObject = nullptr;

        _obj.reset();
    }

    void SceneGraph::ClearScene()
    {
        std::vector<std::shared_ptr<GameObject>> rootsCopy = m_RootsList;
        for (std::shared_ptr<GameObject> obj : rootsCopy)
        {
            RemoveGameObject(obj);
        }
        m_Registry.clear();
        m_RootsList.clear();
    }

    void SceneGraph::CallAllOnStart()
    {
        for (std::shared_ptr<GameObject> root : m_RootsList)
        {
            CallAllOnStartRecursive(root);
        }
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

    void SceneGraph::CallAllOnStartRecursive(std::shared_ptr<GameObject> _obj)
    {
        for (auto& component : _obj->GetComponents())
        {
            component->OnStart();
        }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren())
        {
            CallAllOnStartRecursive(child);
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

        while (current != nullptr)
        {
            auto currentShared = current;

            if (currentShared == _child)
            {
                std::cout << "Error: Circular dependency detected!" << std::endl;
                return false;
            }

            current = currentShared->GetParent();
        }

        if (_child->GetParent() == nullptr)
            std::erase(m_RootsList, _child);
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
