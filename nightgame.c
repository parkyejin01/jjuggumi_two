#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include<math.h>
#include<limits.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


int px[PLAYER_MAX] = { 0 }, py[PLAYER_MAX] = { 0 };
int period[PLAYER_MAX] = { 0 };
int count_item;
int item_limit[ITEM_MAX];
int itemx[ITEM_MAX] = { 0 }, itemy[ITEM_MAX] = { 0 };
int nx, ny;
ITEM night_items[ITEM_MAX] = { 0 };

void nightgame_init(void);
void p0(key_t);
void player_move_night(int, int, int);
void the_rest_player(int);
double get_distance(int, int, int, int);
void shortmove(int, int, int);


//야간운동 맵 생성
void nightgame_init(void)
{
	//맵 틀 만들기
	map_init(10, 30);

	//플레이어 위치 랜덤
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			px[i] = randint(1, 8);
			py[i] = randint(1, 28);

			//중복되지 않은 위치 설정
			while (back_buf[px[i]][py[i]] != ' ')
			{
				px[i] = randint(1, 8);
				py[i] = randint(1, 28);
			}

			back_buf[px[i]][py[i]] = '0' + i;//플레이어 시작 위치로 이동

			period[i] = randint(100, 300);
		}
	}
	
	
	count_item = 0;
	while (count_item != n_alive - 1)
	{
		count_item = 0;

		for (int i = 0; i < n_item; i++)
		{
			item_limit[i] = randint(0, 1);
			
			if (item_limit[i] == 1)
			{
				count_item++;
			}
		}
	}

	int k = 0;
	//야간운동 아이템 배열 옮기기
	for (int i = 0; i < n_item; i++)
	{
		if (item_limit[i] == 1)
		{
			night_items[k] = item[i];
			k++;
		}
	}

	
	for (int i = 0; i < count_item; i++)
	{
		itemx[i] = randint(1, 8);
		itemy[i] = randint(1, 28);

		//중복되지 않은 위치 설정
		while (back_buf[itemx[i]][itemy[i]] != ' ')
		{
			itemx[i] = randint(1, 8);
			itemy[i] = randint(1, 28);
		}

		back_buf[itemx[i]][itemy[i]] = 'I';
	}


	tick = 0;
}


//방향키 입력 받았을 시 플레이어 0 코드
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


//플레이어 0제외한 플레이어 코드
void the_rest_player(int p)
{
	
	double shortest1 = INT_MAX;
	int index1;
	//아이템들 중 가장 짧은 거리
	for (int i = 0; i < count_item; i++)
	{
		if (back_buf[itemx[i]][itemy[i]] == 'I')
		{
			double len = get_distance(px[p], py[p], itemx[i], itemy[i]);

			if (len < shortest1)
			{
				shortest1 = len;
				index1 = i;
			}
		}
	}

	double shortest2 = INT_MAX;
	int index2;
	//살아있는 플레이어 사이에서 아이템 소유한 플레이어들 중 가장 짧은 거리
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].hasitem == true && player[i].is_alive == true)
		{
			double len = get_distance(px[p], py[p], px[i], py[i]);

			if (len < shortest2)
			{
				shortest2 = len;
				index2 = i;
			}
		}
	}

	//==>비교해서 가장 짧은 거리에 있는 쪽으로 이동
	if (shortest1 < shortest2)
	{
		shortmove(p, px[p] - itemx[index1], py[p] - itemy[index1]);
	}
	else if(shortest1 > shortest2)
	{
		shortmove(p, px[p] - px[index2], py[p] - py[index2]);
	}
	else
	{
		int rnd = randint(1, 2);
		if (rnd == 1)
		{
			shortmove(p, px[p] - itemx[index1], py[p] - itemy[index1]);
		}
		else
		{
			shortmove(p, px[p] - px[index2], py[p] - py[index2]);
		}
	}
	if (!placable(nx, ny))
	{
		return;
	}

	player_move_night(p, nx, ny);

}


//플레이어 이동 후 맵에 백업
void player_move_night(int p, int nx, int ny)
{
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}


//아이템 또는 아이템을 소유한 플레이어와 현재 반복문의 플레이어 사이 거리
//((아이템 또는 아이템 소유 플레이어)--거리--(반복문에서 현재 차례 플레이어))
double get_distance(int x1, int y1, int x2, int y2)
{
	int dx = x1 - x2;
	int dy = y1 - y2;
	int xx = dx * dx;
	int yy = dy * dy;

	double d = sqrt(xx + yy);

	return d;
}

void shortmove(int p, int xx, int yy)
{

	if (xx > 0)//아이템 또는 아이템 소유 플레이어가 위쪽
	{
		if (yy > 0)//아이템 또는 아이템 소유 플레이어가 왼쪽
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//위쪽이 더 차이가 클 때
			{
				nx = px[p] - 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//왼쪽이 더 차이가 클 때
			{
				nx = px[p] + 0;
				ny = py[p] - 1;
			}
			else//위쪽 왼쪽 차이가 같을 때
			{
				int rnd = randint(1, 2);
				if (rnd == 1)
				{
					nx = px[p] - 1;
					ny = py[p] + 0;
				}
				else
				{
					nx = px[p] + 0;
					ny = py[p] - 1;
				}
			}
		}
		else//아이템 또는 아이템 소유 플레이어가 오른쪽
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//위쪽이 더 차이가 클 때
			{
				nx = px[p] - 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//오른쪽이 더 차이가 클 때
			{
				nx = px[p] + 0;
				ny = py[p] + 1;
			}
			else//위쪽 왼쪽 차이가 같을 때
			{
				int rnd = randint(1, 2);
				if (rnd == 1)
				{
					nx = px[p] - 1;
					ny = py[p] + 0;
				}
				else
				{
					nx = px[p] + 0;
					ny = py[p] + 1;
				}
			}
		}
	}
	else//아이템 또는 아이템 소유 플레이어가 아래쪽
	{
		if (yy > 0)//아이템 또는 아이템 소유 플레이어가 왼쪽
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//아래쪽이 더 차이가 클 때
			{
				nx = px[p] + 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//왼쪽이 더 차이가 클 때
			{
				nx = px[p] + 0;
				ny = py[p] - 1;
			}
			else//아래쪽 왼쪽 차이가 같을 때
			{
				int rnd = randint(1, 2);
				if (rnd == 1)
				{
					nx = px[p] + 1;
					ny = py[p] + 0;
				}
				else
				{
					nx = px[p] + 0;
					ny = py[p] - 1;
				}
			}
		}
		else//아이템 또는 아이템 소유 플레이어가 오른쪽
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//아래쪽이 더 차이가 클 때
			{
				nx = px[p] + 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//오른쪽이 더 차이가 클 때
			{
				nx = px[p] + 0;
				ny = py[p] + 1;
			}
			else//아래쪽 왼쪽 차이가 같을 때
			{
				int rnd = randint(1, 2);
				if (rnd == 1)
				{
					nx = px[p] + 1;
					ny = py[p] + 0;
				}
				else
				{
					nx = px[p] + 0;
					ny = py[p] + 1;
				}
			}
		}
	}


}


void nightgame(void)
{
	nightgame_init();
	system("cls");
	display();

	while (1)
	{
		//여기에 인접한 칸에 아이템이 있을 때 상호작용 코드 만들기
		for (int p = 0; p < n_player; p++)
		{
			//살아있는 플레이어 중 아이템이 인접한 공간에 있을 때
			if ((player[p].is_alive == true))
			{
				if (back_buf[px[p] - 1][py[p]] == 'I' || back_buf[px[p] + 1][py[p]] == 'I' || back_buf[px[p]][py[p] - 1] == 'I' || back_buf[px[p]][py[p] + 1] == 'I')
				{
					if (player[p].hasitem == true)//플레이어가 아이템을 가지고 있을 경우
					{

					}
					else//플레이어가 아이템을 가지고 있지 않은 경우
					{
						player[p].hasitem == true;//아이템 가지고 있다고 설정
						for (int i = 0; i < count_item; i++)
						{
							if (back_buf[itemx[i]][itemy[i]] == 'I')
							{
								if ((itemx[i] == px[p] - 1) && (itemy[i] == py[p]))//위에 아이템
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p] + 1) && (itemy[i] == py[p]))//아래에 아이템
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1))//왼쪽에 아이템
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1))//오른쪽에 아이템
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
							}
						}

					}
				}

			}
		}

		//여기에 인접한 칸에 플레이어 있을 때 상호작용 코드 만들기
		
		
		
		//키 입력 받기
		key_t key = get_key();
		if (key == K_QUIT)//Q입력 시 종료
		{
			return;
		}
		else if (key != K_UNDEFINED)//방향키 입력 받았을 시 플레이어 0 코드
		{
			if (player[0].is_alive == true)//플레이어 0이 살아있을 때 실행
			{
				p0(key);
			}
		}

		//플레이어 0제외한 플레이어 
		for (int p = 1; p < n_player; p++)
		{
			if (player[p].is_alive == true)
			{
				if (tick % period[p] == 0)
				{
					the_rest_player(p);
				}
			}
		}


		
		

		display();

		tick += 10;
		Sleep(10);


		



	}
}