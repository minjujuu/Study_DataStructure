/* 학번: 201914302
   이름: 박민주
   작성일: 2021-04-10
   프로그램 설명: 유저가 입력한 ID를 카카오 아이디 규칙에 따라 수정된 신규 아이디를 추천해주는 프로그램 
                 총 7단계의 순차적인 처리 과정을 걸쳐 아이디를 검사하고 수정하며, 이 때 원형연결리스트가 사용됨
   */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "CLinkedList.h"
#define MAX_id 1000

void PrintAllData(List plist);

int main(void)
{
    // ID를 관리할 list 생성
    List new_id;

    // list의 data를 확인할 때 사용할 변수 생성
    int data;
    
    // list초기화
    ListInit(&new_id);
    
    char arr_id[MAX_id + 1]; // NULL 값 고려하여 1001 칸으로 생성

    printf("ID를 입력해주세요: ");
    scanf("%s", arr_id);


    int i;
    // 입력받은 ID를 new_id list에 저장
    // - tail에 추가되므로 순서대로 집어넣음
    for(i = 0; i < strlen(arr_id); i++)
    {
        LInsert(&new_id, arr_id[i]);
    }

    int nodeNum;

    nodeNum = LCount(&new_id);

    // 1단계. new_id의 모든 대문자를 대응되는 소문자로 치환합니다.
    if(nodeNum != 0)
    {
        // 처음 data를 검사 
        LFirst(&new_id, &data);
        if(isupper(data))
        {
            // 대문자이면 소문자로 변환하여 data에 집어넣음
            new_id.cur->data = tolower(data);
        }
        // 처음 이후의 data를 검사 
        for (i = 0; i < nodeNum - 1; i++)
        {
            // 다음 노드 조회 
            LNext(&new_id, &data);
            if (isupper(data))
            {
                // 대문자이면 소문자로 변환하여 data에 집어넣음
                new_id.cur->data = tolower(data);
            }
        }
    }


   // 2단계. new_id에서 알파벳 소문자, 숫자, 빼기(-), 밑줄(_), 마침표(.)를 제외한 모든 문자를 제거합니다.
    if(nodeNum != 0)
    {
        // 처음 data를 검사
        LFirst(&new_id, &data);
        if (!islower(new_id.cur->data) && !isdigit(new_id.cur->data) && (new_id.cur->data) != '-' && (new_id.cur->data) != '_' && (new_id.cur->data) != '.')
        {
            // 알파벳 소문자, 숫자, 빼기, 밑줄, 마침표가 아니면 해당 노드를 삭제
            LRemove(&new_id);
        }
        // 처음 이후의 data를 검사
        for (i = 0; i < nodeNum - 1; i++)
        {
            // 다음 노드 조회
            LNext(&new_id, &data);
            if (!islower(new_id.cur->data) && !isdigit(new_id.cur->data) && (new_id.cur->data) != '-' && (new_id.cur->data) != '_' && (new_id.cur->data) != '.')
            {
                // 알파벳 소문자, 숫자, 빼기, 밑줄, 마침표가 아니면 해당 노드를 삭제
                LRemove(&new_id);
            }
        }
    }

    
    // 3단계. new_id에서 마침표(.)가 2번 이상 연속된 부분을 하나의 마침표(.)로 치환합니다.
    // 처리 단계 
    // 1. count 변수는 node의 data가 마침표(.)이면 +1 됩니다.
    // 2. count 변수는 node의 data가 마침표(.)가 아니면 0으로 초기화됩니다.
    // 3. count가 2이상이면 연속되었다는 의미이므로 해당 node를 삭제합니다.
    int count = 0; // 마침표 개수를 관리
    if (nodeNum != 0)
    {
        // 처음부터 data를 조회하기 위해 LFirst로 초기화
        LFirst(&new_id, &data);
        // 처음 데이터 조회
        if ((new_id.cur->data) == '.')
        {
            // 마침표 일 경우 count +1 
            count++;
        }
        // 처음 이후 데이터 조회
        for (i = 0; i < nodeNum-1; i++)
        {
            LNext(&new_id, &data);
            if ((new_id.cur->data) == '.')
            {
                // 마침표일 경우 count +1 
                count++;
                if (count >= 2)
                {
                    // count가 2 이상이면 연속된 것이므로 해당 노드를 삭제
                    LRemove(&new_id);
                }
            }
            else
            {
                // 마침표가 아닐 경우 count = 0 (초기화)
                count = 0;
            }
        }
    }
    
    // 4단계. new_id에서 마침표(.)가 처음이나 끝에 위치한다면 제거합니다.
    // - 첫 data를 조회하여 마침표이면 해당 node를 삭제
    if (LCount(&new_id) != 0)
    {
        LFirst(&new_id, &data);
        if ((new_id.cur->data) == '.')
        {
            LRemove(&new_id);
            // 첫 data가 삭제됐을 경우 
            // 다시 list를 초기화하여 tail을 정상적으로 불러올 수 있도록 함
            LFirst(&new_id, &data);  
        }

        // tail이 마침표인 게 확인되면 해당 node를 삭제
        // - LCount가 1 이상인 경우에만 tail을 검사하도록 예외처리 
        // - 예외 상황) LCount가 1개이고 tail이 마침표인 경우에는 위 LFirst()에서 삭제했으므로 또 삭제하지 않도록 처리
        if ((LCount(&new_id) > 1) && (new_id.tail->data) == '.')
        {
            for (i = 0; i < LCount(&new_id) - 1; i++)
            {
                LNext(&new_id, &data);
     
                // 마지막 문자이면 
                if (i == LCount(&new_id)-2)
                {
                    LRemove(&new_id);
                }
            }
        }
    }

    // 5단계 new_id가 빈 문자열이라면, new_id에 "a"를 대입합니다.
    // - 빈 문자열을 입력받았거나, 위 단계들을 거쳐 리스트에 아무것도 남지 않은 경우 list에 'a'를 대입
    if(!arr_id[0] || LCount(&new_id) == 0)
    {
        LInsert(&new_id, 'a');
    }


    // 6단계(1) new_id의 길이가 16자 이상이면, 
    // new_id의 첫 15개의 문자를 제외한 나머지 문자들을 모두 제거합니다.
   
    int index = 0;  // 임시로 list의 index를 체크할 수 있는 변수 생성
    if(LCount(&new_id) >= 16)
    {
        if(LFirst(&new_id, &data))
        {
            index++;
            for(i = index; i <(LCount(&new_id)); index++)
            {
                // list 데이터 개수가 15개가 되면 break
                if(LCount(&new_id) < 16)
                    break;

                LNext(&new_id, &data);

                // 16번째 data부터 삭제
                if(index >= 15)
                {
                    LRemove(&new_id);
                }
            }
            // 6단계(2) 만약 제거 후 마침표(.)가 new_id의 끝에 위치한다면, 끝에 위치한 마침표(.) 문자를 제거합니다.
            // - tail의 data를 정상적으로 확인하기 위해 list를 초기화
            LFirst(&new_id, &data);
            if ((new_id.tail->data) == '.')
            {
                for (i = 0; i < LCount(&new_id) - 1; i++)
                {
                    LNext(&new_id, &data);
                    if (i == (LCount(&new_id) - 2))
                    {
                        LRemove(&new_id);
                    }
                }
            }
        }
    }

    // 7단계 new_id의 길이가 2자 이하라면, new_id의 마지막 문자를 new_id의 길이가 3이 될 때까지 반복해서 끝에 붙입니다.
    int lastText;
    if(LCount(&new_id) <= 2)
    {
        // tail의 data를 정상적으로 가져오기 위해 list를 초기화
        LFirst(&new_id, &data);
        // lastText 변수에 마지막 문자를 설정
        lastText = new_id.tail->data;
        // 길이가 3보다 작다면 반복해서 마지막 문자를 끝에 추가
        while(LCount(&new_id) < 3)
        {
            LInsert(&new_id, lastText);
        }
    }

    // 모든 데이터 출력
    PrintAllData(new_id);

    printf("\n\n");
}

// PrintAllData() : list 내의 모든 데이터를 출력하는 함수
void PrintAllData(List plist)
{
    int pdata;
    if(LFirst(&plist, &pdata))
    {
        printf("%c", pdata);
        for(int j=0; j<LCount(&plist)-1; j++)
        {
            if(LNext(&plist, &pdata))
            {
                printf("%c",pdata);
            }
        }
    }
    
}
