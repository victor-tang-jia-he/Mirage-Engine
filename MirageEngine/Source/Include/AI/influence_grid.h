#pragma once 
#include <iostream>
#include <string>
#include <vector>
#include <vector3.h>

namespace MirageEngine
{
	enum class InfluenceType
	{
		CHANNEL_DANGER = 0,
		CHANNEL_SAFETY = 1,
		CHANNEL_RESOURCE = 2,
		CHANNEL_COVER = 3,
		COUNT = 4
	};

	struct InfluenceCell
	{
		static constexpr int MAX_CHANNELS = 4;
		float channels[MAX_CHANNELS] = { 0.0f };
	};

	class InfluenceGrid
	{
	private:
		// Data the user can set
		float m_GridWidth;
		float m_GridHeight;
		float m_CellSize;
		float m_DecayRate = 0.95f;

		// Internal data calculated from the other parameters
		int m_ResolutionX;
		int m_ResolutionY;

		std::vector<InfluenceCell> m_cells;

		Core::Components::Vector3 m_Origin;

	public:
		void Initialize(float _gridWidth, float _gridHeight, float _cellsize, Vector3 _origin);

		void WorldToGrid(Vector3 _pos, int& _x, int& _y);

		bool IsInBounds(int _x, int _y) const;
		void Update(float _deltaTime);

		void AddInfluence(int _x, int _y, InfluenceType _type, float _value);
		void AddInfluenceArea(Vector3 pos, float radius, float value, InfluenceType type);

		// Getters
		int GetIndex(int x, int y) const { return y * m_ResolutionX + x; }
		Core::Components::Vector3 GetCellWorldPosition(int x, int y)
	};
}