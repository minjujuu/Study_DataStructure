// 이름 : 박민주
// 학번 : 201914302
// 프로그램 설명 : 프로그래머스 - 베스트 앨범(https://programmers.co.kr/learn/courses/30/lessons/42579)
//                각 장르별로 상위 2개의 곡을 뽑아 베스트 앨범으로 수록하여 출력하는 프로그램
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Table2.h"
#include "Music.h"
#include "UsefulHeap.h"

#define MAX 10000 // 입력 문자열 길이의 최댓값
#define HASHMAX 100 // 장르의 최댓값

int SearchMusicNum(char *input);
void UserInput(char *genres, char *plays);
void SetArrays(char *genres, char *plays, char **genreStrArr, int *playsArr);
int MyHashFunc(int k, char **genreStrArr);
void HeapSort(GNTP gntp[], int n, PriorityComp pc);
int PriComp(int n1, int n2);
int SetHashVal(char *arr);

int main(void)
{
    char genres[MAX];
    char plays[MAX];

    UserInput(genres, plays);
    int glen = strlen(genres);
    int plen = strlen(plays);

    char *p = NULL;

    int tmp = 0;
    int n = 0;
    int count = 0;
    // 음악의 총 개수를 구한다
    count = SearchMusicNum(plays);

    // 초기화
    char *genreStrArr[count]; // 각 고유번호에 대한 장르를 담을 배열
    int playsArr[count]; // 각 고유번호에 대한 재생 횟수를 담을 배열
    GNTP gntp[count]; // 각 장르와 총 재생 횟수 담을 배열 

    SetArrays(genres, plays, genreStrArr, playsArr); // 입력 문자열로부터 배열을 셋팅 

    Table myTbl; // 해시테이블 생성
    Music *mp; // 해시테이블에 슬롯으로 저장될 Music 구조체를 참조할 때 사용할 포인터
    TBLInit(&myTbl, MyHashFunc); // 해시테이블 

    // 장르별 총 횟수를 구한다.
    // - 처음 보는 장르이면 장르 배열에 저장하고 아니면 총 횟수만 가져와서 더한다
    int sameGenre = 0;
    int gidx = 0;
    for (int i = 0; i < count; i++)
    {
        // 일단 저장
        if (playsArr[i] != -1)
        {
            gntp[gidx].genre = genreStrArr[i];
            gntp[gidx].totalPlays = playsArr[i];
            playsArr[i] = -1; // 확인한 곳은 -1로 체크

            for (int j = i + 1; j < count; j++)
            {
                // 이미 등록된 장르이면
                if (!strcmp(gntp[gidx].genre, genreStrArr[j]))
                {
                    gntp[gidx].totalPlays += playsArr[j]; // 총 재생 횟수만 더함
                    playsArr[j] = -1; // 확인한 곳은 -1로 체크
                }
            }
            gidx++; // 다음 반복을 위한 인덱스 조정
            sameGenre++; // 장르의 개수를 구함 (중복되지 않는)
        }
    }


    // 해쉬 전 playsArr만 다시 한 번 더 셋팅
    SetArrays(NULL, plays, NULL, playsArr);

    // 해시테이블에 저장하는 방식
    // 총 재생 횟수가 높은 순서대로 먼저 해시값을 구해서 해당되는 배열에 저장된다
    // 1. gntp[i].totalPlays 바탕으로 gntp배열을 정렬시킨다
    // 2. 출력할 때 gntp의 장르 우선순위를 기준으로 순서대로 해쉬값을 구한다
    // 3. 구한 해쉬값의 해시테이블 인덱스로 바로 찾아가서 리스트의 상위 두 개 값을 가져온다
    // 4. 베스트앨범에 수록하는데 값이 하나밖에 없으면 하나만 수록한다

    // 해시 테이블에 값 저장
    for (int i = 0; i < count; i++)
    {
        // Music구조체에 고유번호와 곡의 재생 횟수를 저장한다
        mp = MakeMusicData(i, playsArr[i]);
        // 해시테이블의 리스트로 넣는다
        TBLInsert(&myTbl, i, mp, genreStrArr);
    }

    // 장르과 총 재생횟수가 담긴 구조체 gntp배열을 정렬한다
    // - 총 재생횟수가 높은 장르 순서대로 정렬
    // - 출력 시 해당 정렬 결과를 바탕으로 먼저 출력되는 장르가 정해짐
    HeapSort(gntp, sameGenre, PriComp);

    // 베스트 앨범 구성
    int bestAlbum[2 * sameGenre];

    // gntp에 총 횟수가 높은 순서대로 장르가 저장되어 있는 점을 이용한다
    // 1. 장르의 해쉬값을 가져온다.
    // 2. 해당 장르의 해쉬값(인덱스)을 이용해서 해쉬테이블의 배열로 찾아간다
    // - 해쉬함수를 이용하므로 0부터 99까지 배열을 뒤지면서 해당 장르를 찾지 않아도 된다
    // - 바로 장르가 저장되어 있는 배열의 인덱스를 알 수 있다
    int hashIdx = 0; // 장르에 매칭되는 해쉬값을 저장할 변수
    int n1 = 0, n2 = 0; // 베스트 앨범에 등록될 상위 1개 or 2개 곡의 고유번호를 담을 변수
    int outIdx = 0; // 베스트 앨범 배열의 인덱스 조정을 위한 변수
    int bestNum = 0; // 베스트 앨범에 수록되는 곡의 개수 
    
    for (int i = 0; i < sameGenre; i++)
    {
        char *g = gntp[i].genre;

        hashIdx = SetHashVal(g);
        // 위에서 찾은 해시값에 대응하는 해쉬테이블의 각 리스트에 접근한다 
        GetTBLelem(&(myTbl.tbl[hashIdx]), &n1, &n2);
        bestAlbum[outIdx++] = n1; // 첫번째 수록곡 저장
        bestNum++;
        if (n2 != -1) // 두번째 곡이 있다면
        {
            bestAlbum[outIdx++] = n2; // 두번째 수록곡 저장
            bestNum++;
        }
        hashIdx = 0; // 다음 반복을 위한 초기화
    }

    // ----------- 결과 출력 --------------
    printf("[");
    for (int i = 0; i < bestNum; i++)
    {
        if(i == (bestNum-1))
        {
            printf("%d]\n",bestAlbum[i]);
            break;
        }
        printf("%d,", bestAlbum[i]);
    }
    printf("\n");

    // ----------- 해시 테이블에 값 동적 메모리 해제 ----------- 
    TBLDelete(&myTbl);

    return 0;
}

void UserInput(char *genres, char *plays)
{
    // 입력 받기
    scanf("%s", genres);
    scanf("%s", plays);
}

void SetArrays(char *genres, char *plays, char **genreStrArr, int *playsArr)
{
    int plen = strlen(plays);
    char *p = plays;
    int n = 0, idx = 0;

    if (plays != NULL)
    {
        // plays 만 담긴 배열 셋팅
        for (int i = 0; i < plen - 1;)
        {
            if (plays[i] == '[' || plays[i] == ']')
            {
                p++;
                i++;
            }
            else
            {
                sscanf(p, "%d,%n", &playsArr[idx++], &n);
                p += n;
                i += n;
            }
        }
    }

    if (genreStrArr != NULL)
    {
        p = strtok(genres, "\",\"");
        idx = 0;
        // genres만 담긴 배열 셋팅
        while (p != NULL)
        {
            p = strtok(NULL, "\",\"");
            genreStrArr[idx++] = p;
        }
    }
}

int SearchMusicNum(char *input)
{
    int plen = strlen(input);
    int count = 0, n = 0, tmp = 0;

    char *p = input;
    // -----------입력개수 조사-----------
    for (int i = 0; i < plen - 1;)
    {
        if (input[i] == '[' || input[i] == ']' || input[i] == ',')
        {
            p++;
            i++;
        }
        else
        {
            sscanf(p, "%d%n", &tmp, &n);
            count++;
            p += n;
            i += n;
        }
    }

    return count;
}

// 고유번호가 들어오면 장르 이름을 기준으로 해시값을 생성하여 리턴해준다
int MyHashFunc(int k, char **genreStrArr)
{
    int hash = SetHashVal(genreStrArr[k]);
    return hash;
}

// 해시값 셋팅을 돕는 함수
int SetHashVal(char *arr)
{
    int hash = 0;
    int i = 0;
    while(arr[i] != '\0')
    {
        hash += arr[i];
        i++;
    }
    hash %= 100;
    return hash;
}

// gntp배열의 정렬 기준이 되는 함수
int PriComp(int n1, int n2)
{
    return n1 - n2;
}

// 힙 정렬
void HeapSort(GNTP gntp[], int n, PriorityComp pc)
{
    Heap heap;
    int i;

    HeapInit(&heap, pc);

    // 정렬 대상을 가지고 힙을 구성
    for (i = 0; i < n; i++)
        HInsert(&heap, gntp[i]);

    // 순서대로 하나씩 꺼내서 정렬 완성
    for (i = 0; i < n; i++)
        gntp[i] = HDelete(&heap);
}