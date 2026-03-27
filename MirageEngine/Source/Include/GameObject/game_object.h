#pragma once

#include <iostream>
#include <string>

class Components;

namespace MirageEngine
{
	// Static : Transform matrix doesn't update every frame
	// Dynamic : Transform matrix updates every frame

	class GameObject
	{
	private:
		std::string m_Name = "";
		bool m_bIsActive = true;
		bool m_bIsStatic = false;

	public:
		GameObject(std::string _name);
		~GameObject();

		void SetActive(bool _isActive);

		// Getters
		std::string GetName() const { return m_Name; }
		bool GetIsActive() const { return m_bIsActive; }

	
	};
}
