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


		display();

		tick += 10;
		Sleep(10);
	}
}