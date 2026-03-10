#include "EffectManager.h"
#include <cstdlib>

namespace Wanted {

	EffectManager& EffectManager::Get() { static EffectManager instance; return instance; }

	void EffectManager::TriggerShake(float intensity, float duration)
	{
		shakeIntensity = intensity;
		shakeDuration = duration;
	}
	
	Vector2 EffectManager::GetShakeOffset(float deltaTime)
	{
		if (shakeDuration <= 0) return Vector2::Zero;

		shakeDuration -= deltaTime;
		int intensity = static_cast<int>(shakeIntensity);

		// 랜덤하게 -intensity ~ +intensity 사이의 값을 정수의 형태로 반환 (정수형이 더 확실하게 흔들림이 보임)
		int offsetX = (rand() % ((int)shakeIntensity * 1 + 1)) - (int)shakeIntensity;
		int offsetY = (rand() % ((int)shakeIntensity * 1 + 1)) - (int)shakeIntensity;

		return Vector2(offsetX, offsetY);
	}
}