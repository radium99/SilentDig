#pragma once

#include "Math/Vector2.h"

// 노드
struct Node
{
	// 좌표 값 저장 변수.
	Wanted::Vector2 position;

	// 비용 계산을 위한 변수.
	float gCost = 0.0f; //  g(n)
	float hCost = 0.0f; // h(n)
	float fCost = 0.0f; // f(n) =  gCost + hCost.

	// 부모 노드.
	Node* parentNode = nullptr;

	// 생성자.
	Node(int x, int y, Node* parent = nullptr)
		: position((float)x, (float)y), parentNode(parent)
	{
	}

	Node(const Wanted::Vector2& pos, Node* parent = nullptr)
		: position(pos), parentNode(parent)
	{
	}

	// 노드 비교 연산자.
	bool operator==(const Node& other) const
	{
		return (int)position.x == (int)other.position.x && (int)position.y == (int)other.position.y;
	}
};