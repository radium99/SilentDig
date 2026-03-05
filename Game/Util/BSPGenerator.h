#pragma once
#include <vector>
#include "Math/Vector2.h"


enum class TileType { Empty, Wall, Ground };


// 가상 구역.
struct Region
{
	// 자신의 위치 정보.
	int x, y, width, height;

	// 자식 노드.
	Region* left = nullptr;
	Region* right = nullptr;


	// 실제 방의 좌표 (구역보다 약간 작게 설정됨)
	int roomX, roomY, roomW, roomH;


	Region(int x, int y, int w, int h)
		: x(x), y(y), width(w), height(h)
	{
		roomX = roomY = roomW = roomH = 0;
	}


	~Region()
	{
		delete left;
		delete right;
	}

	// 구역의 중심점 반환 (복도 연결용)
	Wanted::Vector2 GetCenter() const { return Wanted::Vector2(x + width / 2, y + height / 2); }
};


class BSPGenerator
{
public:
	BSPGenerator(int minRegionSize)
		: minSize(minRegionSize)
	{
	}


	~BSPGenerator()
	{
		delete root;
	}


	void Generate(int width, int height, int iterations);


	const std::vector<Region*>& GetLeafRegions() const
	{
		return leafRegions;
	}

	void CreateTunnel(Region* node, std::vector<std::vector<TileType>>& map); // 인접한 방들을 터널로 연결하는 함수.

	void CreateEmptyRoom(std::vector<std::vector<TileType>>& map);

	Region* GetRoot() { return root; }

private:
	void Split(Region* node, int iter); // 구역을 분리하는 함수. (방을 만들기 위함)

	void CreateRoom(Region* node); // 구역 내부에 실제 방 공간 계산


	Region* root = nullptr;
	int minSize;
	std::vector<Region*> leafRegions; // 최종적으로 방이 생성될 구역들
};