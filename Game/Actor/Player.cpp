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
	: super("P", position, Color::Red)
{
	// �׸��� �켱���� ���� ����.
	sortingOrder = 10;
}

void Player::BeginPlay()
{
	// ���� �Լ� ȣ��.
	// C++�� �θ��Լ� ����Ű�� �����Ͱ� ����.
	Actor::BeginPlay();

	//std::cout << "TestActor::BeginPlay().\n";
}

void Player::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// ESCŰ ó��.
	if (Wanted::Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// �޴� Ȱ��ȭ.
		Game::Get().ToggleMenu();
		return;
	}

	// QŰ ����.
	if (Wanted::Input::Get().GetKeyDown('Q'))
	{
		// Todo: ���� ���� ���� ��û.
		Wanted::Engine::Get().QuitEngine();
	}

	// �����̽��� �ڽ� ����.
	// vk->virtual key.
	if (Input::Get().GetKeyDown(VK_SPACE))
	{
		// �ڽ� ����.
		if (owner)
		{
			owner->AddNewActor(new Box(GetPosition()));
		}
	}

	// �������̽� Ȯ��.
	// 인터페이스 확인.
	ICanPlayerMove* canPlayerMoveInterface = nullptr;

	// 인스턴스 확인 (null 확인).
	if (GetOwner())
	{
		// 인터페이스로 형변환.
		canPlayerMoveInterface = dynamic_cast<ICanPlayerMove*>(GetOwner());
	}

	// 이동.
	if (Input::Get().GetKeyDown(VK_RIGHT) && GetPosition().x < 100)
	{
		// 이동 가능 여부 판단.
		Vector2 newPosition(GetPosition().x + 1, GetPosition().y);
		if (canPlayerMoveInterface && canPlayerMoveInterface->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_LEFT) && GetPosition().x > 0)
	{
		// 이동 가능 여부 판단.
		Vector2 newPosition(GetPosition().x - 1, GetPosition().y);
		if (canPlayerMoveInterface && canPlayerMoveInterface->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_DOWN) && GetPosition().y < 100)
	{
		// 이동 가능 여부 판단.
		Vector2 newPosition(GetPosition().x, GetPosition().y + 1);
		if (canPlayerMoveInterface && canPlayerMoveInterface->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}

	if (Input::Get().GetKeyDown(VK_UP) && GetPosition().y > 0)
	{
		// 이동 가능 여부 판단.
		Vector2 newPosition(GetPosition().x, GetPosition().y - 1);
		if (canPlayerMoveInterface && canPlayerMoveInterface->CanMove(GetPosition(), newPosition))
		{
			SetPosition(newPosition);
		}
	}
}

void Player::Draw()
{
	Actor::Draw();
}
