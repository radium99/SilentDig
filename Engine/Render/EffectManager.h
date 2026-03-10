#pragma once
#include "Common/Common.h"
#include "Math/Vector2.h"

namespace Wanted {
	class WANTED_API EffectManager
	{
	public:
		static EffectManager& Get();

		// 진동 시작 명령 함수.
		void TriggerShake(float intensity, float duration);

		// 매 프레임 업데이트 ( Renderer or Level에서 호출 예정)
		// 진동 오프셋 계산 (정수형 좌표로 반환)
		Vector2 GetShakeOffset(float deltaTime);

	private:
		EffectManager() = default;
		float shakeIntensity = 0.0f;
		float shakeDuration = 0.0f;
		//Wanted::Vector2 originalCamPos;

	};
}