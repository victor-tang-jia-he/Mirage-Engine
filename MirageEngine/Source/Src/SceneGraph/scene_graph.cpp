#include "SceneGraph/scene_graph.h"
#include <matrix4.h>

namespace MirageEngine
{
    std::shared_ptr<GameObject> SceneGraph::CreateGameObject(const std::string& _name, std::shared_ptr<GameObject> _parent)
    {
        std::shared_ptr<GameObject> newObject = std::make_shared<GameObject>(nullptr, _name);
        uint64_t                    objectID  = newObject->GetID();
        auto [it, success]                    = m_Registry.try_emplace(objectID, newObject);

        if (!success) {}

        this->SetParent(_parent, newObject);
        return newObject;
    }

    void SceneGraph::RemoveGameObject(std::shared_ptr<GameObject> _obj)
    {
        if (!_obj) return;

        std::vector<std::shared_ptr<GameObject>> childrenCopy = _obj->GetChildren();
        for (std::shared_ptr<GameObject> child : childrenCopy) RemoveGameObject(child);

        if (_obj->GetParent() == nullptr) std::erase(m_RootsList, _obj);
        else _obj->GetParent()->RemoveChild(_obj);

        m_Registry.erase(_obj->GetID());

        if (m_SelectedObject == _obj) m_SelectedObject = nullptr;

        _obj.reset();
    }

    void SceneGraph::ClearScene()
    {
        std::vector<std::shared_ptr<GameObject>> rootsCopy = m_RootsList;
        for (std::shared_ptr<GameObject> obj : rootsCopy) { RemoveGameObject(obj); }
        m_Registry.clear();
        m_RootsList.clear();
    }

    void SceneGraph::UpdateAllWorldMatrices()
    {
        MirageEngine::Core::Math::Matrix4 Identity = Core::Math::Matrix4::Identity();

        for (std::shared_ptr<GameObject> root : m_RootsList) { WorldMatrixRecursive(root, Identity, false); }
    }

    std::vector<std::shared_ptr<GameObject>> SceneGraph::GetChildren(std::shared_ptr<GameObject> _parent) const
    {
        if (_parent == nullptr) return m_RootsList;
        return _parent->GetChildren();
    }

    bool SceneGraph::SetParent(std::shared_ptr<GameObject> _parent, std::shared_ptr<GameObject> _child)
    {
        if (!_child) return false;
        if (_child == _parent) return false;

        std::cout << "\n--- DEBUT SET PARENT ---" << std::endl;
        std::cout << "Enfant: " << _child->GetName() << " | Parent: " << (_parent ? _parent->GetName() : "Root") << std::endl;

        Core::Math::Matrix4 childWorldBefore = _child->Transform.GetWorldMatrix();

        std::shared_ptr<GameObject> current = _parent;

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

        if (_child->GetParent() == nullptr) std::erase(m_RootsList, _child);
        else { _child->GetParent()->RemoveChild(_child); }

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
            Core::Math::Matrix4 newLoc    = invParent * childWorldBefore;

            //_child->Transform.SetLocalMatrix(newLoc);
            _child->SetPosition(newLoc.GetTranslation());
            _child->SetRotation(newLoc.GetRotation());
            std::cout << "New Local calculated. Scale: " << newLoc.GetScale().X << std::endl;
        }
        else
        {
            std::cout << "Root" << std::endl;
            //_child->Transform.SetLocalMatrix(childWorldBefore);
            _child->SetPosition(childWorldBefore.GetTranslation());
            _child->SetRotation(childWorldBefore.GetRotation());
        }

        _child->SetDirty(true);

        return true;
    }

    void SceneGraph::WorldMatrixRecursive(std::shared_ptr<GameObject> _obj, Core::Math::Matrix4& _parentMatrix, bool _forceUpdate)
    {
        bool mustUpdate = _obj->Transform.GetIsDirty() || _forceUpdate;

        Core::Math::Matrix4 worldMatrix;

        if (mustUpdate)
        {
            const Core::Math::Matrix4 localMat = _obj->Transform.GetMatrix();

            worldMatrix = _parentMatrix * localMat;

            _obj->Transform.SetWorldMatrix(worldMatrix);
        }
        else { worldMatrix = _obj->Transform.GetWorldMatrix(); }

        for (std::shared_ptr<GameObject> child : _obj->GetChildren()) { WorldMatrixRecursive(child, worldMatrix, mustUpdate); }
    }
} // namespace MirageEngine
