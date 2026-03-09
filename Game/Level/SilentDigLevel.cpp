#include "SilentDigLevel.h"
#include "Render/Renderer.h"
#include "../Actor/Player.h"
#include "../Actor/Wall.h"
#include "../Actor/Ground.h"
#include "../Actor/Enemy.h"
#include "../Engine/Core/Input.h"
#include "../Config/Setting.h" // 경로 시각화 옵션.
//#include "../Engine/Engine/Engine.h"

void SilentDigLevel::BeginPlay()
{
	Level::BeginPlay();

	// 중복호출 방지: 처음에는 통과하여 CreateWorld()가 실행되면 hasCreateWorld가 true가 되고
	// 함수 실행이 끝나는 순간, player가 new Player를 할당받고 이후는 통과하지 못함.
	if ( !hasCreateWorld && player == nullptr)
	{
		CreateWorld();
	}
	else
	{
		// Todo: 다시 도전하겠냐는 문구 씬 있어야 함.
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
		
		// Todo: 확률적으로 소음 발생로직.
		if (rand() % 100 < 50)
		{
			NotifyNoiseToEnemies(Vector2(x, y), 50.0f);
		}
		
			//이동 못함.
		return false;
	}

	return true;
}
void SilentDigLevel::NotifyNoiseToEnemies(Vector2 noisePos, float intensity) 
{

	for (Wanted::Actor* const actor : actors)
	{
		if (actor->IsTypeOf<Enemy>())
		{
			float dist = (actor->GetPosition() - noisePos).Length(); //Length는 거리 구하는 공식.
			if (dist < 20.0f) // 소음이 들리는 최대 거리
			{
				// 거리에 따른 감쇄(멀수록 게이지가 적게 오름)
				float finalIntensity = intensity * (1.0f - (dist / 15.0f));
				actor->As<Enemy>()->Enemy::OnHearNoise(noisePos, finalIntensity);
			}
		}
	}
}

TileType SilentDigLevel::GetTileAt(int x, int y) const
{
	return map[y][x];
}

void SilentDigLevel::CreateWorld()
{
	hasCreateWorld = true;
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

	if (player != nullptr)
	{
		// 카메라가 플레이어를 추적하도록 엔진에 알림
		Wanted::Renderer::Get().SetCameraPosition(player->GetPosition());
	}
	else if (isPlayerDead)
	{
		Wanted::Renderer::Get().SetCameraPosition(playerDeadPosition);
	}

	// 경로 시각화 토글.
	if (Wanted::Input::Get().GetKeyDown(VK_F1))
	{
		GameSettings::ShowPath = !GameSettings::ShowPath;
	}

	ProcessCollisionEnemyAndPlayer();
}


Wanted::Vector2 SilentDigLevel::GetPlayerPosition() const
{
	if (player != nullptr) {
		return player->GetPosition();
	}

	return playerDeadPosition;
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
			const char* symbol = (map[y][x] == TileType::Wall) ? "#" : " "; // 확장 가능성: Wall말고 뚫리지 않는 TileType 추가 가능.
			Color color = (map[y][x] == TileType::Wall) ? Color::DARKYELLOW : Color::BLACK;
			
			// Renderer::Draw()가 내부적으로 카메라 변환을 수행하므로, 세상 좌표만 넘김
			Wanted::Renderer::Get().Submit(symbol, Vector2((float)x, (float)y), color, -1);
		}
	}

	// 2. 액터들 그리기
	Level::Draw();

	if (isPlayerDead)
	{
		// 플레이어 죽음 메시지 Renderer에 제출.
		Renderer::Get().Submit("!Dead!", playerDeadPosition);

		// 점수 보여주기.
		//ShowScore();

		// 화면에 바로 표시.
		//Renderer::Get().PresentImmediately();

		// 프로그램 정지.
		Sleep(2000);

		// 게임 종료.
		//Engine::Get().QuitEngine();
	}
}


void SilentDigLevel::ProcessCollisionEnemyAndPlayer()
{
	// 멤버 변수 player가 없거나 이미 죽었으면 처리할 필요 없음.
	if (!player || isPlayerDead)
	{
		return;
	}

	// 적 액터만 필터링.
	std::vector<Actor*> enemys;
	for (Actor* const actor : actors)
	{
		if (actor->IsTypeOf<Enemy>())
		{
			enemys.emplace_back(actor);
		}
	}

	if (enemys.empty())
	{
		return;
	}

	// 충돌 판정.
	for (Actor* const enemy : enemys)
	{
		// 멤버 변수 player와 충돌 검사
		if (enemy->TestIntersect(player))
		{
			// 플레이어 죽음 설정.
			isPlayerDead = true;

			// 죽은 위치 저장.
			playerDeadPosition = player->GetPosition();
			
			// 멤버 변수 player 파괴 및 nullptr 설정 (중요: 멤버 변수를 직접 제어)
			player->Destroy();
			player = nullptr; 
			break;
		}
	}   
}

bool SilentDigLevel::IsPlayerDead()
{     
	if (isPlayerDead)
	{
		return true;
	}
	return false;
}