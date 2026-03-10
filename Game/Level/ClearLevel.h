#pragma once

#include "Level/Level.h"
#include <vector>
#include <string>

// 스테이지 클리어 시 표시되는 레벨 클래스
class ClearLevel : public Wanted::Level
{
    RTTI_DECLARATIONS(ClearLevel, Level)

public:
    ClearLevel();
    virtual ~ClearLevel() = default;

    virtual void BeginPlay() override;
    virtual void Tick(float deltaTime) override;
    virtual void Draw() override;

private:
    std::vector<std::string> asciiArt;
    float timer = 0.0f;
    bool showPressEnter = true;
};