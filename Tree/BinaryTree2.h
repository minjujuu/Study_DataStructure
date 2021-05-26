#ifndef __BINARY_TREE2_H__
#define __BINARY_TREE2_H__

typedef int BTData;

typedef struct _bTreeNode
{
	BTData data;
	int x; // 트리 생성 시 키 값으로 사용될 x값을 멤버로 추가
	struct _bTreeNode * left;
	struct _bTreeNode * right;
} BTreeNode;

BTreeNode * MakeBTreeNode(void);
BTData GetData(BTreeNode * bt);
int GetXval(BTreeNode * bt); // x값을 가져오기 위한 함수 추가
void SetData(BTreeNode * bt, BTData data);
void SetXData(BTreeNode * bt, int data); // x값을 저장하기 위한 함수 추가

BTreeNode * GetLeftSubTree(BTreeNode * bt);
BTreeNode * GetRightSubTree(BTreeNode * bt);

void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub);
void MakeRightSubTree(BTreeNode * main, BTreeNode * sub);

typedef void VisitFuncPtr(BTData data, int * arr, int nodeNum); // 함수 포인터 형식 변경

// 순회 관련 함수 변경 (매개변수 추가)
void PreorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum); 
void InorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum);
void PostorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum);

void DeleteTree(BTreeNode *bt); // p.337 동적할당 해제 함수 추가

#endif