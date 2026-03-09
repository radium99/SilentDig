#include "Box.h"

Box::Box(const Vector2& newPosition)
	: super("B", newPosition, Color::BLUE)
{
	// 그리기 우선순위 설정.
	sortingOrder = 5;
}
