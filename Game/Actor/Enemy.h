#pragma once
#include "Actor/Actor.h"
// AStar 적용을 위함.
#include "Util/AStarAlgorithm/PathFindManager.h"
#include "../Config/Setting.h"

using namespace Wanted;

// 몹 상태.
enum class EnemyState {
	IDLE,
	SUSPICIOUS,
	ANGRY
};


class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	// 소음 이벤트리스너.
	void OnHearNoise(Vector2 location, float intensity);
	bool CanSeePlayer(const Wanted::Vector2& playerPos, const IMapPathfinder& map);

protected:
	//virtual void BeginPlay() override;
	
	// 경로를 시각화 하기 위한 함수.
	virtual void Draw() override;

private:

	// 경로 요청 함수.
	void RequestNewPath();

	// 경로 리스트.
	std::vector<Vector2> currentPath;

	// 현재 추적 중인 경로 인덱스.
	int pathIndex = 0;

	float moveTimer = 0.0f; // 이동 타이머 관련 변수.
	float moveInterval = 0.2f; // 일정 간격으로 한 번씩 이동 설정 변수.

	float rePathTimer = 0.0f;
	float rePathInterval = 2.0f;

	// 몹의 상태. (기본 상태로 초기화)
	EnemyState state = EnemyState::IDLE;

	// 어그로 게이지
	float aggroGauge = 0.0f;
	float guageThreshold = 70.0f; // 임계치 설정.

	// 시야 범위
	float viewDistance = 15.0f;

	Vector2 lookDir; // 시야 거리 10칸.
	float viewAngle; 
};
