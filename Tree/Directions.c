
// 제목 : 과제 5 - 길찾기 게임 (https://programmers.co.kr/learn/courses/30/lessons/42892)
// 학번 : 201914302 
// 이름 : 박민주
// 작성일 : 2021-05-23
// 설명 : 입력된 좌표 배열을 통해 트리를 구성한 후 전위/후위 순회하여 출력하는 프로그램

#include <stdio.h>
#include <string.h>
#include "BinaryTree2.h"

#define MAX 10000
#define XYMAX 100000

BTreeNode *MakeTree(char *nodeInfo, int nodeNum);                // 트리를 만드는 함수
void InsertNode(BTreeNode **pRoot, int xVal, int idx);           // 트리 구성 시 노드를 생성하여 추가하는 함수
int FindMaxYval(char *nodeInfo, int preMax, int *sameYNum);      // 알고리즘에서 가장 큰 y값을 찾아오는 함수
int FindXIdx(char *nodeInfo, int nextY, int preIdx, int *nextX); // 알고리즘에서 y값에 대응하는 x값을 찾아오는 함수
int SearchNodeNum(char *nodeInfo);                               // 트리에 만들어질 노드의 개수를 찾아오는 함수
void SetOutArr(int data, int *arr, int nodeNum);                 // 노드 방문 시 해당 노드의 데이터를 배열에 담는 함수


// -------------------------- main() ---------------------------------
// - 트리를 만드는 데 사용될 좌표 배열을 문자열로 입력 받는다
// - 트리에 만들어질 총 노드의 개수를 구하며 이 값은 트리를 만드는 데 활용된다
// - 출력 시 사용될 2차원 배열을 만들고 셋팅한다
// - 전위/후위 순회 함수를 호출하여 2차원 배열에 값을 저장한다
// - 결과 출력 후에는 동적할당을 해제한다
int main(void)
{
    int nodeNum = 0;
    // nodeInfo 배열에 문자열을 입력받는다
    char nodeInfo[MAX];
    scanf("%s", nodeInfo);

    // 최상위 루트 노드를 먼저 만든다
    BTreeNode *root = NULL;
    nodeNum = SearchNodeNum(nodeInfo); // 노드의 개수를 찾는다

    // 입력받은 좌표 배열 대한 트리를 만든다
    root = MakeTree(nodeInfo, nodeNum);

    // 출력용 2차원 배열 셋팅
    int i, j;
    int outArr[2][nodeNum];
    for (i = 0; i < 2; i++)
    {
        for (j = 0; j < nodeNum; j++)
            outArr[i][j] = 0;
    }

    // 전위/순회 출력을 위해 노드의 값을 배열에 저장
    PreorderTraverse(root, SetOutArr, &outArr[0][0], nodeNum);
    PostorderTraverse(root, SetOutArr, &outArr[1][0], nodeNum);

    // 출력
    printf("[");
    for (i = 0; i < 2; i++)
    {
        printf("[");
        for (j = 0; j < nodeNum; j++)
        {
            if (j != nodeNum - 1)
                printf("%d,", outArr[i][j]);
            else
                printf("%d", outArr[i][j]);
        }
        if (i == 0)
            printf("],");
        else
            printf("]");
    }
    printf("]\n");

    // 동적 할당 해제
    DeleteTree(root);

    return 0;
}


// -------------------------- MakeTree() ---------------------------------
// - 트리를 만드는 함수
// - 사용된 알고리즘
//   1. 최대의 y값을 찾고, 해당 레벨에 존재하는 y값의 중복 개수를 찾는다
//   2. 위의 y값에 대응되는 x값을 찾는다 (y값의 중복 개수만큼 반복)
//   3. 노드를 만들어서 트리에 삽입한다
//   4. 다음 레벨로 내려가서 위 과정을 반복한다
BTreeNode *MakeTree(char *nodeInfo, int nodeNum)
{
    int maxY = 0;        // 최대 y값
    int sameYNum = 0;    // 같은 레벨의 y값 개수
    int x = 0;           // 찾은 y값에 대응하는 x값
    int preMaxY = XYMAX; // 이전 레벨의 y값
    int preIdx = 0;      // 같은 레벨에서 찾을 x값의 중복을 피하기 위한 변수

    BTreeNode *root = NULL;

    for (int i = 0; i < nodeNum + 1; i++)
    {
        // 1. 존재하는 y값 중 가장 큰 y값을 가져온다
        maxY = FindMaxYval(nodeInfo, preMaxY, &sameYNum);
        preMaxY = maxY;
        for (int j = 0; j < sameYNum; j++)
        {
            // y값에 대응되는 x값을 찾고 해당 좌표의 순번값을 리턴받는다
            preIdx = FindXIdx(nodeInfo, maxY, preIdx, &x);
            // x값을 기준으로 트리에 삽입하고, 노드의 데이터는 순번값으로 설정한다
            InsertNode(&root, x, preIdx);
        }
        // 반복횟수 보정
        nodeNum -= sameYNum;
        i = 0;
        preIdx = 0; // 다음 반복 시 사용을 위해 초기화
    }

    return root;
}

// -------------------------- InsertNode() ---------------------------------
// - 강의 12주차 (교재 p.441) 에 배운 BSTInsert()를 변형하여 만든 함수
// - x값을 정렬 기준으로 하여 자신의 위치를 찾아간다
void InsertNode(BTreeNode **pRoot, int xVal, int idx)
{
    BTreeNode *pNode = NULL;   // 부모 노드 생성
    BTreeNode *cNode = *pRoot; // 인수로 들어온 노드를 현재 노드로 설정
    BTreeNode *nNode = NULL;   // 새 노드 생성

    // 현재 노드가 NULL이 아닐 때까지 반복
    while (cNode != NULL)
    {
        // 중복된 x값은 없음
        if (xVal == GetXval(cNode))
            return;

        // 현재 노드를 부모 노드로 설정
        pNode = cNode;

        // - 비교대상의 x값이 인자로 들어온 x값보다 크면
        if (GetXval(cNode) > xVal)
            cNode = GetLeftSubTree(cNode);  // 현재 노드를 왼쪽에 추가하기 위해 Left Sub Tree로 내려간다
        else                                // - 비교대상의 x값이 인자로 들어온 x값보다 작으면
            cNode = GetRightSubTree(cNode); // 현재 노드를 오른쪽에 추가하기 위해 Right Sub Tree로 내려간다
    }

    nNode = MakeBTreeNode(); // 새 노드 생성
    SetData(nNode, idx);     // 노드의 메인 Data를 좌표 순번값으로 저장 (노드의 번호이자 출력데이터가 됨)
    SetXData(nNode, xVal);   // 노드의 x값을 저장

    if (pNode != NULL)
    {
        if (xVal < GetXval(pNode)) // 현재 x값이 부모 노드보다 작으면
        {
            MakeLeftSubTree(pNode, nNode); // Left Sub Tree로 설정
        }
        else // 현재 x값이 부모 노드보다 크면
        {
            MakeRightSubTree(pNode, nNode); // Right Sub Tree로 설정
        }
    }
    else
    {
        *pRoot = nNode; // 들어온 게 첫 노드이면 최상위 루트 노드로 설정
    }
}

// -------------------------- FindMaxYval() ---------------------------------
// - 다음 레벨의 y값을 구하는 함수
// - 이전 레벨의 y값 보다는 작고, 배열에 있는 y값 중에는 가장 큰 y값을 찾는다
// - 찾은 y값을 리턴하고, 인자로 들어온 포인터를 통해 중복 개수를 전달한다
int FindMaxYval(char *nodeInfo, int preMax, int *sameYNum)
{
    int tmpY = 0, tmpX = 0, n = 0, count = 1;
    int len = strlen(nodeInfo);
    int max = 0;
    char *p = nodeInfo;

    for (int i = 0; i < len - 1;)
    {
        if (nodeInfo[i] == '[' || nodeInfo[i] == ']' || nodeInfo[i] == ',')
        {
            p++;
            i++;
        }
        else // 숫자이면
        {
            sscanf(p, "%d,%d%n", &tmpX, &tmpY, &n);
            if (max == tmpY)
                count++;                     // 같은 레벨의 y값 개수
            if (tmpY < preMax && tmpY > max) // 읽어온 y값이 이전 y값보다 작고, 임의의 max값보다 크면
            {
                max = tmpY; // 최대값으로 설정
                count = 1;  // 최대값이 바뀌었으므로 count를 1로 설정
            }
            p += n; // 포인터 이동
            i += n; // 반복 횟수 조정
        }
    }

    *sameYNum = count; // 찾은 y값의 중복 개수를 포인터로 전달

    return max; // 찾은 y값을 리턴
}

// -------------------------- FindXval() ---------------------------------
// - 위의 y값에 대응하는 x값을 찾는다
// - 한 번 찾은 값은 또 찾지 않기 위해 찾은 x좌표의 인덱스를 기록한다
// - 앞 쪽 좌표를 우선으로 찾고, 다음 번엔 그 뒤에 있는 좌표만 탐색 대상이 된다
int FindXIdx(char *nodeInfo, int nextY, int preIdx, int *nextX)
{
    int tmpY = 0, tmpX = 0, count = 0, n = 0, idx = 0;
    int len = strlen(nodeInfo);
    char *p = nodeInfo;
    int target = 0;

    for (int i = 0; i < len - 1;)
    {
        if (nodeInfo[i] == '[' || nodeInfo[i] == ']' || nodeInfo[i] == ',')
        {
            p++;
            i++;
        }
        else // 숫자이면
        {
            count++;
            sscanf(p, "%d,%d%n", &tmpX, &tmpY, &n);
            // 찾으려는 y값에 대응하고, 전에 찾은 x값의 인덱스보다 큰 경우
            if (tmpY == nextY && count > preIdx)
            {
                target = tmpX; // 입력된 x값을 target 값으로 설정
                idx = count;   // 순번 값을 기록
                break;         // 앞 쪽 좌표를 우선으로 찾기 위해 한 번 발견하면 더 이상 탐색하지 않음
            }
            p += n; // 포인터 이동
            i += n; // 반복 횟수 조정
        }
    }

    *nextX = target; // 찾은 값을 포인터로 전달

    return idx; // 찾은 x값의 순번 값를 반환
}

// -------------------------- SearchNodeNum() ---------------------------------
// - 노드의 개수를 찾아 반환하는 함수
int SearchNodeNum(char *nodeInfo)
{
    int len = strlen(nodeInfo);
    int count = 0;

    for (int i = 0; i < len; i++)
    {
        if (nodeInfo[i] == '[')
            count++;
    }
    return count - 1;
}

// -------------------------- SetOutArr() ---------------------------------
// - 트리를 순회하며 노드의 데이터를 배열에 담는 함수
void SetOutArr(int data, int *arr, int nodeNum)
{
    int static idx = 0;
    
    arr[idx++] = data;
    if (idx == nodeNum)
        idx = 0;
}