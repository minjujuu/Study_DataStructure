#ifndef __TABLE2_H__
#define __TABLE2_H__

#include "Slot2.h"
#include "DLinkedList.h"

#define MAX_TBL     100

typedef int HashFunc(Key k, char ** hashSearchArr);

typedef struct _table
{
//	Slot tbl[MAX_TBL];
	List tbl[MAX_TBL];
	HashFunc * hf;
} Table;

void TBLInit(Table * pt, HashFunc * f); 
void TBLInsert(Table * pt, Key k, Value v, char ** hashSearchArr);
Value TBLDelete(Table * pt);
Value TBLSearch(Table * pt, Key k, char ** hashSearchArr);
void TBLPrint(Table *pt); // 전체요소 출력 함수 추가
void GetTBLelem(List *plist, int *n1, int *n2); // 리스트의 상위 두 개 요소를 가져오는 함수 추가
int WhoIsPrecede(LData d1, LData d2);
#endif