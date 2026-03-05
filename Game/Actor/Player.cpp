#include "Player.h"
#include "Core/Input.h"
#include "Engine/Engine.h"
#include "Actor/Box.h"
#include "Level/Level.h"
#include "Game/Game.h"
#include "Interface/ICanPlayerMove.h"
#include <iostream>
#include <Windows.h>

using namespace Wanted;

Player::Player(const Vector2& position)
	: super("P", position, Color::Green)
{
	sortingOrder = 10;
}

void Player::BeginPlay()
{
	Actor::BeginPlay();
	
	// 인터페이스 캐싱
	if (GetOwner())
	{
		canPlayerMoveInterface = dynamic_cast<ICanPlayerMove*>(GetOwner());
	}
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	if (Wanted::Input::Get().GetKeyDown(VK_ESCAPE))
	{
		Game::Get().ToggleMenu();
		return;
	}

	if (Wanted::Input::Get().GetKeyDown('Q'))
	{
		Wanted::Engine::Get().QuitEngine();
	}

	// 이동 처리
	Vector2 direction(0, 0);
	if (Input::Get().GetKeyDown(VK_RIGHT)) direction.x = 1;
	else if (Input::Get().GetKeyDown(VK_LEFT)) direction.x = -1;
	else if (Input::Get().GetKeyDown(VK_DOWN)) direction.y = 1;
	else if (Input::Get().GetKeyDown(VK_UP)) direction.y = -1;

	if (direction.x != 0 || direction.y != 0)
	{
		Vector2 newPosition = GetPosition() + direction;
		
		// 인터페이스가 유효할 때만 이동 검사 수행
		if (canPlayerMoveInterface)
		{
			if (canPlayerMoveInterface->CanMove(GetPosition(), newPosition))
			{
				SetPosition(newPosition);
			}
		}
		else
		{
			// 인터페이스가 없는 레벨이라면 기본적으로 이동 허용 (단순 경계 체크만)
			SetPosition(newPosition);
		}
	}
}

void Player::Draw()
{
	Actor::Draw();
}
