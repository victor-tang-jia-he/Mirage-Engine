#include "SceneGraph/scene_graph.h"
#include <matrix4.h>

namespace MirageEngine
{
	GameObject* SceneGraph::CreateGameObject(const std::string& _name, GameObject* _parent)
	{
		GameObject* newObject = new GameObject(nullptr, _name);
		uint64_t objectID = newObject->GetID();
		auto [it, success] = m_Registry.try_emplace(objectID, newObject);

		if (!success) {
		}

		this->SetParent(_parent, newObject);
		return newObject;
	}

	void SceneGraph::RemoveGameObject(GameObject* _obj)
	{
		if (!_obj) return;

		std::vector<GameObject*> childrenCopy = _obj->GetChildren();
		for (GameObject* child : childrenCopy)
			RemoveGameObject(child);

		if (_obj->GetParent() == nullptr)	
			std::erase(m_RootsList, _obj);
		else
			_obj->GetParent()->RemoveChild(_obj);

		m_Registry.erase(_obj->GetID());

		if (m_SelectedObject == _obj) 
			m_SelectedObject = nullptr;

		delete _obj;
	}

	void SceneGraph::ClearScene()
	{
		std::vector<GameObject*> rootsCopy = m_RootsList;
		for (GameObject* obj : rootsCopy) 
		{
			RemoveGameObject(obj);
		}	
		m_Registry.clear();
		m_RootsList.clear();
	}

	void SceneGraph::UpdateAllWorldMatrices()
	{
		MirageEngine::Core::Math::Matrix4 Identity = Core::Math::Matrix4::Identity();

		for (GameObject* root : m_RootsList)
		{
			WorldMatrixRecursive(root, &Identity, false);
		}
	}

	std::vector<GameObject*> SceneGraph::GetChildren(GameObject* _parent) const
	{
		if (_parent == nullptr)
			return m_RootsList;
		return _parent->GetChildren();
	}

	bool SceneGraph::SetParent(GameObject* _parent, GameObject* _child)
	{
		if (!_child) return false;
		if (_child == _parent) return false;

		std::cout << "\n--- DEBUT SET PARENT ---" << std::endl;
		std::cout << "Enfant: " << _child->GetName() << " | Parent: " << (_parent ? _parent->GetName() : "Root") << std::endl;

		Core::Math::Matrix4 childWorldBefore = _child->Transform.GetWorldMatrix();

		GameObject* current = _parent;
		while (current != nullptr)
		{
			if (current == _child)
			{	
				std::cout << "Error: Circular dependency detected!" << std::endl;
				return false;
			}
			current = current->GetParent();
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

			_child->Transform.SetLocalMatrix(newLoc);
			std::cout << "New Local calculated. Scale: " << newLoc.GetScale().X << std::endl;
		}
		else
		{
			std::cout << "Root" << std::endl;
			_child->Transform.SetLocalMatrix(childWorldBefore);
		}

		_child->SetDirty(true);

		return true;
	}

	void SceneGraph::WorldMatrixRecursive(GameObject* _obj, const void* _parentMatrix, bool _forceUpdate)
	{
		bool mustUpdate = _obj->Transform.IsDirty() || _forceUpdate;

		if (mustUpdate)
		{
			Core::Math::Matrix4 localMat = _obj->Transform.GetMatrix();
			const Core::Math::Matrix4* parentMat = static_cast<const Core::Math::Matrix4*>(_parentMatrix);
			Core::Math::Matrix4 worldMatrix = (*parentMat) * localMat;
			_obj->Transform.SetWorldMatrix(worldMatrix);
			for (GameObject* child : _obj->GetChildren())
			{
				WorldMatrixRecursive(child, &worldMatrix, true);
			}
		}
		else
		{
			for (GameObject* child : _obj->GetChildren())
			{
				WorldMatrixRecursive(child, &_obj->Transform.GetWorldMatrix(), false);
			}
		}
	}
}