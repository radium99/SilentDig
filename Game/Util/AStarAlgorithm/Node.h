#pragma once

#include "Math/Vector2.h"

// .
struct Node
{
	// ġ.
	Wanted::Vector2 position;

	// .
	float gCost = 0.0f; //  ġ .
	float hCost = 0.0f; // ǥ ġ ߻  (޸ƽ).
	float fCost = 0.0f; // gCost + hCost.

	// θũ.
	Node* parentNode = nullptr;

	// .
	Node(int x, int y, Node* parent = nullptr)
		: position((float)x, (float)y), parentNode(parent)
	{
	}

	Node(const Wanted::Vector2& pos, Node* parent = nullptr)
		: position(pos), parentNode(parent)
	{
	}

	// ġ  .
	bool operator==(const Node& other) const
	{
		return (int)position.x == (int)other.position.x && (int)position.y == (int)other.position.y;
	}
};