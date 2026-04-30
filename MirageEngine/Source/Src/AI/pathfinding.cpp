#include "AI/pathfinding.h"
#include "AI/influence_grid.h"
#include <stdlib.h>

namespace MirageEngine::AI
{
	std::vector<Vector3> Pathfinding::FindPath(Vector3 _start, Vector3 _end, InfluenceGrid* _grid, const NavProfile _profile)
	{
		int startX, startY;
		int endX, endY;
		_grid->WorldToGrid(_start, startX, startY);
		_grid->WorldToGrid(_end, endX, endY);

		if (_grid->IsInBounds(startX, startY))
		{
			if (_grid->IsInBounds(endX, endY))
			{
				int width = _grid->GetResX();
				int height = _grid->GetResY();
				ResetPool(width, height);

				// Start point initialization
				int index = _grid->GetIndex(startX, startY);
				PathNode& startNode = m_NodePool[index];
				startNode.gCost = 0.0f;
				startNode.hCost = GetHeuristic(startX, startY, endX, endY);
				m_OpenList.push(&startNode);

				while (!m_OpenList.empty())
				{
					PathNode* currentNode = m_OpenList.top();
					m_OpenList.pop();

					if (currentNode->x == endX && currentNode->y == endY)
					{
						// Reconstruction
					}

					currentNode->isClosed = true;

					for (int neighborX = -1; neighborX <= 1; neighborX++)
					{
						for (int neighborY = -1; neighborY <= 1; neighborY++)
						{
							if (neighborX == 0 && neighborY == 0) continue;

							int checkX = currentNode->x + neighborX;
							int checkY = currentNode->y + neighborY;

							if (_grid->IsInBounds(checkX, checkY))
							{
								int neighborIndex = checkY * width + checkX;
								PathNode& neighbor = m_NodePool[neighborIndex];

								if (neighbor.isClosed) continue;

								int moveX = std::abs(neighborX); 
								int moveY = std::abs(neighborY);
								float distanceStep = 0.0f;

								if (moveX + moveY == 2)
									distanceStep = 1.414f;
								else
									distanceStep = 1.0f;

								float influenceCost = GetInfluenceCost(checkX, checkY, _grid, _profile);
								float totalCost = currentNode->gCost + influenceCost + distanceStep;
							}
						}
					}
				}
			}
		}
	}

	float Pathfinding::GetHeuristic(int _x1, int _y1, int _x2, int _y2)
	{
		float dx = static_cast<float>(std::abs(_x2 - _x1));
		float dy = static_cast<float>(std::abs(_y2 - _y1));
		return (dx + dy) + (1.414f - 2.0f) * std::min(dx, dy);
	}

	float Pathfinding::GetInfluenceCost(int _x, int _y, Core::Components::InfluenceGrid* _grid, const NavProfile& _profile)
	{
		float influenceCost = 0.0f;

		if (_grid->IsInBounds(_x, _y))
		{
			int index = _grid->GetIndex(_x, _y);
			const InfluenceCell& cell = _grid->GetCells()[index];

			size_t numChannels = std::min(cell.channels.size(), _profile.influenceWeights.size());
			for (size_t i = 0; i< numChannels; i++)
			{
				influenceCost += cell.channels[i] * _profile.influenceWeights[i];
			}
		}

		return influenceCost;
	}

	void Pathfinding::ResetPool(int _width, int _height)
	{
		size_t totalSize = (size_t)_width * _height;
		if (m_NodePool.size() != totalSize)
			m_NodePool.resize(totalSize);

		for (PathNode& node : m_NodePool)
		{
			node.Reset();
		}

		m_OpenList = std::priority_queue<PathNode*, std::vector<PathNode*>, NodeComparator>();
	}
}