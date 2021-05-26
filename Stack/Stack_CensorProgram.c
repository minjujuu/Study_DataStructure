/* 학번 : 201914302
 * 이름 : 박민주
 * 생성일 : 2021-04-24
 * 프로그램 설명 : 스택(Stack)을 활용하여 입력받은 텍스트에서 특정 단어를 검열하는 프로그램
 */

#include <stdio.h>
#include <string.h>
#include "ListBaseStack.h"
#define MAX_A 25
#define MAX_T 300000

int SearchFirStack(Stack *firStc, char *arrA, int lenA, int *lenTp, Stack *endStc);
int SearchEndStack(Stack *endStc, char *arrA, int lenA, int *lenTp, Stack *firStc);
void SetStcA(int part, int lenA, Stack *stcA, char* arrA);

int main(void) {
    // 첫째 줄에 단어 A를 입력받음 (알파벳 소문자로만 구성, 최대 25자)
    // 둘째 줄에 텍스트 T를 입력받음 (알파벳 소문자로만 구성, 최대 300000자)
    char arrA[MAX_A + 1];
    char arrT[MAX_T + 1];

    // 단어A와 텍스트T를 입력받는다
    scanf("%s", arrA);
    scanf("%s", arrT);

    // 입력된 텍스트T를 담을 스택 생성
    Stack firStc; // 텍스트 T에서 처음 등장하는 단어 A를 검열하기 위한 스택
    Stack endStc; // 텍스트 T에서 마지막에 등장하는 단어 A를 검열하기 위한 스택

    // 단어 A와 텍스트 T의 길이를 저장하는 변수
    int lenT, lenA;
    lenT = strlen(arrT); // 텍스트 T의 길이
    lenA = strlen(arrA); // 텍스트 A의 길이

    // 출력할 때 사용할 배열 생성
    char outArr[lenT];

    // 스택 초기화
    StackInit(&firStc);
    StackInit(&endStc);

    int i, j;
    char tok;
    // T에 A가 있는지를 판단하는 변수
    // - 스택을 검사하는 함수들의 리턴값을 담는다
    // - 스택에 더 이상 검열할 단어가 없는 경우 1이 된다
    int isCompleted = 0;

    // 처음 등장하는 단어 A를 찾기 위해 먼저 firStc에 넣는다
    // 텍스트T의 길이만큼 반복해서 Stack에 담는다.
    // 앞 글자가 Top에 위치해야하므로 뒤 인덱스부터 집어넣는다.
    int comp = 0;
    int tmpin;
    int wordsNum = 0;
    for (i = lenT - 1; i >= 0; i--)
    {
	tok = arrT[i];
	SPush(&firStc, tok);
	tmpin = i;

	// 최초의 텍스트 T에서 단어 A가 존재하는지 검토
	// - 정확한 단어의 개수를 세는 것보다 1개라도 있는지를 찾는 것이 목적
	if(tok == arrA[lenA-1])
	{
	    for(j=lenA-1; j>=0; j--)
	    {
		if(arrT[tmpin--] == arrA[j])
		    comp++;
	    }
	    if(comp == lenA)
	    {
		wordsNum++;
	    }
	    comp = 0;
	}

    }

    // 텍스트 T에 단어 A가 없으면 알고리즘 시작 후 1번에서 종료
    if(wordsNum == 0)
    {
	isCompleted = 1;
    }

    // 알고리즘 수행
    while(1)
    {
	// 1. T에 A가 없으면 알고리즘을 종료한다
	if(isCompleted)
	{
	    printf("\n");
	    // 검열 완료된 텍스트 출력
	    if(!SIsEmpty(&firStc))
	    {
		for(i=0; i<lenT; i++)
		{
		    printf("%c", SPop((&firStc)));
		}
	    }
	    printf("\n");
	    return 0;
	}

	// 2. T에서 처음 등장하는 A를 찾은 뒤, 삭제한다.
	isCompleted = SearchFirStack(&firStc, arrA, lenA, &lenT, &endStc);

	// 3. T에 A가 없으면 알고리즘을 종료한다.
	if(isCompleted)
	{
	    printf("\n");
	    // 검열 완료된 텍스트 출력
	    if(!SIsEmpty(&endStc))
	    {
		// endStc에서 출력하는 경우 거꾸로 출력해야하므로
		// 임시 배열에 담은 다음 출력
		for(i=lenT-1; i>=0; i--)
		{
		    outArr[i] = SPop(&endStc);
		}
	    }
	    for(i=0; i<lenT; i++)
	    {
		printf("%c", outArr[i]);
	    }
	    printf("\n");
	    return 0;
	}

	// 4. T에서 마지막으로 등장하는 A를 찾은 뒤, 삭제한다.
	isCompleted = SearchEndStack(&endStc, arrA, lenA, &lenT, &firStc);
    }
}

// SearchFirStack()
// - 처음으로 등장하는 A를 찾기 위해 firStc를 검사한다
// - 마지막으로 등장하는 A를 찾기 위해 텍스트를 outStc으로 보낸다
int SearchFirStack(Stack *firStc, char *arrA, int lenA, int *lenTp, Stack *endStc)
{
    // stcA: 단어 A를 담는 스택
    // tmpStc : 비교할 때 필요한 임시 스택 (메인 스택에서 단어를 꺼내와서 담는다)
    Stack stcA, tmpStc;
    // 스택 초기화
    StackInit(&stcA);
    StackInit(&tmpStc);

    int i, check = 0;

    // 단어 A의 배열에 있는 글자를 꺼내와서 스택 셋팅
    SetStcA(1, lenA, &stcA, arrA);

    // 단어를 하나라도 삭제했는지 체크하는 변수
    int isRemove = 0;
    // FirstStack에서는 단어 A의 첫 글자와 스택의 Peek글자를 비교
    char firstWord = arrA[0];

    // 스택에서 글자를 꺼내와서 담을 char변수 (비교할 때 사용)
    char toktmp, tokA;

    // (예외처리용) 매 반복마다 꺼낸 단어 개수를 트래킹하는 변수
    int popNum = 0;

    // 스택이 비어있지 않은 경우에만 검색 진행
    while(!SIsEmpty(firStc))
    {
	// 만약 맨 위 글자와 첫 글자가 같으면 && 한 개의 단어도 삭제하지 않았으면
	if(SPeek(firStc) == firstWord && !isRemove)
	{
	    // 단어 A의 길이만큼 Pop한다
	    for(i=0; i<lenA; i++)
	    {
		// 단어 A의 길이만큼 다 꺼내지 못했어도 스택이 비어있으면 Pop을 멈춘다
		if(SIsEmpty((firStc)))
		    break;

		// 가장 위의 tok을 꺼낸다
		toktmp = SPop(firStc);
		// 임시배열에 저장
		SPush(&tmpStc, toktmp);
		// 비교를 위해 단어 A에서도 tok을 꺼낸다
		tokA = SPop(&stcA);

		// 비교
		if(toktmp == tokA)
		{
		    check++;
		}
		popNum++; // (예외처리용) 나중에 도로 집어넣을 때 필요하다
	    }

	    // - check가 lenA랑 다르다면 꺼낸 단어가 A와 같지 않다는 의미이다
	    if(check != lenA)
	    {
		// '꺼낸 개수-1' 만큼 도로 집어 넣음
		for(i = 0; i<popNum-1; i++)
		{
		    SPush(firStc, SPop((&tmpStc)));
		}
		// 첫글자는 다시 검사되지 않아도 되므로 endStc으로 보냄
		SPush(endStc, SPop(&tmpStc));
	    }
	    else // 검열할 단어를 발견했다면
	    {
		// 해당 단어는 삭제된 것으로 간주 (Pop한 걸 다시 Push하지 않으므로)
		// 텍스트 T의 길이가 줄어들었으므로 변경사항을 반영해준다 (출력 시 필요)
		(*lenTp)-=lenA;
		// 단어 1개를 삭제했으므로 검열을 멈춰야 한다
		isRemove = 1;
	    }
	    // 다음 검색 시 사용되는 변수들은 다시 0으로 초기화 해줌
	    check = 0;
	    popNum = 0;
	    // StcA에서 글자를 꺼내서 비교했으므로 다음 검색을 위해 다시 채워주어야 함
	    SetStcA(1, lenA, &stcA, arrA);
	}
	else
	{
	    // 첫 글자부터 아예 다르면 한 글자를 꺼내서 endStc로 보낸다
	    SPush(endStc, SPop(firStc));
	}
    }

    if(isRemove) // 삭제한 단어가 있으면 return 0
    {
	return 0;
    }
    else // 모든 검색이 끝났는데도 아무것도 삭제하지 않아 알고리즘 종료해도 되는 경우
	return 1;

}



int SearchEndStack(Stack *endStc, char *arrA, int lenA, int *lenTp, Stack *firStc)
{
    // stcA: 단어 A를 담는 스택
    // tmpStc : 비교할 때 필요한 임시 스택 (메인 스택에서 단어를 꺼내와서 담는다)
    Stack stcA, tmpStc;
    // 스택 초기화
    StackInit(&stcA);
    StackInit(&tmpStc);

    int i, check = 0;

    // 단어 A의 배열에 있는 글자를 꺼내와서 스택 셋팅
    SetStcA(0, lenA, &stcA, arrA);

    // 단어를 하나라도 삭제했는지 체크하는 변수
    int isRemove = 0;
    // EndStack에서는 단어A의 마지막 글자와 스택의 Peek글자를 비교
    char lastWord = arrA[lenA-1];

    // 스택에서 글자를 꺼내와서 담을 char변수 (비교할 때 사용)
    char toktmp, tokA;

    // (예외처리용) 매 반복마다 꺼낸 단어 개수를 트래킹하는 변수
    int popNum = 0;

    // 스택이 비어있지 않은 경우에만 반복 검색 진행
    while(!SIsEmpty(endStc))
    {
	// 만약 맨 위 글자와 A의 마지막 글자가 같고 && 아직 한 개의 단어도 지우지 않았다면
	if(SPeek(endStc) == lastWord && !isRemove)
	{
	    // 단어 A의 길이만큼 Pop한다
	    for(i=0; i<lenA; i++)
	    {
		// 단어 A의 길이만큼 다 꺼내지 못했어도 스택이 비어있으면 Pop을 멈춘다
		if(SIsEmpty((endStc)))
		    break;

		// end Stack에서 글자를 꺼낸다
		toktmp = SPop(endStc);
		// 단어 A와 다르면 도로 집어 넣기 위해 임시배열에 저장
		SPush(&tmpStc, toktmp);
		// 비교를 위해 단어 A의 스택에서도 tok을 꺼낸다
		tokA = SPop(&stcA);

		// 비교
		if(toktmp == tokA)
		{
		    check++;
		}
		popNum++; // (예외처리용) 나중에 도로 집어넣을 때 필요
	    }

	    // check가 lenA랑 다르다면 꺼낸 단어가 A와 같지 않다면
	    if(check != lenA)
	    {
		// '꺼낸 개수-1' 만큼 도로 집어 넣음 (마지막 글자는 다른 스택으로 보냄)
		for(i = 0; i<popNum-1; i++)
		{
		    SPush(endStc, SPop((&tmpStc)));
		}
		// 마지막 글자는 다시 검사되지 않아도 되므로 firStc으로 보냄
		SPush(firStc, SPop(&tmpStc));
	    }
	    else // 검열할 단어를 발견했다면
	    {
		// 해당 단어는 삭제된 것으로 간주 (Pop한 걸 다시 Push하지 않으므로)
		// 텍스트 T의 길이가 줄어들었으므로 변경사항을 반영해준다 (출력 시 필요)
		(*lenTp)-=lenA;
		// 단어 1개를 삭제했으므로 검열을 멈춰야 한다
		isRemove = 1;
	    }
	    // 다음 검색 시 사용되는 변수들은 다시 원래의 값으로 초기화 해줌
	    check = 0;
	    popNum = 0;
	    // StcA에서 글자를 꺼내서 비교했으므로 다음 검색을 위해 다시 채워주어야 함
	    SetStcA(0, lenA, &stcA, arrA);
	}
	else
	{
	    // 첫 글자부터 아예 다르면 한 글자를 꺼내서 firStc로 보낸다
	    SPush(firStc, SPop(endStc));
	}
    }

    if(isRemove) // 삭제한 단어가 있으면 return 0
    {
	return 0;
    }
    else // 모든 검색이 끝났는데도 아무것도 삭제하지 않아 알고리즘 종료해도 되는 경우
	return 1;
}

// SetStcA() : 단어를 비교/검사할 때 쓰이는 Stack을 셋팅하는 함수
// - 비교하고 검사할 때 SPop을 통해 꺼내서 검사하므로 다시 채워주는 기능이 필요
void SetStcA(int part, int lenA, Stack *stcA, char* arrA)
{
    int j;
    if(part) // firstack이면
    {
	// 단어 A의 뒷 글자부터 stack에 넣는다
	for(j=lenA-1; j>=0; j--)
	{
	    SPush(stcA, arrA[j]);
	}
    }
    else // endStack이면
    {
	// 단어 A의 앞 글자부터 stack에 넣는다
	for(j=0; j<lenA; j++)
	{
	    SPush(stcA, arrA[j]);
	}
    }
}
