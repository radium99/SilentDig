#include "AStar.h"
#include <cmath>
#include <algorithm>

AStar::AStar() {}

AStar::~AStar()
{
	for (Node* node : openList) delete node;
	for (Node* node : closedList) delete node;
	openList.clear();
	closedList.clear();
}

std::vector<Wanted::Vector2> AStar::FindPath(const Wanted::Vector2& startPos, const Wanted::Vector2& goalPos, const IMapPathfinder& map)
{
	// 초기화
	for (Node* node : openList) delete node;
	for (Node* node : closedList) delete node;
	openList.clear();
	closedList.clear();

	Node* startNode = new Node(startPos);
	startNode->gCost = 0; // g(n)
	startNode->hCost = CalculateHeuristic(startNode, goalPos); // h(n)
	startNode->fCost = startNode->gCost + startNode->hCost; // f(n) = g(n) + h(n)
	openList.push_back(startNode);

	// x좌표, y좌표, 길이
	std::vector<Direction> directions = {
		{ 0, 1, 1.0f }, { 0, -1, 1.0f }, { 1, 0, 1.0f }, { -1, 0, 1.0f },
		{ 1, 1, 1.414f }, { 1, -1, 1.414f }, { -1, 1, 1.414f }, { -1, -1, 1.414f }
	};

	while (!openList.empty())
	{
		Node* currentNode = openList[0];
		int currentIndex = 0;
		for (int i = 1; i < (int)openList.size(); ++i)
		{
			if (openList[i]->fCost < currentNode->fCost)
			{
				currentNode = openList[i];
				currentIndex = i;
			}
		}

		// 목적지 도착 시, 최종 경로 반환.
		if (IsDestination(currentNode, goalPos))
		{
			return ConstructPath(currentNode);
		}

		openList.erase(openList.begin() + currentIndex);
		closedList.push_back(currentNode);

		for (const auto& dir : directions)
		{
			int nextX = (int)currentNode->position.x + dir.x;
			int nextY = (int)currentNode->position.y + dir.y;

			if (!IsInRange(nextX, nextY, map) || !map.IsWalkable(nextX, nextY))
				continue;

			float newGCost = currentNode->gCost + dir.cost;

			if (HasVisited(nextX, nextY, newGCost))
				continue;

			Node* neighbor = new Node(nextX, nextY, currentNode);
			neighbor->gCost = newGCost;
			neighbor->hCost = CalculateHeuristic(neighbor, goalPos);
			neighbor->fCost = neighbor->gCost + neighbor->hCost;

			bool inOpenList = false;
			for (Node* openNode : openList)
			{
				if (*openNode == *neighbor)
				{
					if (neighbor->gCost < openNode->gCost)
					{
						openNode->gCost = neighbor->gCost;
						openNode->fCost = neighbor->fCost;
						openNode->parentNode = neighbor->parentNode;
					}
					inOpenList = true;
					break;
				}
			}

			if (!inOpenList)
			{
				openList.push_back(neighbor);
			}
			else
			{
				delete neighbor;
			}
		}
	}

	return {};
}

std::vector<Wanted::Vector2> AStar::ConstructPath(Node* goalNode)
{
	std::vector<Wanted::Vector2> path;
	Node* current = goalNode;
	while (current != nullptr)
	{
		// 현재 노드가 가지고 있는 좌표 값 경로에 추가.
		path.push_back(current->position);

		// 현재 노드의 부모 노드로 이동.
		current = current->parentNode;
	}
	std::reverse(path.begin(), path.end());
	return path;
}

float AStar::CalculateHeuristic(Node* currentNode, const Wanted::Vector2& goalPos)
{
	return std::sqrt(std::pow(currentNode->position.x - goalPos.x, 2) + std::pow(currentNode->position.y - goalPos.y, 2));
}

bool AStar::IsInRange(int x, int y, const IMapPathfinder& map)
{
	return x >= 0 && x < map.GetWidth() && y >= 0 && y < map.GetHeight();
}

bool AStar::HasVisited(int x, int y, float gCost)
{
	for (Node* node : closedList)
	{
		if ((int)node->position.x == x && (int)node->position.y == y)
		{
			return true;
		}
	}
	return false;
}

bool AStar::IsDestination(const Node* const node, const Wanted::Vector2& goalPos)
{
	return (int)node->position.x == (int)goalPos.x && (int)node->position.y == (int)goalPos.y;
}