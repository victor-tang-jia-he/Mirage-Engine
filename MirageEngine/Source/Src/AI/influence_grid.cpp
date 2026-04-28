#include "AI/influence_grid.h"
#include "GameObject/game_object.h"
#include "Components/transform.h"
#include <matrix4.h>
#include "time_manager.h"

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
	void InfluenceGrid::Init()
	{
		Bake();	
	}

	void InfluenceGrid::Update()
	{
		float dt = TimeManager::GetDeltaTime();
		UpdateDecay(dt);
	}

	void InfluenceGrid::Bake()
	{
		if (GetOwner())
		{
			m_Origin = GetOwner()->GetPosition();
			m_ResolutionX = static_cast<int>(m_GridWidth / m_CellSize);
			m_ResolutionY = static_cast<int>(m_GridHeight / m_CellSize);

			if (m_ResolutionX <= 0) m_ResolutionX = 1;
			if (m_ResolutionY <= 0) m_ResolutionY = 1;

			m_Cells.assign(m_ResolutionX * m_ResolutionY, InfluenceCell());
			for (InfluenceCell& cell : m_Cells)
				cell.channels.resize(m_ChannelsNames.size(), 0.0f);
		}
	}

	void InfluenceGrid::UpdateDecay(float _deltaTime)
	{
		float frameDecay = powf(m_DecayRate, _deltaTime);

		for (auto& cell : m_Cells)
		{
			for (int i = 0; i < m_ChannelsNames.size(); i++)
			{
				cell.channels[i] *= frameDecay;
				if (cell.channels[i] < 0.01f)
					cell.channels[i] = 0.0f;
			}
		}
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

	void InfluenceGrid::AddInfluence(int _x, int _y, int _channelIndex, float _value)
	{
		if (IsInBounds(_x, _y) && _channelIndex >= 0 && _channelIndex < m_ChannelsNames.size())
		{
			int index = GetIndex(_x, _y);
			m_Cells[index].channels[_channelIndex] += _value;
		}
	}

	void InfluenceGrid::AddInfluenceArea(Vector3 pos, float radius, float value, int _channelIndex)
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
							AddInfluence(cellX, cellY, _channelIndex, influenceValue);
						}
					}
				}
			}
		}
	}

	void InfluenceGrid::AddChannel(std::string _name)
	{
		m_ChannelsNames.push_back(_name);
		for (InfluenceCell& cell : m_Cells)
			cell.channels.push_back(0.0f);
	}

	Vector3 InfluenceGrid::GetCellWorldPosition(int _x, int _y) const
	{
		return Vector3(m_Origin.X + _x * m_CellSize + m_CellSize * 0.5f, m_Origin.Y, m_Origin.Z + _y * m_CellSize + m_CellSize * 0.5f);
	}

	int InfluenceGrid::GetChannelsIndex(std::string _name) const
	{
		for (size_t i = 0; i < m_ChannelsNames.size(); i++)
		{
			if (m_ChannelsNames[i] == _name)
				return static_cast<int>(i);
		}
		return -1; 
	}

	void InfluenceGrid::RenderGrid()
	{
		// To implement 
	}
}