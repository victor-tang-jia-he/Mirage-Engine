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

		m_Cells.resize(m_ResolutionX * m_ResolutionY);
	}

	void InfluenceGrid::WorldToGrid(Vector3 _pos, int& _x, int& _y)
	{
		_x = static_cast<int>((_pos.X - m_Origin.X) / m_CellSize);
		_y = static_cast<int>((_pos.Z - m_Origin.Z) / m_CellSize);
	}

	bool InfluenceGrid::IsInBounds(int _x, int _y) const
	{
		return _x >= 0 && _x < m_ResolutionX && _y >= 0 && _y < m_ResolutionY;
	}

	void InfluenceGrid::Update(float _deltaTime)
	{
		float frameDecay = powf(m_DecayRate, _deltaTime);

		for (auto& cell : m_Cells)
		{
			for (int i = 0; i < InfluenceCell::MAX_CHANNELS; i++)
			{
				cell.channels[i] *= frameDecay;
				if (cell.channels[i] < 0.01f)
					cell.channels[i] = 0.0f;
			}
		}
	}

	void InfluenceGrid::AddInfluence(int _x, int _y, InfluenceType _type, float _value)
	{
		if (IsInBounds(_x, _y))
		{
			int index = GetIndex(_x, _y);
			m_Cells[index].channels[static_cast<int>(_type)] += _value;
		}
	}

	void InfluenceGrid::AddInfluenceArea(Vector3 pos, float radius, float value, InfluenceType type)
	{
		if(IsInBounds(pos.X, pos.Z))
		{
			int centerX, centerY;
			WorldToGrid(pos, centerX, centerY);
			int radiusInCells = static_cast<int>(radius / m_CellSize);
			for (int y = -radiusInCells; y <= radiusInCells; y++)
			{
				for (int x = -radiusInCells; x <= radiusInCells; x++)
				{
					int cellX = centerX + x;
					int cellY = centerY + y;
					if (IsInBounds(cellX, cellY))
					{
						float distSq = (x * x + y * y) * (m_CellSize * m_CellSize);
						float distance = sqrtf(distSq);

						if (distance <= radius)
						{
							float influenceValue = value * (1.0f - (distance / radius));
							AddInfluence(cellX, cellY, type, influenceValue);
						}
					}
				}
			}
		}
	}

	Vector3 InfluenceGrid::GetCellWorldPosition(int _x, int _y) const
	{
		return Vector3(m_Origin.X + _x * m_CellSize + m_CellSize * 0.5f, m_Origin.Y, m_Origin.Z + _y * m_CellSize + m_CellSize * 0.5f);
	}
}