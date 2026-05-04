#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "GameObject/game_object.h"	

//class GameObject;
namespace MirageEngine::Core::Components
{
	class Camera;
}

namespace MirageEngine
{
	class Renderer;

	class SceneGraph
	{
	public:
	protected:
	private:
		std::vector<std::shared_ptr<GameObject>> m_RootsList;
		std::unordered_map<uint64_t, std::shared_ptr<MirageEngine::GameObject>> m_Registry;

		std::shared_ptr<GameObject> m_SelectedObject = nullptr;
		std::shared_ptr<GameObject> m_LastCreatedObject = nullptr;

	public:
		SceneGraph() = default;

        ~SceneGraph() = default;

		void Clean()
		{
            m_SelectedObject.reset();
            m_Registry.clear();
            m_RootsList.clear();
		}

		std::shared_ptr<GameObject> CreateGameObject(const std::string& _name, std::shared_ptr<GameObject> _parent = nullptr);
		void AddGameObject(std::shared_ptr<GameObject> _newGameObject);
		void RemoveGameObject(std::shared_ptr<GameObject> _obj);											
		void ClearScene();																	
		std::shared_ptr<GameObject> FindGameObject(uint64_t _id) const;
		std::shared_ptr<GameObject> CreateCamera(const std::string& _name = "Camera", std::shared_ptr<GameObject> _parent = nullptr);
		std::shared_ptr<GameObject> CreateObject3D(const std::string& _name = "Obj", const std::string& _mesh = "cube", std::shared_ptr<GameObject> _parent = nullptr, MirageEngine::Renderer* _renderer = nullptr);

		void UpdateAllWorldMatrices();															

		// Manages all the logic of change of parenting.
		bool SetParent(std::shared_ptr<GameObject> _parent, std::shared_ptr<GameObject> child);

		template<typename T>
		T* FindComponentInScene()
		{
			for (auto const& [id, gameObject] : m_Registry)
			{
				if (gameObject)
				{
					std::shared_ptr<T> comp = gameObject->GetComponent<T>();
					if (comp)
						return comp.get();
				}
			}
			return nullptr;
		}

		void SetSelectedObject(std::shared_ptr<GameObject> _obj) { m_SelectedObject = _obj; }
		std::shared_ptr<GameObject> GetSelectedObject() { return m_SelectedObject; }
		const std::vector<std::shared_ptr<GameObject>>& GetRootObjects() const { return m_RootsList; }
		std::vector<std::shared_ptr<GameObject>> GetChildren(std::shared_ptr<GameObject> _parent) const;
		std::shared_ptr<GameObject> GetLastCreatedObject() const { return m_LastCreatedObject; }

	protected:
	private:
		void WorldMatrixRecursive(std::shared_ptr<GameObject> _obj, Core::Math::Matrix4& _parentMatrix, bool _forceUpdate);
	};
}