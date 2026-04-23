#pragma once 
#include <iostream>
#include <vector>
#include <vector3.h>
#include "AI/influence_grid.h"

namespace MirageEngine::Core::Components
{
	class InfluenceEmitter : public Component
	{
	private:
		InfluenceType m_Type;
		float m_Radius = 5.0f;
		float m_Strength = 10.0f;

		InfluenceGrid* m_Grid = nullptr;


	public:
		InfluenceEmitter() = default;
		virtual ~InfluenceEmitter() = default;

		void Init() override;
		void Update() override;
		entt::id_type GetType() const override { return entt::type_id<InfluenceEmitter>().hash(); }

		// Getters
		InfluenceType GetInfluenceType() const { return m_Type; }
		float GetRadius() const { return m_Radius; }
		float GetStrength() const { return m_Strength; }
		InfluenceGrid* GetTargetGrid() const { return m_Grid; }

		// Setters
		void SetInfluenceType(InfluenceType _type) { m_Type = _type; }
		void SetRadius(float _radius) { m_Radius = _radius; }
		void SetStrength(float _strength) { m_Strength = _strength; }
		void SetTargetGrid(InfluenceGrid* _grid) { m_Grid = _grid; }
	};
}