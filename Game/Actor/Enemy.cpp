#include "Enemy.h"
#include "Level/Level.h"
#include "Level/SilentDigLevel.h"
#include "Interface/ICanPlayerMove.h"
#include "../Engine/Render/Renderer.h"
//#include <cstdlib>




Enemy::Enemy(const Vector2& position)
	: super("E", position, Color::YELLOW)
{
	// 플레이어를 잡는 포지션이므로 플레이어 위로 덮히도록 함. (player는 10)
	sortingOrder = 11;

	lookDir = Vector2(0,1); 
	viewDistance = 10.0f; // 시야 거리 10칸.
	viewAngle = 90.0f;
}


void Enemy::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	moveTimer += deltaTime;
	rePathTimer += deltaTime;
	if (moveTimer >= moveInterval)
	{
		moveTimer = 0.0f;

		// 경로가 없거나, 목적지까지 갔다면 새로 요청.
		// Todo: 시각화가 끝난 후 진행해야 함. 적이 특정 이벤트가 발생했을 때, 경로탐색 함수를 실행하도록 수정해야 함. (현재는 시작하자마자 경로 탐색한다.)
		switch (state)
		{
		case EnemyState::IDLE:
			break;
		case EnemyState::SUSPICIOUS:
			break;
		case EnemyState::ANGRY:
			// 이미 경로가 있고 아직 이동 중인 경우 새로운 경로 요청 x.
			if (currentPath.empty() || pathIndex >= currentPath.size())
			{
				// 경로 요청.
				RequestNewPath();
			}

			// 5. 경로를 한 칸씩 읽으며 이동.
			if (pathIndex < currentPath.size())
			{
				Vector2 nextPos = currentPath[pathIndex];
				Vector2 currentPos = GetPosition();

				lookDir = nextPos - currentPos;
				lookDir.Normalize();
				SetPosition(currentPath[pathIndex]);
				pathIndex++;    
			
			}
			
		}
		
		
		//if (currentPath.empty() || pathIndex >= currentPath.size())
		//{
		//	RequestNewPath();
		//}

		// 5. 경로를 한 칸씩 읽으며 이동.
		//if (pathIndex < currentPath.size())
		//{
		//	SetPosition(currentPath[pathIndex]);
		//	pathIndex++;
		//}
		
		// 
	

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
	if (state == EnemyState::IDLE) { color = Color::BLUE; }
	if (state == EnemyState::SUSPICIOUS) { color = Color::YELLOW; }
	if (state == EnemyState::ANGRY) { color = Color::RED; }

	// 경로 시각화.
	// 시각화 활성화 && 현재 경로가 존재한 경우.
	if (GameSettings::ShowPath && !currentPath.empty())
	{
		for (int i = pathIndex; i < currentPath.size(); ++i)
		{
			Renderer::Get().Submit("*", currentPath[i], Wanted::Color::DARKRED, 20);
		}
	}
}


void Enemy::RequestNewPath()
{
	// 쿨타임 중이면 요청 거절.
	if (rePathTimer < rePathInterval)
	{
		return;
	}

	// 자신이 속한 레벨 객체(level*)를 다운 캐스팅하여 SilentDigLevel*로 가져옴.
	auto* level = dynamic_cast<SilentDigLevel*>(GetOwner());
	if (!level) return;

	if (level->IsPlayerDead())
	{
		return;
	}
	// 플레이어 위치와 맵 인터페이스 획득.
	Wanted::Vector2 playerPos = level->GetPlayerPosition(); // SilentDigLevel의 함수 GetPlayerPosition 호출.
	const IMapPathfinder& map = *level;

	lookDir = playerPos - GetPosition();
	lookDir.Normalize();

	// PathFindManager의 FindePath 함수는 전역 함수(static)이기 때문에 객체없이 어디에서든 사용가능하다.
	currentPath = PathFindManager::FindPath(map, GetPosition(), playerPos);
	pathIndex = 0;
	rePathTimer = 0.0f; // 타이머 초기화.
}

void Enemy::OnHearNoise(Vector2 location, float intensity)
{
	// 소음수치를 어그로 게이치에 중첩시킴.
	aggroGauge += intensity;
	
	// 이미 화난 상태라면 게이지 수치에 상관없이 추격 유지.
	if (state == EnemyState::ANGRY) return;

	// 어그로 게이지가 임계치를 넘을 경우 경로 탐색 요청.(게이지가 넘었을 때만 최초 1회 경로 탐색 수행.)
	if (aggroGauge >= guageThreshold && state != EnemyState::ANGRY)
	{
		state = EnemyState::ANGRY;
		RequestNewPath(); // 상태가 바뀔 때만 즉시 경로 갱신.
	}
	else if (aggroGauge > 35)
	{
		state = EnemyState::SUSPICIOUS;
	}
	else
	{
		state = EnemyState::IDLE;
	}

}

bool Enemy::CanSeePlayer(const Wanted::Vector2& playerPos, const IMapPathfinder& map)
{
	Wanted::Vector2 myPos = GetPosition();
	Wanted::Vector2 diff = playerPos - myPos;
	float distance = diff.Length();

	if (distance > viewDistance) return false;

	diff.Normalize();

	float dot = lookDir.Dot(diff);
	float angle = cosf(viewAngle * 0.5f * (3.14159f / 180.0f));
	if (dot < angle) return false;

	for (float d = 0.5f; d < distance; d += 0.5f) {
		Wanted::Vector2 checkPos = myPos + (diff * d);
		if (map.GetTileAt((int)checkPos.x, (int)checkPos.y) == TileType::Wall) {
			return false;
		}
	}

	return true;
}