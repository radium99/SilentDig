#include "Renderer.h"
#include "ScreenBuffer.h"
#include "Util/Util.h"

namespace Wanted
{
	Renderer::Frame::Frame(int bufferCount)
	{
		// 배열 생성 및 초기화.
		charInfoArray = new CHAR_INFO[bufferCount];
		memset(charInfoArray, 0, sizeof(CHAR_INFO) * bufferCount);

		sortingOrderArray = new int[bufferCount];
		memset(sortingOrderArray, 0, sizeof(int) * bufferCount);
	}

	Renderer::Frame::~Frame()
	{
		SafeDeleteArray(charInfoArray);
		SafeDeleteArray(sortingOrderArray);
	}

	void Renderer::Frame::Clear(const Vector2& screenSize)
	{
		// 2차원 배열로 다루는 1차원 배열을 순회하면서
		// 빈 문자(' ')를 설정.
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; ++y)
		{
			for (int x = 0; x < width; ++x)
			{
				// 배열 인덱스 구하기.
				const int index = (y * width) + x;

				// 글자 값 및 속성 설정.
				CHAR_INFO& info = charInfoArray[index];
				info.Char.AsciiChar = ' ';
				info.Attributes = 0;

				// 그리기 우선순위 초기화.
				sortingOrderArray[index] = -1;
			}
		}
	}

	// -------------- Frame -------------- //

	// 정적 변수 초기화.
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize)
		: screenSize(screenSize), cameraPosition(0, 0)// 초기 위치 0,0
	{
		instance = this;

		// 프레임 객체 생성.
		const int bufferCount = screenSize.x * screenSize.y;
		frame = new Frame(bufferCount);

		// 프레임 초기화.
		frame->Clear(screenSize);

		// 이중 버퍼 객체 생성 및 초기화.
		screenBuffers[0] = new ScreenBuffer(screenSize);
		//screenBuffers[0]->Clear();

		screenBuffers[1] = new ScreenBuffer(screenSize);
		//screenBuffers[1]->Clear();

		// 활성화 버퍼 설정.
		Present();
	}

	Renderer::~Renderer()
	{
		SafeDelete(frame);
		for (ScreenBuffer*& buffer : screenBuffers)
		{
			SafeDelete(buffer);
		}
	}

	void Renderer::Draw(float deltaTime)
	{
		// 화면 지우기.
		Clear();

		// 진동 오프셋 계산 (EffectiveManager에서 가져옴)
		// deltaTime은 엔진에서 관리하는 값을 사용하거나 인자로 받아야 한다.
		Vector2 shakeOffset = EffectManager::Get().GetShakeOffset(deltaTime);

		// 진동이 적용된 임시 카메라 위치 계산.
		// 원본 카메라 위치는 유지. 이번 프레임 렌더링만 shakeOffset 더함.
		Vector2 effecttiveCamPos = cameraPosition - shakeOffset;

		// 화면 중심 오프셋 계산 (카메라 위치가 화면의 정중앙이 되도록)
		Vector2 screenCenter(screenSize.x / 2, screenSize.y / 2);

		for (const RenderCommand& command : renderQueue)
		{
			if (!command.text) continue;
			// 월드 좌표 - 카메라 좌표 + 화면 중심 오프셋 = 화면 상대좌표 계산
			//Vector2 screenPos = command.position - cameraPosition + screenCenter;
			Vector2 screenPos = command.position - effecttiveCamPos + screenCenter; // 기존 cameraPosition에서 effectiveCamPos로 수정.

			// 화면 밖 y축 클램핑 체크 ( screen 기준으로 변경)
			if (screenPos.y < 0 || screenPos.y >= screenSize.y)
			{
				continue;
			}
			const int length = static_cast<int>(strlen(command.text));
			if (length <= 0) { continue; }

			const int startX = screenPos.x;
			const int endX = screenPos.x + length - 1;

			// X축 클램핑 체크
			if (endX < 0 || startX >= screenSize.x)
			{
				continue;
			}
			const int visibleStart = startX < 0 ? 0 : startX;
			const int visibleEnd = endX >= screenSize.x ? screenSize.x - 1: endX;
			
			for (int x = visibleStart; x <= visibleEnd; ++x)
			{
				const int sourceIndex = x - startX;
				const int index = (screenPos.y * screenSize.x) + x;

				if (frame->sortingOrderArray[index] > command.sortingOrder) { continue; }
				frame->charInfoArray[index].Char.AsciiChar = command.text[sourceIndex];
				frame->charInfoArray[index].Attributes = (WORD)command.color;
				frame->sortingOrderArray[index] = command.sortingOrder;
			
			}

			// Todo: 이후 그리는 로직은 screenPos를 사용하여 진행.
		}


		// 그리기.
		GetCurrentBuffer()->Draw(frame->charInfoArray);

		// 버퍼 교환.
		Present();

		// 렌더 큐 비우기.
		renderQueue.clear();
	}

	Renderer& Renderer::Get()
	{
		if (!instance)
		{
			MessageBoxA(
				nullptr,
				"Renderer::Get() - instance is null",
				"Error",
				MB_OK
			);

			__debugbreak();
		}

		return *instance;
	}

	void Renderer::SetCameraPosition(const Vector2& pos)
	{
		
		cameraPosition = pos;
	}

	void Renderer::Clear()
	{
		// 화면 지우기.
		// 1. 프레임(2차원 배열 데이터) 지우기.
		frame->Clear(screenSize);

		// 2. 콘솔 버퍼 지우기.
		GetCurrentBuffer()->Clear();
	}

	void Renderer::Submit(
		const char* text,
		const Vector2& position,
		Color color,
		int sortingOrder)
	{
		// 렌더 데이터 생성 후 큐에 추가.
		RenderCommand command = {};
		command.text = text;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::Present()
	{
		// 버퍼 교환.
		SetConsoleActiveScreenBuffer(GetCurrentBuffer()->GetBuffer());

		// 인덱스 교체.
		currentBufferIndex = 1 - currentBufferIndex;
	}

	ScreenBuffer* Renderer::GetCurrentBuffer()
	{
		return screenBuffers[currentBufferIndex];
	}
}