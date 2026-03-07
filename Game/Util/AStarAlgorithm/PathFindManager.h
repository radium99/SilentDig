#pragma once
#include <vector>
#include "Math/Vector2.h"
#include "../../Interface/IMapPathfinder.h"
#include "AStar.h"

class PathFindManager
{
public:
	// Static function for global access
	static std::vector<Wanted::Vector2> FindPath(
		const IMapPathfinder& map,
		const Wanted::Vector2& startPos,
		const Wanted::Vector2& goalPos
	)
	{
		AStar astar;
		return astar.FindPath(startPos, goalPos, map);
	}
};
