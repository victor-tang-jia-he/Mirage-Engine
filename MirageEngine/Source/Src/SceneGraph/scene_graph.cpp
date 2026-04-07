#include "SceneGraph/scene_graph.h"

namespace MirageEngine
{
	GameObject* SceneGraph::CreateGameObject(const std::string& _name, GameObject* _parent)
	{
		GameObject* newObject = new GameObject(_parent, _name);
		uint64_t objectID = newObject->GetID();
		auto [it, success] = m_Registry.try_emplace(objectID, newObject);

		if (!success) {
		}

		this->SetParent(_parent, newObject);
		return newObject;
	}

	void SceneGraph::SetParent(GameObject* _parent, GameObject* _child)
	{
		if (_child->GetParent() == nullptr)
			std::erase(m_RootsList, _child);
		else
			_child->GetParent()->RemoveChild(_child);

		if (_parent == nullptr)
		{
			m_RootsList.push_back(_child);
			_child->SetParent(nullptr);
		}
		else
			(_parent->AddChild(_child));
	}
}