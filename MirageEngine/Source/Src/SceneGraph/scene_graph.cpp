#include "SceneGraph/scene_graph.h"

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

	void SceneGraph::SetParent(GameObject* _parent, GameObject* _child)
	{
		if (!_child) return;

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
			(_parent->AddChild(_child));
			_child->SetParent(_parent);
		}

		_child->SetDirty(true);
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
}