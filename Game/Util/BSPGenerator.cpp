#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime>
#include <algorithm>

void BSPGenerator::Generate(int width, int height, int iterations)
{
    // 기존 데이터 초기화
    if (root) 
    {
        delete root;
        root = nullptr; 
    }
    
    leafRegions.clear();
    
    // 시드 초기화
    static bool seeded = false;
    if (!seeded) { srand((unsigned int)time(NULL)); seeded = true; }
    
    // 루트 영역 생성
    root = new Region(0, 0, width, height);
    
    // 영역 분할
    Split(root, iterations);

    // 각 분할된 영역에 실제 방 생성 (추가된 부분)
    for (auto* region : leafRegions)
    {
        CreateRoom(region);
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
