#pragma once
#include "Level/Level.h"
#include "Interface/ICanPlayerMove.h"
#include <vector>
#include "Util/BSPGenerator.h"
#include "Interface/IMapPathfinder.h"
#include "Engine/Engine.h"

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
	virtual bool IsWalkable(int x, int y)const override;
	Wanted::Vector2 GetPlayerPosition() const;
	// 적 생성 함수.
	void SpawnEnemy(BSPGenerator& bsp, const Wanted::Vector2& playerPos);

	// 거리 기반 노이즈 수치 전달.
	void NotifyNoiseToEnemies(Wanted::Vector2 noisePos, float intensity);

	// ============== 부과 컨텐츠 ==============================
	// 플레이어와 적 충돌 감지 및 처리 함수.
	void ProcessCollisionEnemyAndPlayer();

	bool IsPlayerDead();

	// 탈출 계단 생성 함수.
	void SpawnStair(BSPGenerator& bsp, const Wanted::Vector2& playerPos);

	// 플레이어와 계단 충돌 감지 및 처리 함수.
	void IsPlayerGoalIn();

	// 다음 층 이동 전, 기존 액터들 삭제 처리 함수.
	void DestroyActors();

	// 현재 층을 보여주는 함수.
	void ShowScore();

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

	bool hasCreateWorld = false;

	// 게임을 클리어 했는지 확인.
	bool isGameClear = false;

	int floor = 1; // 현재 던전 층 변수.
	int maxFloor = 1; // 마지막 층 설정 변수.

	// 층 문자열.
	char floorString[128];
};