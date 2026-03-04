#include "Actor.h"
#include "Util/Util.h"
#include "Render/Renderer.h"
#include <iostream>
#include <Windows.h>

namespace Wanted
{
	Actor::Actor(const char* image, const Vector2& position, Color color)
		: position(position), color(color)
	{
		size_t length = strlen(image) + 1;
		this->image = new char[length];
		strcpy_s(this->image, length, image);
	}

	Actor::~Actor()
	{
		SafeDeleteArray(image);
	}

	void Actor::BeginPlay() { hasBeganPlay = true; }
	void Actor::Tick(float deltaTime) {}

	void Actor::Draw()
	{
		// Renderer가 카메라 위치를 바탕으로 화면 좌표를 알아서 계산해 주므로,
		// 그냥 세상(World) 좌표를 그대로 Submit 함.
		Renderer::Get().Submit(image, position, color, sortingOrder);
	}

	void Actor::SetPosition(const Vector2& newPosition)
	{
		if (position == newPosition) return;
		position = newPosition;
	}
}
