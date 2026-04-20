#include "AI/influence_grid.h"
#include "Components/transform.h"
#include <matrix4.h>

using namespace MirageEngine::Core::Math;

namespace MirageEngine
{

	void InfluenceGrid::Initialize(float _gridWidth, float _gridHeight, float _cellsize, Vector3 _origin)
	{
		m_GridWidth = _gridWidth;
		m_GridHeight = _gridHeight;
		m_CellSize = _cellsize;
		m_Origin = _origin;

		m_ResolutionX = static_cast<int>(m_GridWidth / m_CellSize);
		m_ResolutionY = static_cast<int>(m_GridHeight / m_CellSize);

		m_cells.resize(m_ResolutionX * m_ResolutionY);
	}

	void InfluenceGrid::WorldToGrid(Vector3 _pos, int& _x, int& _y)
	{
		_x = static_cast<int>(_pos.X - m_Origin.X) / m_CellSize;
		_y = static_cast<int>(_pos.Y - m_Origin.Y) / m_CellSize;
	}

	bool InfluenceGrid::IsInBounds(int _x, int _y) const
	{

	}

	void InfluenceGrid::Update(float _deltaTime)
	{

	}

	void InfluenceGrid::AddInfluence(int _x, int _y, InfluenceType _type, float _value)
	{

	}

	void InfluenceGrid::AddInfluenceArea(Vector3 pos, float radius, float value, InfluenceType type)
	{

	}
}