#include "Enemy.h"
#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
//#include <cstdlib>



Enemy::Enemy(const Vector2& position)
	: super("E", position, Color::Yellow)
{
	sortingOrder = 9;
}


void Enemy::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	moveTimer += deltaTime;
	if (moveTimer >= moveInterval)
	{
		moveTimer = 0.0f;
		int direction = rand() % 4;
		Wanted::Vector2 nextPos = GetPosition();

		switch (direction)
		{
		case 0: nextPos.y -= 1; break;
		case 1: nextPos.y += 1; break;
		case 2: nextPos.x -= 1; break;
		case 3: nextPos.x += 1; break;
		}

		// 충돌 체크 (범위 검사 포함)
		if (GetOwner())
		{
			auto* moveInterface = dynamic_cast<ICanPlayerMove*>(GetOwner());

			if (moveInterface && moveInterface->CanMove(GetPosition(), nextPos))
			{
				SetPosition(nextPos);
			}
		}
	}

}

//void Enemy::Draw()
//{
//	Actor::Draw();
//
//}
