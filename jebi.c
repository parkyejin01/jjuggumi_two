#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<math.h>
#include<limits.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


void jebi_init(void);


void jebi_init(void)
{
	map_init(6, 25);


	tick = 0;
}



void jebi(void)
{
	jebi_init();
	system("cls");
	display();

	while (n_alive != 1)
	{
		//방향키로 제비 선택하고 스페이스바를 눌렀을 때 플레이어 통과 여부 결정하고 다음 플레이어에게 턴 넘기기
		//참가자 순서대로 진행하며 fail제비를 뽑으면 탈락하고 바로 다음 라운드 진행하게 한다. 
		//fail제비는 라운드마다 한개씩만! 나머지는 pass제비
		//반복문 루프 계속해서 돌면서 좌, 우, 스페이스, q 키 입력받도록 설정
		//fail제비가 나오면 다음 라운드로 바꿔주고, 매 라운드마다 제비 설정하고
		//스페이스 바가 눌리면, pss fail여부 확인하고 다이얼로그 띄워준 후에 다음 플레이어로 넘긴다!
		//맵 아래 쪽에 N_ROW +1,0으로 gotoxy 사용해 이동하고 현재 라운드, 현재 플레이어를 띄워준다. 
		//좌, 우 키는 입력 받으면 현재 있던 '@'를 '?'로 바꿔준 후에 +-2칸 이동해서 '?'를 '@'로 바꿔준다. 
		//이때 좌, 우 끝 위치 설정해서 현재 '@'가 좌 또는 우의 끝에 있을 때, 공간을 벗어나려고 하면 이동하지 못하게 처리한다. 


		display();

		tick += 10;
		Sleep(10);
	}
}