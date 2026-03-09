#pragma once
#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
#include <vector>
#include "Util/BSPGenerator.h"
#include "Interface/IMapPathfinder.h"

namespace Wanted { class Actor; }

class SilentDigLevel : public Wanted::Level, public ICanPlayerMove, public IMapPathfinder
{
	RTTI_DECLARATIONS(SilentDigLevel, Level)

public:
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	virtual bool CanMove(
		const Wanted::Vector2& playerPosition,
		const Wanted::Vector2& nextPosition) override;

	// IMapPathfinder 함수 인스턴스화.
	virtual TileType GetTileAt(int x, int y) const override;
	virtual int GetWidth() const override { return mapWidth; }
	virtual int GetHeight() const override { return mapHeight; }
	virtual bool IsWalkable(int x, int y)const override {
		return map[y][x] != TileType::Wall;
	}
	Wanted::Vector2 GetPlayerPosition() const;
	// 적 생성 함수.
	void SpawnEnemy(BSPGenerator& bsp, const Wanted::Vector2& playerPos);

	// 경로 탐색 매니저에게 맵정보와 플레이어 위치를 넘

	// ============== 부과 컨텐츠 ==============================
	// 플레이어와 적 충돌 감지 및 처리 함수.
	void ProcessCollisionEnemyAndPlayer();


private:
	// BSP 활용 맵 생성 함수.
	void CreateWorld();

	Wanted::Actor* player = nullptr;
	Wanted::Actor* enemy = nullptr;

	// 맵 크기 설정 변수.
	int mapWidth = 60;
	int mapHeight = 60;

	// 실제 보여지는 맵 데이터를 담는 벡터.
	std::vector<std::vector<TileType>> map;


	// ============== 부과 컨텐츠 ==============================
	// 플레이어가 죽은 위치 (Draw에서 처리하기 위해 Tick에서 저장).
	Wanted::Vector2 playerDeadPosition;


	// 플레이어가 죽었는지 확인.
	bool isPlayerDead = false;

};