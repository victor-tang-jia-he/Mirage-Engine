#include "AI/pathfinding.h"
#include "AI/influence_grid.h"
#include <stdlib.h>

namespace MirageEngine::AI
{
	std::vector<Vector3> Pathfinding::FindPath(Vector3 _start, Vector3 _end, InfluenceGrid* _grid, const NavProfile _profile)
	{

	}

	float Pathfinding::GetHeuristic(int _x1, int _y1, int _x2, int _y2)
	{
		float dx = static_cast<float>(std::abs(_x2 - _x1));
		float dy = static_cast<float>(std::abs(_y2 - _y1));
		return (dx + dy) + (1.414f - 2.0f) * std::min(dx, dy);
	}

	float Pathfinding::GetInfluenceCost(int _x, int _y, Core::Components::InfluenceGrid* _grid, const NavProfile& _profile)
	{

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