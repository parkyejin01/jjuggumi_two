#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<math.h>
#include<limits.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


int jebi_round = 1;
int point = 2;
int jebi_list[PLAYER_MAX] = { 0 };//제비 리스트 //만약 space바가 눌리면 (jebi_point == point)일때 fail처리하고 다시 제비 생성=>다음 라운드round++;
												//(jebi_point != point)일때, point<jebi_point이면 jebi_point를 -2하고 jebi_list[point/2-1] 부터 jebi_list[(n_alive-1-k)*2]까지(point/2-1<=  <=(n_alive-1-k)*2까지) jebi_list[i] = jebi_list[i+1]
												//point>jebi_point이면 그대로 두어도 상관없음(제비 위치에는 변동이 없기 때문에!) //그리고 k위치 바꿔주기 전에 back_buf[(n_alive-1-k)*2]=' '로 바꿔주고 k++해준다. 
int jebi_player[PLAYER_MAX];//라운드 마다 n_alive까지 생존 플레이어 저장해서 현재 제비 뽑는 플레이어 바뀔 때(player_point++;) jebi_player[player_point]로 플레이어를 바꿔준다. 
int player_point = 0;//매 라운드마다 player_point=0지정 //space바가 눌릴 때마다 player_point++;해준다. (얘가 k)
int count_pass = 0;
int jebi_point = 0;//제비 리스트에서 fail제비의 y값 위치
char space_player[1] = { ' ' };


void jebi_init(void);
void reset_jebi(void);
void space_key(void);


void jebi_init(void)//제비 맵 시작
{
	map_init(7, 22);

	for (int i = 0; i < n_alive; i++)
	{
		if (i == 0)
		{
			back_buf[3][2 + (i * 2)] = '@';
		}
		else
		{
			back_buf[3][2 + (i * 2)] = '?';
		}
	}

	reset_jebi();
	
	tick = 0;
}


void reset_jebi(void)
{
	point = 2;

	
	for (int i = 0; i < n_alive; i++)
	{
		if (i == 0)
		{
			back_buf[3][2] = '@';
		}
		else
		{
			back_buf[3][2 + (i * 2)] = '?';
		}
	}

	
	int k = 0;
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			jebi_player[k] = i;
			k++;
		}
	}
	
	count_pass = 0;
	while (count_pass != n_alive - 1)
	{
		count_pass = 0;

		for (int i = 0; i < n_alive; i++)
		{
			jebi_list[i] = randint(0, 1);

			if (jebi_list[i] == 1)
			{
				count_pass++;
			}
			else
			{
				jebi_point = 2 + (i * 2);
			}
		}
	}

}

void space_key(void)
{
	if (jebi_point == point)
	{
		back_buf[3][2 + (n_alive -1) * 2] = ' ';
		player[jebi_player[player_point]].is_alive = false;
		n_alive--;
		space_player[0] = '0' + jebi_player[player_point];
		dialog_jebi(space_player, "fail");
		jebi_round++;
		player_point = 0;
		reset_jebi();
	}
	else if (point < jebi_point)
	{
		jebi_point -= 2;
		back_buf[3][(n_alive - player_point) * 2] = ' ';
		space_player[0] = '0' + jebi_player[player_point];
		dialog_jebi(space_player, "pass");
		player_point++;
		for (int i = point / 2 - 1; i < (n_alive - player_point); i++)
		{
			jebi_list[i] = jebi_list[i + 1];
		}
	}
	else//point > jebi_point
	{
		back_buf[3][(n_alive - player_point) * 2] = ' ';
		space_player[0] = '0' + jebi_player[player_point];
		dialog_jebi(space_player, "pass");
		player_point++;
	}
}


void jebi(void)
{
	jebi_init();
	system("cls");
	display();

	while (n_alive != 1)
	{
		key_t key = get_key();
		if (key == K_QUIT)//Q입력 시 종료
		{
			return;
		}
		else if (key != K_UNDEFINED)
		{
			switch (key)
			{
			case K_SPACE: space_key(); break;
			case K_LEFT: back_buf[3][point] = '?'; point -= 2; break;
			case K_RIGHT: back_buf[3][point] = '?'; point += 2; break;
			default: break;
			}
			if (point < 2 || point > (n_alive - player_point) * 2)
			{
				if (point == 0)
				{
					point += 2;
				}
				else
				{
					point -= 2;
				}
			}

			back_buf[3][point] = '@';
		}
		//방향키로 제비 선택하고 스페이스바를 눌렀을 때 플레이어 통과 여부 결정하고 다음 플레이어에게 턴 넘기기
		//참가자 순서대로 진행하며 fail제비를 뽑으면 탈락하고 바로 다음 라운드 진행하게 한다. 
		//fail제비는 라운드마다 한개씩만! 나머지는 pass제비
		//반복문 루프 계속해서 돌면서 좌, 우, 스페이스, q 키 입력받도록 설정
		//fail제비가 나오면 다음 라운드로 바꿔주고, 매 라운드마다 제비 설정하고
		//스페이스 바가 눌리면, pss fail여부 확인하고 다이얼로그 띄워준 후에 다음 플레이어로 넘긴다!
		//맵 아래 쪽에 N_ROW +1,0으로 gotoxy 사용해 이동하고 현재 라운드, 현재 플레이어를 띄워준다. 
		//좌, 우 키는 입력 받으면 현재 있던 '@'를 '?'로 바꿔준 후에 +-2칸 이동해서 '?'를 '@'로 바꿔준다. 
		//이때 좌, 우 끝 위치 설정해서 현재 '@'가 좌 또는 우의 끝에 있을 때, 공간을 벗어나려고 하면 이동하지 못하게 처리한다. 
		//이것은 py의 위치가 지정해놓은 처음 제비 시작 열(에: 2)부터 마지막 제비 열(n_alive-1)까지 매 라운드마다 받고, 
		//이때 플레이어 턴들이 계속 돌아가면서 제비의 수가 점점 줄기 때문에 플레이어 턴이 돌 때마다(스페이스 바가 눌릴 때마다) 변수 k를 설정하여 k=0부터 시작한것을 k++; 시켜서
		//(n_alive-1-k)*2 까지 받는다. 라운드가 돌아갈때마다 k=0으로 초기화 시켜준다!
		//제비는 한 (3,2)쯤부터 하면 될듯
		//jebi_point(fail제비 포인트) == point == (상자 포인트..?)
		
		

		gotoxy(N_ROW + 1, 0);
		printf("round %d, turn: player %d", jebi_round, jebi_player[player_point]);
		display();

		tick += 10;
		Sleep(10);
	}
}