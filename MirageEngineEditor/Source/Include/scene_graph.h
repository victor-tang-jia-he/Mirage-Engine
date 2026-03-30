#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <cstdint>

class GameObject;
class Camera;

namespace Mirage
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

	public:
		void AddGameObject(GameObject* _obj);
		void RemoveGameObject(GameObject* _obj);
		void ClearScene();

		void UpdateWorldMatrix(GameObject* _startObject);

		void SetParent(GameObject* _child, GameObject* _parent); // Manages all the logic of change of parenting.
		std::vector<GameObject*> GetChildren(GameObject* _parent);

		void ObjectPicking(float _mouseX, float _mouseY); // Porjects a raycast to select an object

		void SetSelectedObject(GameObject* _obj) { m_SelectedObject = _obj; }
		GameObject* GetSelectedObject() { return m_SelectedObject; }
		const std::vector<GameObject*>& GetRootObjects() const { return m_RootsList; }
	protected:
	private:
		void WorldMatrixRecursive(GameObject* _obj, const void* _parentMatrix);
	};
}