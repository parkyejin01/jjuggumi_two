#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


int px[PLAYER_MAX] = { 0 }, py[PLAYER_MAX] = { 0 };
int itemx[ITEM_MAX], itemy[ITEM_MAX];



void nightgame_init(void);
void p0(key_t);
void player_move_night(int, int, int);

//야간운동 맵 생성
void nightgame_init(void)
{
	//맵 틀 만들기
	map_init(11, 35);

	//플레이어 위치 랜덤
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			px[i] = randint(1, 9);
			py[i] = randint(1, 33);

			//중복되지 않은 위치 설정
			while (back_buf[px[i]][py[i]] != ' ')
			{
				px[i] = randint(1, 9);
				py[i] = randint(1, 33);
			}

			back_buf[px[i]][py[i]] = '0' + i;//플레이어 시작 위치로 이동
		}
	}
	

	
	for (int i = 0; i < n_item; i++)
	{
		itemx[i] = randint(1, 9);
		itemy[i] = randint(1, 33);
		
		//중복되지 않은 위치 설정
		while (back_buf[itemx[i]][itemy[i]] != ' ')
		{
			itemx[i] = randint(1, 9);
			itemy[i] = randint(1, 33);
		}

		back_buf[itemx[i]][itemy[i]] = 'I';
	}


	tick = 0;
}

void p0(key_t key)
{
	int nx, ny;
	switch (key)
	{
	case K_UP: nx = px[0] - 1; ny = py[0] + 0; break;
	case K_DOWN: nx = px[0] + 1; ny = py[0] + 0; break;
	case K_LEFT: nx = px[0] + 0; ny = py[0] - 1; break;
	case K_RIGHT: nx = px[0] + 0; ny = py[0] + 1; break;
	default: return;
	}
	if (!placable(nx, ny))
	{
		return;
	}

	player_move_night(0, nx, ny);
}

void player_move_night(int p, int nx, int ny)
{
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void nightgame(void)
{
	nightgame_init();
	system("cls");
	display();

	while (1)
	{
		key_t key = get_key();
		if (key == K_QUIT)//Q입력 시 종료
		{
			return;
		}
		else if (key != K_UNDEFINED)//방향키 입력 받았을 시
		{
			p0(key);
		}


		

		display();

		tick += 10;
		Sleep(10);
	}
}