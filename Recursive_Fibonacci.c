/* 이름: 박민주
   학번: 201914302
   프로그램 작성일: 2021-03-20
   프로그램 설명: 재귀함수를 통해 피보나치 수열을 시간복잡도 O(n)으로 구현한 프로그램
   */

#include <stdio.h>

// <피보나치 함수>
// - 파라미터 설명 
//   index: 배열 인덱스 
//   result: 배열을 가리키는 포인터 
//
// - 피보나치 수열 연산 
//   a) n == 1이면 0을 반환
//   b) n == 2이면 1을 반환
//   c) n >= 3이면 
//    - 해당 인덱스의 배열 값을 체크
//    - 값이 있으면 배열 값을 반환하고, 
//      값이 없으면 재귀호출하여 연산 진행 후 배열에 저장 & 반환
int Fibo(int index, unsigned int *result)
{
    if(index == 1)
    {   return 0;   }
    else if(index == 2)
    {   return 1;   }
    else
    {
        if(result[index] != 0) // 값이 있으면
        {   return result[index];   }
        else // 값이 없으면
        {   return result[index] = Fibo(index-1, result) + Fibo(index-2, result);   }
    }
}

// <메인 함수>
// - 숫자를 입력받고 그 수만큼 피보나치 수열 값 출력
// - 배열 생성 및 초기화 진행
// - 반복문을 돌리면서 Fibo() 호출
int main(void)
{
    // 1. 숫자 입력 받기
    printf("피보나치 수열을 몇 번째까지 출력할까요? \n");
    printf("숫자를 입력하세요: ");
    
    int num = 0;
    scanf("%d", &num);
    
    // 2. 값을 저장할 배열 생성
    unsigned int r_arr[num+1];
    
    // 3. 배열을 0으로 초기화
    for(int i = 0; i<sizeof(r_arr)/sizeof(unsigned int); i++)
    {
        r_arr[i] = 0;
    }

    // 4. 피보나치 수열 출력
    for(int i = 1; i <= num; i++)
    {
        // 입력받은 숫자만큼 반복하여 Fibo()의 반환값 출력
        printf("%d ", Fibo(i, r_arr));
    }
    printf("\n");

    return 0;
}
