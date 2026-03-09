#pragma once
#include "Actor/Actor.h"
// AStar 적용을 위함.
#include "Util/AStarAlgorithm/PathFindManager.h"
#include "../Config/Setting.h"

using namespace Wanted;


class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2& position);

	virtual void Tick(float deltaTime) override;

protected:
	//virtual void BeginPlay() override;
	virtual void Draw() override;

private:

	float moveTimer = 0.0f; // 이동 타이머 관련 변수.
	float moveInterval = 1.0f; // 일정 간격으로 한 번씩 이동 설정 변수.

	// 경로 리스트.
	std::vector<Vector2> currentPath;

	// 현재 추적 중인 경로 인덱스.
	int pathIndex = 0;

	void RequestNewPath();

	// 시작 위치 및 목표 위치.
	//Node* startNode = nullptr;
	//Node* goalNode = nullptr;
};
