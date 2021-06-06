#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Music.h"

int GetTotalNum(Music * m)
{
	return 0; //임시
}

void ShowMusicInfo(Music * m)
{
	printf("고유번호 : %d\n", m->uniqueNum);
	printf("플레이 횟수 : %d\ns", m->plays);
}

// 고유번호, 총 횟수를 순서대로 입력받는다 
Music * MakeMusicData(int uniqueNum, int plays)
{
	Music * newMusic = (Music*)malloc(sizeof(Music));
    newMusic->uniqueNum = uniqueNum;
    newMusic->plays = plays;

	return newMusic;
}