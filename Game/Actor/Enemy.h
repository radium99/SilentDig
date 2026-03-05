#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2& position);

	virtual void Tick(float deltaTime) override;
//protected:
//	//virtual void BeginPlay() override;
//	virtual void Draw() override;
//
	float moveTimer = 0.0f; // 이동 타이머 관련 변수.
	float moveInterval = 1.0f; // 일정 간격으로 한 번씩 이동 설정 변수.


};
