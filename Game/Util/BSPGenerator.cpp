#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime> // time() 사용을 위함.
#include <algorithm>

void BSPGenerator::Generate(int width, int height, int iterations)
{
    // 기존 데이터 초기화
    if (root) 
    {
        delete root;
        root = nullptr; 
    }
    
    // Todo: 메모리 누수 가능성 확인 후, leafRegions를 스마트 포인터로 선언할 것. (new Region()하고 delete안하면 메모리 누수 발생)
    leafRegions.clear();
    
    // 시드 초기화
    static bool seeded = false;
    if (!seeded) 
    {
        // time()을 이용한 종자값 설정.
        srand((unsigned int)time(nullptr));
        seeded = true; 
    }
    
    // 루트 영역 생성
    root = new Region(0, 0, width, height);
    
    // 영역 분할
    Split(root, iterations);
   
    // 각 분할된 영역에 실제 방 생성 (추가된 부분)
    for (auto* region : leafRegions)
    {
        CreateRoom(region);
        // Todo: 터널 생성 함수 여기에서 호출해야 함.
        
    }
}

void BSPGenerator::Split(Region* node, int iter)
{
    // 분할 중단 조건
    if (iter <= 0 || (node->width <= minSize * 2 && node->height <= minSize * 2))
    {
        leafRegions.push_back(node);
        return;
    }
    
    // 분할 방향 결정
    bool splitHorizontal = (rand() % 2 == 0);
    if (node->width > node->height && (float)node->width / node->height >= 1.25f) splitHorizontal = false;
    else if (node->height > node->width && (float)node->height / node->width >= 1.25f) splitHorizontal = true;
    
    int maxSplit = (splitHorizontal ? node->height : node->width) - minSize;
    if (maxSplit <= minSize)
    {
        leafRegions.push_back(node);
        return;
    }
    
    int splitPos = minSize + (rand() % (maxSplit - minSize));
    
    if (splitHorizontal)
    {
        node->left = new Region(node->x, node->y, node->width, splitPos);
        node->right = new Region(node->x, node->y + splitPos, node->width, node->height - splitPos);
    }
    else
    {
        node->left = new Region(node->x, node->y, splitPos, node->height);
        node->right = new Region(node->x + splitPos, node->y, node->width - splitPos, node->height);
    }
    
    Split(node->left, iter - 1);
    Split(node->right, iter - 1);
}

void BSPGenerator::CreateRoom(Region* node)
{
    // 방 크기 결정 (영역보다 약간 작게, 최소 3x3 보장)
    int minW = max(3, node->width / 2);
    int minH = max(3, node->height / 2);
    
    node->roomW = minW + (rand() % (node->width - minW));
    node->roomH = minH + (rand() % (node->height - minH));

    // 영역 내부에서 랜덤 위치 선정
    node->roomX = node->x + (rand() % (node->width - node->roomW));
    node->roomY = node->y + (rand() % (node->height - node->roomH));
}

// Todo: 방 간의 연결할 터널 생성 함수.
void BSPGenerator::CreateTunnel(Region* node, std::vector<std::vector<TileType>>& map)
{
    // 연결할 자식 노드가 하나라도 없는 경우.
    if (!node->left || !node->right)
    {
        return;
    }

    // 연결할 자식 노드들이 있는 경우.
    Wanted::Vector2 startPos = node->left->GetCenter();
    Wanted::Vector2 endPos = node->right->GetCenter();

    int curX = startPos.x;
    int curY = startPos.y;

    int targetX = endPos.x;
    int targetY = endPos.y;

    while(curX != targetX)
    {
        map[curY][curX] = TileType::Empty;
        curX += curX < targetX ? 1 : -1;
    }

    while (curY != targetY)
    {
        map[curY][curX] = TileType::Empty;
        curY += curY < targetY ? 1 : -1;
    }

    CreateTunnel(node->left, map);
    CreateTunnel(node->right, map);
}

void BSPGenerator::CreateEmptyRoom(std::vector<std::vector<TileType>>& map)
{
    for (auto* region : GetLeafRegions())
    {
        // 빈 공간(방) 크기 정보로 실제 맵에 그리기.
        for (int y = region->roomY; y < region->roomY + region->roomH; ++y)
            for (int x = region->roomX; x < region->roomX + region->roomW; ++x)
                map[y][x] = TileType::Empty;

        
    }
}
