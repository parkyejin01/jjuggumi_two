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

//�߰�� �� ����
void nightgame_init(void)
{
	//�� Ʋ �����
	map_init(11, 35);

	//�÷��̾� ��ġ ����
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			px[i] = randint(1, 9);
			py[i] = randint(1, 33);

			//�ߺ����� ���� ��ġ ����
			while (back_buf[px[i]][py[i]] != ' ')
			{
				px[i] = randint(1, 9);
				py[i] = randint(1, 33);
			}

			back_buf[px[i]][py[i]] = '0' + i;//�÷��̾� ���� ��ġ�� �̵�
		}
	}
	

	
	for (int i = 0; i < n_item; i++)
	{
		itemx[i] = randint(1, 9);
		itemy[i] = randint(1, 33);
		
		//�ߺ����� ���� ��ġ ����
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
		if (key == K_QUIT)//Q�Է� �� ����
		{
			return;
		}
		else if (key != K_UNDEFINED)//����Ű �Է� �޾��� ��
		{
			p0(key);
		}


		

		display();

		tick += 10;
		Sleep(10);
	}
}