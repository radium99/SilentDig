#include "BSPGenerator.h"
#include <cstdlib>
#include <ctime>
void BSPGenerator::Generate(int width, int height, int iterations)
{

    // 기존 데이터 초기화
    if (root) 
    {
        delete root;
        root = nullptr; 
    }
    
    leafRegions.clear();
    
    // 난수 시드 초기화 (실제 게임에서는 한 번만 수행 권장)
    static bool seeded = false;
    
    if (!seeded) { srand((unsigned int)time(NULL)); seeded = true; }
    
    // 루트 영역 생성 (전체 맵 크기)
    root = new Region(0, 0, width, height);
    
    // 재귀 분할 시작
    Split(root, iterations);
}

void BSPGenerator::Split(Region* node, int iter)
{
    // 종료 조건: 최대 재귀 횟수 도달 또는 영역이 너무 작음
    if (iter <= 0 || (node->width <= minSize * 2 && node->height <= minSize * 2))
    {
        leafRegions.push_back(node); // 더 이상 쪼갤 수 없는 영역이 '방' 후보
        return;
    }
    
    // 분할 방향 결정 (가로로 길면 세로로 자르고, 세로로 길면 가로로 자름)
    bool splitHorizontal = (rand() % 2 == 0);
    
    // 너비/높이 비율이 너무 차이 나면 강제로 방향 조정 (길쭉한 방 방지)
    if (node->width > node->height && (float)node->width / node->height >= 1.25f) { splitHorizontal = false; }
    else if (node->height > node->width && (float)node->height / node->width >= 1.25f) { splitHorizontal = true; }
    
    int maxSplit = (splitHorizontal ? node->height : node->width) - minSize;
    
    // 분할 가능 여부 재확인
    if (maxSplit <= minSize)
    {
        leafRegions.push_back(node);
        return;
    }
    
    // 최소 크기를 보장하는 지점에서 무작위 분할 위치 선정
    int splitPos = minSize + (rand() % (maxSplit - minSize));
    
    if (splitHorizontal)
    {
        // 가로로 자름 (위/아래 구역 생성)
        node->left = new Region(node->x, node->y, node->width, splitPos);
        node->right = new Region(node->x, node->y + splitPos, node->width, node->height - splitPos);
    }
    else
    {
        // 세로로 자름 (왼쪽/오른쪽 구역 생성)
        node->left = new Region(node->x, node->y, splitPos, node->height);
        node->right = new Region(node->x + splitPos, node->y, node->width - splitPos, node->height);
    }
    
    // 자식 영역들을 대상으로 재귀 분할
    Split(node->left, iter - 1);
    Split(node->right, iter - 1);
}

void BSPGenerator::CreateRoom(Region* node)
{
    // 구역 크기보다 최소 2타일 이상 작은 무작위 방 생성
    node->roomW = (rand() % (node->width - 4)) + 3;
    node->roomH = (rand() % (node->height - 4)) + 3;
    // 구역 내에서 방의 위치를 무작위로 배치
    node->roomX = node->x + (rand() % (node->width - node->roomW - 1)) + 1;
    node->roomY = node->y + (rand() % (node->height - node->roomH - 1)) + 1;
}
