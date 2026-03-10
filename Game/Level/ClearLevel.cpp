#include "ClearLevel.h"
#include "Render/Renderer.h"
#include "Core/Input.h"
#include "Game/Game.h"


ClearLevel::ClearLevel()
{
    // 아스키 아트 구성 (STAGE CLEAR)
    asciiArt.push_back("  ____ _____   _    ____ _____    ____ _     _____   _    ____  ");
    asciiArt.push_back(" / ___|_   _|/ \\  / ___| ____|  / ___| |    | ____| / \\  |  _ \\ ");
    asciiArt.push_back(" \\___ \\ | | / _ \\| |  _|  _|    | |   | |    |  _|  / _ \\ | |_) |");
    asciiArt.push_back("  ___) || |/ ___ \\ |_| | |___   | |___| |___| |___ / ___ \\|  _ < ");
    asciiArt.push_back(" |____/ |_/_/   \\_\\____|_____|  \\____|_____|_____/_/   \\_\\_| \\_\\");
}

void ClearLevel::BeginPlay()
{
    Level::BeginPlay();

    Wanted::Renderer::Get().SetCameraPosition(Wanted::Vector2::Zero);
}

void ClearLevel::Tick(float deltaTime)
{
    Level::Tick(deltaTime);

    // 안내 메시지 깜빡임 효과를 위한 타이머 업데이트 (0.5초 간격)
    timer += deltaTime;
    if (timer >= 0.5f)
    {
        showPressEnter = !showPressEnter;
        timer = 0.0f;
    }

    // 엔터 키 입력 시 메인 메뉴(또는 메뉴 상태)로 전환
    if (Wanted::Input::Get().GetKeyDown(VK_RETURN))
    {
        // 실무 팁: Game 클래스 내부에 정의된 메뉴 전환 로직 호출
        //Game::Get().ToggleMenu();
        Engine::Get().QuitEngine();
    }
}

void ClearLevel::Draw()
{
    // 1. 거대한 ASCII 아트 출력 (Silent Dig)
// 상단 여백을 위해 startY를 설정합니다.
    int artY = 1;

    // Effective C++ Tip: 텍스트 배열을 static으로 선언하여 매 프레임 생성을 방지합니다.
    static std::vector<std::string> titleArt = {
        R"(     ('-.       .-') _  _ .-') _    )",
        R"(   _(  OO)     ( OO ) )( (  OO) )   )",
        R"(  (,------.,--./ ,--,'  \     .'_   )",
        R"(   |  .---'|   \ |  |\  ,`'--..._)  )",
        R"(   |  |    |    \|  | ) |  |  \  '  )",
        R"(  (|  '--. |  .     |/  |  |   ' |  )",
        R"(   |  .--' |  |\    |   |  |   / :  )",
        R"(   |  `---.|  | \   |   |  '--'  /  )",
        R"(   `------'`--'  `--'   `-------'   )",
        R"(       THANK YOU FOR PLAYING!       )"
    };

    for (size_t i = 0; i < titleArt.size(); ++i)
    {
        Renderer::Get().Submit(titleArt[i].c_str(), Vector2(0, artY + i), Color::DARKYELLOW, 100, true);
    }
    //// 1. 아스키 아트 출력 (중앙 정렬을 고려한 좌표 설정)
    //int startY = 5;
    //for (size_t i = 0; i < asciiArt.size(); ++i)
    //{
    //    Wanted::Renderer::Get().Submit(
    //        asciiArt[i].c_str(),
    //        Wanted::Vector2(10.0f, static_cast<float>(startY + i)),
    //        Wanted::Color::GREEN,
    //        100,
    //        true
    //    );
    //}

    // 2. 안내 메시지 출력 (깜빡임 효과 적용)
    if (showPressEnter)
    {
        Wanted::Renderer::Get().Submit(
            "--- PRESS ENTER TO QUIT ---",
            Wanted::Vector2(5.0f, static_cast<float>(artY + 12)),
            Wanted::Color::WHITE,
            100,
            true
        );
    }
}