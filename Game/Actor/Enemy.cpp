#include "Enemy.h"
#include "Level/Level.h"
#include "Level/SilentDigLevel.h"
#include "Interface/ICanPlayerMove.h"
//#include <cstdlib>




Enemy::Enemy(const Vector2& position)
	: super("E", position, Color::Yellow)
{
	// 플레이어를 잡는 포지션이므로 플레이어 위로 덮히도록 함. (player는 10)
	sortingOrder = 11;
}


void Enemy::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	moveTimer += deltaTime;
	if (moveTimer >= moveInterval)
	{
		moveTimer = 0.0f;

		// 경로가 없거나, 목적지까지 갔다면 새로 요청.
		if (currentPath.empty() || pathIndex >= currentPath.size())
		{
			RequestNewPath();
		}

		// 5. 경로를 한 칸씩 읽으며 이동.
		if (pathIndex < currentPath.size())
		{
			SetPosition(currentPath[pathIndex]);
			pathIndex++;
		}
		//int direction = rand() % 4;
		//Wanted::Vector2 nextPos = GetPosition();

		//switch (direction)
		//{
		//case 0: nextPos.y -= 1; break;
		//case 1: nextPos.y += 1; break;
		//case 2: nextPos.x -= 1; break;
		//case 3: nextPos.x += 1; break;
		//}

		//// 충돌 체크 (범위 검사 포함)
		//if (GetOwner())
		//{
		//	auto* moveInterface = dynamic_cast<ICanPlayerMove*>(GetOwner());

		//	if (moveInterface && moveInterface->CanMove(GetPosition(), nextPos))
		//	{
		//		SetPosition(nextPos);
		//	}
		//}
	}

}

//void Enemy::PathChasing()
//{
//	//1. 요청: 플레이어 위치와 map정보를 Level에게 달라고 요청.
//	SilentDigLevel* level = 
//}

void Enemy::RequestNewPath()
{

	auto* level = dynamic_cast<SilentDigLevel*>(GetOwner());
	if (!level) return;

	// 플레이어 위치와 맵 인터페이스 획득.
	Wanted::Vector2 playerPos = level->GetPlayerPosition();
	const IMapPathfinder& map = *level;

	currentPath = PathFindManager::FindPath(map, GetPosition(), playerPos);
	pathIndex = 0;
}


//void Enemy::Draw()
//{
//	Actor::Draw();
//
//}
