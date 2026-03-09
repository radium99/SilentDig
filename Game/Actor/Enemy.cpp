#include "Enemy.h"
#include "Level/Level.h"
#include "Level/SilentDigLevel.h"
#include "Interface/ICanPlayerMove.h"
#include "../Engine/Render/Renderer.h"
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
		// Todo: 시각화가 끝난 후 진행해야 함. 적이 특정 이벤트가 발생했을 때, 경로탐색 함수를 실행하도록 수정해야 함. (현재는 시작하자마자 경로 탐색한다.)
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

void Enemy::Draw()
{
	Actor::Draw();

	// 시각화 활성화 && 현재 경로가 존재한 경우.
	if (GameSettings::ShowPath && !currentPath.empty())
	{
		for (int i = pathIndex; i < currentPath.size(); ++i)
		{
			Renderer::Get().Submit("*", currentPath[i], Wanted::Color::Red, 20);
		}
	}
}


void Enemy::RequestNewPath()
{
	// 자신이 속한 레벨 객체(level*)를 다운 캐스팅하여 SilentDigLevel*로 가져옴.
	auto* level = dynamic_cast<SilentDigLevel*>(GetOwner());
	if (!level) return;

	// 플레이어 위치와 맵 인터페이스 획득.
	Wanted::Vector2 playerPos = level->GetPlayerPosition(); // SilentDigLevel의 함수 GetPlayerPosition 호출.
	const IMapPathfinder& map = *level;

	// PathFindManager의 FindePath 함수는 전역 함수(static)이기 때문에 객체없이 어디에서든 사용가능하다.
	currentPath = PathFindManager::FindPath(map, GetPosition(), playerPos);
	pathIndex = 0;
}
