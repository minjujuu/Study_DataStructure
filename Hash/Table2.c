#include <stdio.h>
#include <stdlib.h>
#include "Table2.h"
#include "DLinkedList.h"

void TBLInit(Table * pt, HashFunc * f)
{
	int i;

	for(i=0; i<MAX_TBL; i++)
	{
		ListInit(&(pt->tbl[i]));
		SetSortRule(&(pt->tbl[i]), WhoIsPrecede);
	}
	pt->hf = f;
}

void TBLInsert(Table * pt, Key k, Value v, char ** hashSearchArr)
{
	int hv = pt->hf(k, hashSearchArr);
	Slot ns = {k, v};
	
	if(TBLSearch(pt, k, hashSearchArr) != NULL)       // 키가 중복되었다면
	{
		printf("키 중복 오류 발생 \n");
		return;
	}
	else
	{
		LInsert(&(pt->tbl[hv]), ns);
	}
}

Value TBLDelete(Table * pt)
{
	List *plist;
	LData pdata;
	for(int i = 0; i <MAX_TBL; i++)
	{
		if((pt->tbl[i].numOfData) > 0)
		{
			
			if (LFirst(&(pt->tbl[i]), &pdata))
			{
				LRemove(&(pt->tbl[i]));
				while (LNext(&(pt->tbl[i]), &pdata))
				{
					LRemove(&(pt->tbl[i]));
				}
			}
		}
	}
	return NULL;
}

Value TBLSearch(Table * pt, Key k, char ** hashSearchArr)
{
	int hv = pt->hf(k, hashSearchArr);
	Slot cSlot;

	if(LFirst(&(pt->tbl[hv]), &cSlot))
	{
		if(cSlot.key == k)
		{
			printf("[%d: %d]\n", cSlot.val->uniqueNum, cSlot.val->plays);
			return cSlot.val;
		}
		else
		{
			while(LNext(&(pt->tbl[hv]), &cSlot))
			{
				if(cSlot.key == k)
				{
					printf("[%d: %d]\n", cSlot.val->uniqueNum, cSlot.val->plays);
					return cSlot.val;
				}
					
			}
		}
	}

	return NULL;
}

// 전체를 순회하는 함수
// 배열의 한 인덱스에 있는 모든 것들을 가져오고 싶음
void TBLPrint(Table *pt)
{
	Slot cSlot;
	for(int i = 0; i <MAX_TBL; i++)
	{
		if((pt->tbl[i].numOfData) > 0)
		{
			printf("---->> 해시값 %d에 대한 연결리스트 <<----\n", i);
			if (LFirst(&(pt->tbl[i]), &cSlot))
			{
				printf("[%d: %d]\n", cSlot.key, cSlot.val->plays);
				while (LNext(&(pt->tbl[i]), &cSlot))
				{
					printf("[%d: %d]\n", cSlot.key, cSlot.val->plays);
				}
			}
		}
	}
}

// 원하는 인덱스의 리스트에서 숫자를 두개씩 가져온다 
void GetTBLelem(List *plist, int *n1, int *n2)
{
	LData ld;
	LFirst(plist, &ld);
	*n1 = ld.key;
	if(plist->numOfData > 1)
	{
		LNext(plist, &ld);
		*n2 = ld.key;
	}
	else
		*n2 = -1;
}

int WhoIsPrecede(LData d1, LData d2)
{
    if(d1.val->plays > d2.val->plays) // 재생 횟수가 높은 순서대로 저장
    {
        return 0;
    }
    else if(d1.val->plays == d2.val->plays && d1.key < d2.key) // 재생 횟수가 같으면
    {
        return 0;
    }
    else
        return 1;
}