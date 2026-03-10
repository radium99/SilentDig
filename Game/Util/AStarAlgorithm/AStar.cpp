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
			int curX = (int)currentNode->position.x;
			int curY = (int)currentNode->position.y;
			int nextX = curX + dir.x;
			int nextY = curY + dir.y;

			// 대각 장애물 이동 불가 처리 (4방향 체크: 왼 외각 위, 오른 대각 위, 왼 대각 아래, 오른 대각 아래)
			// 대각선 이동인 경우에만 체크 (x와 y가 둘 다 변했다면 대각선임)
			if (dir.x != 0 && dir.y != 0)
			{
				// 이동하려는 칸의 양옆(길목)이 모두 막혀 있는지 확인
				// 현재 위치(curX, curY) 기준으로 x축 인접칸과 y축 인접칸을 검사
				
				if (!map.IsWalkable(curX + dir.x, curY) && !map.IsWalkable(curX, curY + dir.y))
				{
					continue; // 길목이 막혔으면 이동 불가.
				}

			}


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