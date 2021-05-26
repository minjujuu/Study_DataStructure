#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree2.h"

BTreeNode * MakeBTreeNode(void)
{
	BTreeNode * nd = (BTreeNode*)malloc(sizeof(BTreeNode));
	nd->x = 0; // x값을 0으로 초기화
	nd->left = NULL;
	nd->right = NULL;
	return nd;
}

BTData GetData(BTreeNode * bt)
{
	return bt->data;
}

// 기존 ADT에서 추가된 함수
// - 노드에 저장된 x값을 가져온다
int GetXval(BTreeNode * bt)
{
	return bt->x;
}

void SetData(BTreeNode * bt, BTData data)
{
	bt->data = data;
}

// 기존 ADT에서 추가된 함수
// - 노드에 x값을 저장한다
void SetXData(BTreeNode * bt, int data)
{
	bt->x = data;
}

BTreeNode * GetLeftSubTree(BTreeNode * bt)
{
	return bt->left;
}

BTreeNode * GetRightSubTree(BTreeNode * bt)
{
	return bt->right;
}

void MakeLeftSubTree(BTreeNode * main, BTreeNode * sub)
{
	if(main->left != NULL)
		free(main->left);

	main->left = sub;
}

void MakeRightSubTree(BTreeNode * main, BTreeNode * sub)
{
	if(main->right != NULL)
		free(main->right);

	main->right = sub;
}


// 2차원 배열에 담아 출력하기 위해 전위/중위/후위 순회 함수 변경
// - action 함수의 인자로 넘길 정수형 배열과 노드의 개수에 대한 매개변수 추가
void PreorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum)
{
	if(bt == NULL)
		return;

	action(bt->data, arr, nodeNum);
	PreorderTraverse(bt->left, action, arr, nodeNum);
	PreorderTraverse(bt->right, action, arr, nodeNum);
}

void InorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum)
{
	if(bt == NULL)
		return;

	InorderTraverse(bt->left, action, arr, nodeNum);
	action(bt->data, arr, nodeNum);
	InorderTraverse(bt->right, action, arr, nodeNum);
}

void PostorderTraverse(BTreeNode * bt, VisitFuncPtr action, int * arr, int nodeNum)
{
	if(bt == NULL)
		return;

	PostorderTraverse(bt->left, action, arr, nodeNum);
	PostorderTraverse(bt->right, action, arr, nodeNum);
	action(bt->data, arr, nodeNum);
}

// 동적 할당된 메모리를 해제해주는 함수 추가 
void DeleteTree(BTreeNode *bt)
{
	if(bt == NULL)
		return;
	
	DeleteTree(bt->left);
	DeleteTree(bt->right);

	free(bt);
}