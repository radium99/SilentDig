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

	bool Actor::TestIntersect(const Actor* const other)
	{
		// AABB (Axis Aligned Bounding Box).
		// x 좌표만 고려하면됨. y는 크기가 1이기 때문.

		// 자기자신의 x좌표 정보.
		int xMin = position.x;
		int xMax = position.x + width - 1;

		// 충돌을 비교할 다른 액터의 x좌표 정보.
		int otherXMin = other->GetPosition().x;
		int otherXMax
			= other->position.x + other->width - 1;

		// 안겹치는 조건 확인.

		// 다른 액터의 왼쪽 좌표가
		// 내 오른쪽 좌표보다 더 오른쪽에 있는 경우.
		if (otherXMin > xMax)
		{
			return false;
		}

		// 다른 액터의 오른쪽 좌표가
		// 내 왼쪽 좌표보다 더 왼쪽에 있는 경우.
		if (otherXMax < xMin)
		{
			return false;
		}

		// y는 크기가 1이기 때문에 좌표가 같은지 여부만 확인.
		return position.y == other->position.y;
	}

	void Actor::ChangeImage(const char* newImage)
	{
		if (!newImage) return;
		int newWidth = static_cast<int>(strlen(newImage));

		if (image == nullptr || width < newWidth)
		{
			// 기존 메모리 해제.
			SafeDeleteArray(image);

			// 새로운 문자열 복사.
			width = static_cast<int>(strlen(newImage));
			image = new char[width + 1];

		}
		strcpy_s(image, width + 1, newImage);
	}

	void Actor::Destroy()
	{
		// 삭제 플래그 설정.
		destroyRequested = true;

		// 삭제 이벤트 호출.
		//OnDestroy();
	}
	
}
