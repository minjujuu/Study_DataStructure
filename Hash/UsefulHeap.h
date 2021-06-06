#ifndef __USEFUL_HEAP_H__
#define __USEFUL_HEAP_H__

#include "Music.h"
#define TRUE	1
#define FALSE	0

/*** Heap�� ���� ****/
#define HEAP_LEN	100

typedef GNTP HData;

typedef int PriorityComp(int d1, int d2);

typedef struct _heap
{
	PriorityComp * comp;
	int numOfData;
	HData heapArr[HEAP_LEN];
} Heap;

/*** Heap ���� ����� ****/
void HeapInit(Heap * ph, PriorityComp pc);
int HIsEmpty(Heap * ph);

void HInsert(Heap * ph, HData data);
HData HDelete(Heap * ph);

#endif