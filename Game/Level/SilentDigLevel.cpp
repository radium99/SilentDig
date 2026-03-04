#include "SilentDigLevel.h"
#include "Util/BSPGenerator.h"
#include "Render/Renderer.h"
#include "../Actor/Player.h"
#include "../Actor/Wall.h"
#include "../Actor/Ground.h"

void SilentDigLevel::BeginPlay()
{
	Level::BeginPlay();
	if (player == nullptr)
	{
		CreateWorld();
	}
}

bool SilentDigLevel::CanMove(const Wanted::Vector2& playerPosition, const Wanted::Vector2& nextPosition)
{
	int x = (int)nextPosition.x;
	int y = (int)nextPosition.y;

	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;
	if (map[y][x] == TileType::Wall) return false;

	return true;
}

void SilentDigLevel::CreateWorld()
{
	mapWidth = 30;
	mapHeight = 30;
	map.assign(mapHeight, std::vector<TileType>(mapWidth, TileType::Wall));

	BSPGenerator bsp(8);
	bsp.Generate(mapWidth, mapHeight, 3);
	
	for (auto* region : bsp.GetLeafRegions())
	{
	    for (int y = region->roomY; y < region->roomY + region->roomH; ++y)
	         for (int x = region->roomX; x < region->roomX + region->roomW; ++x)
	             map[y][x] = TileType::Empty;
	}

	if (player == nullptr && !bsp.GetLeafRegions().empty())
	{
	    auto* startRoom = bsp.GetLeafRegions()[0];
		float spawnX = (float)startRoom->roomX + (startRoom->roomW / 2.0f);
		float spawnY = (float)startRoom->roomY + (startRoom->roomH / 2.0f);
		
	    player = new Player(Wanted::Vector2(spawnX, spawnY));
	    AddNewActor(player);
	}
}

void SilentDigLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	if (player)
	{
		// 카메라가 플레이어를 추적하도록 엔진에 알림
		Wanted::Renderer::Get().SetCameraPosition(player->GetPosition());
	}
}

void SilentDigLevel::Draw()
{
	// 1. 맵 직접 렌더링 (순수 세상 좌표만 전달)
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			const char* symbol = (map[y][x] == TileType::Wall) ? "#" : ".";
			Color color = (map[y][x] == TileType::Wall) ? Color::White : Color::Purple;
			
			// Renderer::Draw()가 내부적으로 카메라 변환을 수행하므로, 세상 좌표만 넘김
			Wanted::Renderer::Get().Submit(symbol, Vector2((float)x, (float)y), color, -1);
		}
	}

	// 2. 액터들 그리기
	Level::Draw();
}
