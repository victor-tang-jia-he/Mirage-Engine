#include <iostream>
#include <vector>
#include <vector3.h>
#include <queue>
#include <functional>

using namespace MirageEngine::Core::Math;

namespace MirageEngine::Core::Components
{
	class InfluenceGrid;
}

namespace MirageEngine
{
	using namespace Core::Components;

	namespace AI
	{
		struct PathNode
		{
			// Position in the grid
			int x, y;
			// Initial cost from start node to this node
			float gCost = 0.0f;
			// Heuristic cost from this node to the en node
			float hCost = 0.0f;
			// Total cost
			float fCost() const { return gCost + hCost; }
			// Index of the parent for path reconstruction
			int parentIndex = -1;

			bool isClosed = false;
			bool isOpen = false;

			void Reset() 
			{
				gCost = 0; hCost = 0;
				parentIndex = -1;
				isClosed = false;
				isOpen = false;
			}
		};

		struct NavProfile
		{
			// Weight for each channel of the nav grid
			std::vector<float> influenceWeights;
			bool canWalkDiagonal = true;
		};

		struct NodeComparator 
		{
			bool operator()(const PathNode* a, const PathNode* b) 
			{
				return a->fCost() > b->fCost(); 
			}
		};

		enum class Pathstatus
		{
			Searching,
			Found,
			Failed_NoPath,
			Failed_OutOfBounds
		};

		class Pathfinding
		{
		private:
			std::vector<PathNode> m_NodePool;
			std::priority_queue<PathNode*, std::vector<PathNode*>, NodeComparator> m_OpenList;

		public:
			std::vector<Vector3> FindPath(Vector3 _start, Vector3 _end, InfluenceGrid* _grid, const NavProfile _profile);
		private:
			float GetHeuristic(int _x1, int _y1, int _x2, int _y2);
			float GetInfluenceCost(int _x, int _y, Core::Components::InfluenceGrid* _grid, const NavProfile& _profile);
			void ResetPool(int _width, int _height);
		};
	}
}