#ifndef __SLOT2_H__
#define __SLOT2_H__

#include "Music.h"

typedef int Key;
typedef Music * Value;

typedef struct _slot
{
	Key key;
	Value val;
} Slot;

#endif