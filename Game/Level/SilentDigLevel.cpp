#include "SilentDigLevel.h"
#include "Util/BSPGenerator.h"
#include "Render/Renderer.h"
#include "../Actor/Player.h"
#include "../Actor/Wall.h"
#include "../Actor/Ground.h"

void SilentDigLevel::BeginPlay()
{
	Level::BeginPlay();
	CreateWorld();
}

bool SilentDigLevel::CanMove(const Wanted::Vector2& playerPosition, const Wanted::Vector2& nextPosition)
{
	int x = (int)nextPosition.x;
	int y = (int)nextPosition.y;

	// 1. Boundary check
	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight)
	{
		return false;
	}

	// 2. Wall check
	if (map[y][x] == TileType::Ground)
	{
		return false;
	}

	return true;
}

void SilentDigLevel::CreateWorld()
{
	// 1. Initialize: Fill with Ground
	map.assign(mapHeight, std::vector<TileType>(mapWidth, TileType::Ground));

	// 2. Outer Walls
	for (int x = 0; x < mapWidth; ++x)
	{
		map[0][x] = TileType::Wall;
		map[mapHeight - 1][x] = TileType::Wall;
	}

	for (int y = 0; y < mapHeight; ++y) {
		map[y][0] = TileType::Wall;
		map[y][mapWidth - 1] = TileType::Wall;
	}


	// 3. BSP Room Generation
	BSPGenerator bsp(12);
	bsp.Generate(mapWidth, mapHeight, 4);
	for (auto* region : bsp.GetLeafRegions())
	{
	    for (int y = region->roomY; y < region->roomY + region->roomH; ++y)
	         for (int x = region->roomX; x < region->roomX + region->roomW; ++x)
	             map[y][x] = TileType::Empty;
	}
	
	// 4. Actor Spawning
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			Wanted::Vector2 pos((float)x, (float)y);
			switch (map[y][x])
			{
			case TileType::Wall: AddNewActor(new Wall(pos)); break;
			case TileType::Ground: AddNewActor(new Ground(pos)); break;
			case TileType::Empty: break;
			}
		}
	}
	
	// 5. Player Spawning
	if (!bsp.GetLeafRegions().empty())
	{
	    auto* startRoom = bsp.GetLeafRegions()[0];

		float spawnX = (float)startRoom->roomX + (startRoom->roomW / 2.0f);
		float spawnY = (float)startRoom->roomY + (startRoom->roomH / 2.0f);
		Wanted::Vector2 spawnPos(spawnX, spawnY);
	    player = new Player(spawnPos);
	    AddNewActor(player);
	}
}

void SilentDigLevel::Tick(float deltaTime)
{
	Level::Tick(deltaTime);

	if (player)
	{
		Wanted::Renderer::Get().SetCameraPosition(player->GetPosition());
	}
}
