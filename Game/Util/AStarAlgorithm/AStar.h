#pragma once

#include "Node.h"
#include <vector>
#include "../../Interface/IMapPathfinder.h"
#include "Math/Vector2.h"

template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

// =============================================
class AStar
{
	struct Direction
	{
		int x = 0;
		int y = 0;
		float cost = 0.0f;
	};

public:
	AStar();
	~AStar();

	std::vector<Wanted::Vector2> FindPath(
		const Wanted::Vector2& startPos, 
		const Wanted::Vector2& goalPos,
		const IMapPathfinder& map
	);

private:
	std::vector<Wanted::Vector2> ConstructPath(Node* goalNode);
	float CalculateHeuristic(Node* currentNode, const Wanted::Vector2& goalPos);
	bool IsInRange(int x, int y, const IMapPathfinder& map);
	bool HasVisited(int x, int y, float gCost);
	bool IsDestination(const Node* const node, const Wanted::Vector2& goalPos);

private:
	std::vector<Node*> openList;
	std::vector<Node*> closedList;
};