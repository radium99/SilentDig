#include "MainMenu.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include <iostream>

MainMenu::MainMenu()
{
	// 메뉴 아이템 생성.
	items.emplace_back(new MenuItem(
		"Start Game",
		[]()
		{
			// 메뉴 토글 함수 호출.
			Game::Get().ToggleMenu();
		}
	));

	items.emplace_back(new MenuItem(
		"Quit Game",
		[]()
		{
			// 게임 종료.
			Game::Get().QuitEngine();
		}
	));
}

MainMenu::~MainMenu()
{
	// 메뉴 아이템 제거.
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화.
	items.clear();
}

void MainMenu::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 입력 처리 (방향키 위/아래키, 엔터키, ESC키).
	// 배열 길이.
	static int length = static_cast<int>(items.size());
	if (Input::Get().GetKeyDown(VK_UP))
	{
		// 인덱스 돌리기 (위 방향으로).
		currentIndex = (currentIndex - 1 + length) % length;
	}

	if (Input::Get().GetKeyDown(VK_DOWN))
	{
		// 인덱스 돌리기.
		currentIndex = (currentIndex + 1) % length;
	}

	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 메뉴 아이템이 저장한 함수 포인터 호출.
		items[currentIndex]->onSelected();
	}

	if (Input::Get().GetKeyDown(VK_ESCAPE))
	{
		// 메뉴 토글.
		Game::Get().ToggleMenu();

		// 인덱스 초기화.
		currentIndex = 0;
	}
}

void MainMenu::Draw()
{
	// 1. 거대한 ASCII 아트 출력 (Silent Dig)
	// 상단 여백을 위해 startY를 설정합니다.
	int artY = 1;

	// Effective C++ Tip: 텍스트 배열을 static으로 선언하여 매 프레임 생성을 방지합니다.
	static std::vector<std::string> titleArt = {
		R"(   _ .-') _      S I L E N T       )",
		R"(  ( (  OO) )                       )",
		R"(   \     .'_   ,-.-')   ,----.     )",
		R"(   ,`'--..._)  |  |OO) '  .-./-')  )",
		R"(   |  |  \  '  |  |  \ |  |_( O- ))",
		R"(   |  |   ' |  |  |(_/ |  | .--, \ )",
		R"(   |  |   / : ,|  |_.'(|  | '. (_/ )",
		R"(   |  '--'  /(_|  |    |  '--'  |  )",
		R"(   `-------'   `--'     `------'   )"
	};

	for (size_t i = 0; i < titleArt.size(); ++i)
	{
		Renderer::Get().Submit(titleArt[i].c_str(), Vector2(0, artY + i), Color::DARKYELLOW, 100,  true);
	}
	// 메뉴 아이템 출력.
	for (int ix = 0; ix < static_cast<int>(items.size()); ++ix)
	{
		// 아이템 색상 확인 (선택됐는지 여부).
		Color textColor =
			(ix == currentIndex) ? selectedColor : unselectedColor;

		Renderer::Get().Submit(
			items[ix]->text,
			Vector2(-7, 2 + ix),
			textColor
		);

		// 색상 설정.
		//Util::SetConsoleTextColor(textColor);

		// 텍스트 출력.
		//std::cout << items[ix]->text << "\n";
	}
	// 2. 메뉴 아이템 출력 (아트 아래쪽에 배치)
	// 아트가 9줄이므로, 여백을 포함해 11번 줄부터 아이템을 출력합니다.
	//int menuStartY = artY + static_cast<int>(titleArt.size()) + 2;

	//for (int ix = 0; ix < static_cast<int>(items.size()); ++ix)
	//{
	//	// 선택된 아이템은 강조 색상, 나머지는 기본 색상
	//	Color textColor = (ix == currentIndex) ? selectedColor : unselectedColor;

	//	// 선택된 아이템 앞에 포인터(>) 표시 추가 (선택지 가독성 향상)
	//	std::string prefix = (ix == currentIndex) ? "> " : "  " ;
	//	std::string menuText = prefix + items[ix]->text;

	//	Renderer::Get().Submit(
	//		menuText.c_str(),
	//		Vector2(10.0f, static_cast<float>(menuStartY + ix)), // X좌표를 10 정도로 밀어서 중앙 느낌을 줌
	//		textColor,
	//		100,
	//		true
	//	);
	//}
}
//void MainMenu::Draw()
//{
//	// 메뉴 제목 출력.
//	//Util::SetConsolePosition(Vector2::Zero);
//	//Util::SetConsoleTextColor(Color::White);
//	//
//	//// 텍스트 출력.
//	//std::cout << "Sokoban Game\n\n";
//
//	Renderer::Get().Submit("Silent Dig Game", Vector2::Zero, Color::DARKYELLOW);
//
//	// 메뉴 아이템 출력.
//	for (int ix = 0; ix < static_cast<int>(items.size()); ++ix)
//	{
//		// 아이템 색상 확인 (선택됐는지 여부).
//		Color textColor =
//			(ix == currentIndex) ? selectedColor : unselectedColor;
//
//		Renderer::Get().Submit(
//			items[ix]->text,
//			Vector2(0, 2 + ix),
//			textColor
//		);
//
//		// 색상 설정.
//		//Util::SetConsoleTextColor(textColor);
//
//		// 텍스트 출력.
//		//std::cout << items[ix]->text << "\n";
//	}
//}
