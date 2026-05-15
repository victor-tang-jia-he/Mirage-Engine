#pragma once 
#include <queue>
#include <vector3.h>

namespace MirageEngine::Core::Math
{
	struct Vector3;
}

namespace MirageEngine::Core::Components
{
	class InfluenceGrid;
}

namespace MirageEngine
{
	using namespace Core::Components;

	namespace AI
	{
		using Vector3 = Core::Math::Vector3;

		struct PathNode
		{
			// Position in the grid
			int x, y;
			// Initial cost from start node to this node
			float GCost = 0.0f;
			// Heuristic cost from this node to the en node
			float HCost = 0.0f;
			// Total cost
			float FCost() const { return GCost + HCost; }
			// Index of the parent for path reconstruction
			int ParentIndex = -1;

			bool bIsClosed = false;
			bool bIsOpen = false;

			void Reset() 
			{
				GCost = 0; HCost = 0;
				ParentIndex = -1;
				bIsClosed = false;
				bIsOpen = false;
			}
		};

		struct NavProfile
		{
			// Weight for each channel of the nav grid
			std::vector<float> InfluenceWeights;
			bool CanWalkDiagonal = true;
		};

		struct NodeComparator 
		{
			bool operator()(const PathNode* a, const PathNode* b) 
			{
				return a->FCost() > b->FCost(); 
			}
		};

		enum class Pathstatus
		{
			SEARCHING,
			FOUND,
			FAILED_NO_PATH,
			FAILED_OUT_OF_BOUNDS
		};

		class Pathfinding
		{
		private:
			std::vector<PathNode> m_NodePool;
			std::priority_queue<PathNode*, std::vector<PathNode*>, NodeComparator> m_OpenList;

		public:
			std::vector<Vector3> FindPath(Vector3 _start, Vector3 _end, InfluenceGrid* _grid, const NavProfile& _profile);

		private:
			float GetHeuristic(int _x1, int _y1, int _x2, int _y2);
			float GetInfluenceCost(int _x, int _y, Core::Components::InfluenceGrid* _grid, const NavProfile& _profile);
			void ResetPool(int _width, int _height);
		};
	}
}