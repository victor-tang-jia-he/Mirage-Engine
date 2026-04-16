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
	class SceneGraph
	{
	public:
	protected:
	private:
		std::vector<GameObject*> m_RootsList;
		std::unordered_map<uint64_t, GameObject*> m_Registry;

		GameObject* m_SelectedObject = nullptr;
		MirageEngine::Core::Components::Camera* m_EditorCamera = nullptr;
		MirageEngine::Core::Components::Camera* m_ActiveCamera = nullptr;

	public:
		SceneGraph() = default;
		~SceneGraph() { }

		GameObject* CreateGameObject(const std::string& _name, GameObject* _parent = nullptr); 
		void RemoveGameObject(GameObject* _obj);											
		void ClearScene();																		
		GameObject* FindGameObject(uint64_t _id) const;

		void UpdateWorldMatrix(GameObject* _startObject);
		void UpdateAllWorldMatrices();															

		// Manages all the logic of change of parenting.
		bool SetParent(GameObject* _parent, GameObject* child);

		// Projects a raycast to select an object
		void ObjectPicking(float _mouseX, float _mouseY);

		void SetSelectedObject(GameObject* _obj) { m_SelectedObject = _obj; }
		GameObject* GetSelectedObject() { return m_SelectedObject; }
		const std::vector<GameObject*>& GetRootObjects() const { return m_RootsList; }
		std::vector<GameObject*> GetChildren(GameObject* _parent) const;

		void SetEditorCamera(MirageEngine::Core::Components::Camera* _cam) { m_EditorCamera = _cam; m_ActiveCamera = _cam; }
		MirageEngine::Core::Components::Camera* GetActiveCamera() const { return m_ActiveCamera; }
	protected:
	private:
		void WorldMatrixRecursive(GameObject* _obj, Core::Math::Matrix4& _parentMatrix, bool _forceUpdate);
		GameObject* CreateCube(const std::string& _name = "Cube", GameObject* _parent = nullptr);
		GameObject* CreateCamera(const std::string& _name = "Camera", GameObject* _parent = nullptr);
	};
}