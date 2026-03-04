#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

class Enemy : public Actor
{
	RTTI_DECLARATIONS(Enemy, Actor)

public:
	Enemy(const Vector2& position);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
};
