#pragma once
#include <iostream>
#include <string>
#include <vector>

class ClearScreenRenderer {
public:
    // Effective C++ Item 3: 상태를 변경하지 않는 함수는 const로 선언
    void Render(int floor, int exp, int gold) const {
        PrintHeader();
        std::cout << "                     [ FLOOR " << floor << "  CLEARED! ]" << std::endl;
        std::cout << "\n================================================================================" << std::endl;
        std::cout << "    >> 획득한 경험치: " << exp << " EXP" << std::endl;
        std::cout << "    >> 획득한 골드  : " << gold << " G" << std::endl;
        std::cout << "================================================================================" << std::endl;
        std::cout << "                 ( 다음 층으로 가려면 Enter )" << std::endl;
    }

private:
    // Effective C++ Item 2: 매크로(#define) 대신 상수 변수나 인라인 함수 사용
    void PrintHeader() const {
        const char* art =
            "      _______  __       _______     ___      .______      \n"
            "     /      ||  |     |   ____|   /   \\     |   _  \\     \n"
            "    |  ,----'|  |     |  |__     /  ^  \\    |  |_)  |    \n"
            "    |  |     |  |     |   __|   /  /_\\  \\   |      /     \n"
            "    |  `----.|  `----.|  |____ /  _____  \\  |  |\\  \\----.\n"
            "     \\______||_______||_______/__/     \\__\\ | _| `._____|\n";

        std::cout << "================================================================================" << std::endl;
        std::cout << art << std::endl;
    }
};