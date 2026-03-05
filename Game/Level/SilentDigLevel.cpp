#include "SilentDigLevel.h"
#include "Render/Renderer.h"
#include "../Actor/Player.h"
#include "../Actor/Wall.h"
#include "../Actor/Ground.h"
#include "../Actor/Enemy.h"

void SilentDigLevel::BeginPlay()
{
	//Level::BeginPlay();
	//CreateWorld();
	Level::BeginPlay();

	// 중복호출 방지: 처음에는 통과하여 CreateWorld()가 실행되고 끝나는 순간, player가 new Player를 할당받아서 이후는 통과하지 못함.
	if (player == nullptr)
	{
		CreateWorld();
	}
}

bool SilentDigLevel::CanMove(const Wanted::Vector2& playerPosition, const Wanted::Vector2& nextPosition)
{
	int x = (int)nextPosition.x;
	int y = (int)nextPosition.y;

	// 맵 데이터 자체의 유효성 검사.
	if (map.empty()) return false;

	if (x < 0 || x >= mapWidth || y < 0 || y >= mapHeight) return false;

	// 음수 좌표 및 경계 초과 검사 (vector 범위 밖으로 나가지 않도록)
	if (y < 0 || y >= (int)map.size() || x < 0 || x >= (int)map[0].size())
	{
		return false;
	}

	// 벽 충돌 검사: 이동하려는 위치의 맵 타일 유형이 벽일 경우.
	if (map[y][x] == TileType::Wall) 
	{
		// Dig. 해당 위치의 벽 파괴.
		map[y][x] = TileType::Empty; //effective c++ 관점에서 맞지 않음. (쿼리와 커멘드를 구분하자.)

		//이동 못함.
		return false;
	}

	return true;
}

void SilentDigLevel::CreateWorld()
{
	mapWidth = 60;
	mapHeight = 60;
	map.assign(mapHeight, std::vector<TileType>(mapWidth, TileType::Wall));

	BSPGenerator bsp(8);
	bsp.Generate(mapWidth, mapHeight, 3);
	
	// 빈 방 생성.
	bsp.CreateEmptyRoom(map);
	// 인접한 방끼리 통로 연결. (같은 부모노드를 가리키는 자식 노드들끼리)
	bsp.CreateTunnel(bsp.GetRoot(), map);

	// 플레이어를 맵 특정 위치에 생성.
	if (player == nullptr && !bsp.GetLeafRegions().empty())
	{
	    auto* startRoom = bsp.GetLeafRegions()[0]; // Todo: 플레이어 시작 위치를 0으로 하드 코딩하지 않고, rand()를 이용해 매번 바뀌도록 설정해야 함.
		float spawnX = (float)startRoom->roomX + (startRoom->roomW / 2.0f);
		float spawnY = (float)startRoom->roomY + (startRoom->roomH / 2.0f);
		
	    player = new Player(Wanted::Vector2(spawnX, spawnY));
	    AddNewActor(player);
	}

	SpawnEnemy(bsp, Wanted::Vector2(0, 0)); // Todo: 현재는 플레이어 위치를 가져오지 않고, 하드 코딩으로 0,0 전달하므로 플레이어 위치바꿔야 함.
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
void SilentDigLevel::SpawnEnemy(BSPGenerator& bsp, const Wanted::Vector2& playerPos)
{
	const auto& leafRegions = bsp.GetLeafRegions();
	if (leafRegions.size() > 1) {

		for (size_t i = 1; i < leafRegions.size(); ++i)
		{
			auto* region = leafRegions[i];

			float spawnX = (float)region->roomX + (region->roomW / 2.0f);
			float spawnY = (float)region->roomY + (region->roomH / 2.0f);

			AddNewActor(new Enemy(Wanted::Vector2(spawnX, spawnY)));
		}
		//for (int ix = 0; ix < leafRegions.size(); ++ix)
		//{
		//	// 플레이어를 맵 특정 위치에 생성.
		//	if (enemy == nullptr && !leafRegions.empty())
		//	{
		//		int random = max(1, rand()); // Todo: 플레이어 위치인 0 제외.
		//		auto* spawnRoom = leafRegions[random]; // Todo: 플레이어 시작 위치를 0으로 하드 코딩하지 않고, rand()를 이용해 매번 바뀌도록 설정해야 함.
		//		float spawnX = (float)spawnRoom->roomX + (spawnRoom->roomW / 2.0f);
		//		float spawnY = (float)spawnRoom->roomY + (spawnRoom->roomH / 2.0f);

		//		enemy = new Enemy(Wanted::Vector2(spawnX, spawnY));
		//		AddNewActor(enemy);
		//	}
		//}
	}

}
void SilentDigLevel::Draw()
{
	if (map.empty()) return;
	Vector2 screenSize = Wanted::Renderer::Get().GetScreenSize();
	Vector2 camPos = Wanted::Renderer::Get().GetCameraPosition();
	// 1. 맵 직접 렌더링 (순수 세상 좌표만 전달)
	for (int y = 0; y < mapHeight; ++y)
	{
		for (int x = 0; x < mapWidth; ++x)
		{
			const char* symbol = (map[y][x] == TileType::Wall) ? "#" : "."; // 확장 가능성: Wall말고 뚫리지 않는 TileType 추가 가능.
			Color color = (map[y][x] == TileType::Wall) ? Color::White : Color::Maganta;
			
			// Renderer::Draw()가 내부적으로 카메라 변환을 수행하므로, 세상 좌표만 넘김
			Wanted::Renderer::Get().Submit(symbol, Vector2((float)x, (float)y), color, -1);
		}
	}

	// 2. 액터들 그리기
	Level::Draw();
}
