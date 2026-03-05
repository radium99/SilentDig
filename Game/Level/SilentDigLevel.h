#pragma once
#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
#include <vector>
#include "Util/BSPGenerator.h"

namespace Wanted { class Actor; }

class SilentDigLevel : public Wanted::Level, public ICanPlayerMove
{
	RTTI_DECLARATIONS(SilentDigLevel, Level)

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition) override;

private:
	// BSP 활용 맵 생성 함수.
	void CreateWorld();

	Wanted::Actor* player = nullptr;

	// 맵 크기 설정 변수.
	int mapWidth = 40;
	int mapHeight = 40;

	// 실제 보여지는 맵 데이터를 담는 벡터.
	std::vector<std::vector<TileType>> map;
};