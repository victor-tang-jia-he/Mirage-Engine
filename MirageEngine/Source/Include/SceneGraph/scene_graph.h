#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>
#include "GameObject/game_object.h"	

//class GameObject;
class Camera;

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
		Camera* m_EditorCamera = nullptr;
		Camera* m_ActiveCamera = nullptr;

	public:
		SceneGraph() = default;
		~SceneGraph() { ClearScene(); }

		GameObject* CreateGameObject(const std::string& _name, GameObject* _parent = nullptr);  // done
		void RemoveGameObject(GameObject* _obj);												// done
		void ClearScene();																		// done
		GameObject* FindGameObject(uint64_t _id) const;

		void UpdateWorldMatrix(GameObject* _startObject);
		void UpdateAllWorldMatrices();

		bool SetParent(GameObject* _parent, GameObject* child); // Manages all the logic of change of parenting.  // done
		std::vector<GameObject*> GetChildren(GameObject* _parent);												  // done

		void ObjectPicking(float _mouseX, float _mouseY); // Porjects a raycast to select an object

		void SetSelectedObject(GameObject* _obj) { m_SelectedObject = _obj; }
		GameObject* GetSelectedObject() { return m_SelectedObject; }
		const std::vector<GameObject*>& GetRootObjects() const { return m_RootsList; }
		std::vector<GameObject*> GetChildren(GameObject* _parent) const;

		void SetEditorCamera(Camera* _cam) { m_EditorCamera = _cam; m_ActiveCamera = _cam; }
		Camera* GetActiveCamera() const { return m_ActiveCamera; }
	protected:
	private:
		void WorldMatrixRecursive(GameObject* _obj, const void* _parentMatrix);
	};
}