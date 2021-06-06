#ifndef __MUSIC_H__
#define __MUSIC_H__

typedef struct music
{
    int uniqueNum;
    int plays;
} Music;

typedef struct _genreNtotalP
{
    char *genre;
    int totalPlays;
} GNTP;

int GetTotalNum(Music * p);
void ShowMusicInfo(Music * p);
Music * MakeMusicData(int uniqueNum, int plays);

#endif