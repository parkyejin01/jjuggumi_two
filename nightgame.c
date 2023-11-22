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


//�߰�� �� ����
void nightgame_init(void)
{
	//�� Ʋ �����
	map_init(10, 30);

	//�÷��̾� ��ġ ����
	for (int i = 0; i < n_player; i++)
	{
		if (player[i].is_alive == true)
		{
			px[i] = randint(1, 8);
			py[i] = randint(1, 28);

			//�ߺ����� ���� ��ġ ����
			while (back_buf[px[i]][py[i]] != ' ')
			{
				px[i] = randint(1, 8);
				py[i] = randint(1, 28);
			}

			back_buf[px[i]][py[i]] = '0' + i;//�÷��̾� ���� ��ġ�� �̵�

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
	//�߰�� ������ �迭 �ű��
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

		//�ߺ����� ���� ��ġ ����
		while (back_buf[itemx[i]][itemy[i]] != ' ')
		{
			itemx[i] = randint(1, 8);
			itemy[i] = randint(1, 28);
		}

		back_buf[itemx[i]][itemy[i]] = 'I';
	}


	tick = 0;
}


//����Ű �Է� �޾��� �� �÷��̾� 0 �ڵ�
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


//�÷��̾� 0������ �÷��̾� �ڵ�
void the_rest_player(int p)
{
	
	double shortest1 = INT_MAX;
	int index1;
	//�����۵� �� ���� ª�� �Ÿ�
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
	//����ִ� �÷��̾� ���̿��� ������ ������ �÷��̾�� �� ���� ª�� �Ÿ�
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

	//==>���ؼ� ���� ª�� �Ÿ��� �ִ� ������ �̵�
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


//�÷��̾� �̵� �� �ʿ� ���
void player_move_night(int p, int nx, int ny)
{
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}


//������ �Ǵ� �������� ������ �÷��̾�� ���� �ݺ����� �÷��̾� ���� �Ÿ�
//((������ �Ǵ� ������ ���� �÷��̾�)--�Ÿ�--(�ݺ������� ���� ���� �÷��̾�))
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

	if (xx > 0)//������ �Ǵ� ������ ���� �÷��̾ ����
	{
		if (yy > 0)//������ �Ǵ� ������ ���� �÷��̾ ����
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//������ �� ���̰� Ŭ ��
			{
				nx = px[p] - 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//������ �� ���̰� Ŭ ��
			{
				nx = px[p] + 0;
				ny = py[p] - 1;
			}
			else//���� ���� ���̰� ���� ��
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
		else//������ �Ǵ� ������ ���� �÷��̾ ������
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//������ �� ���̰� Ŭ ��
			{
				nx = px[p] - 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//�������� �� ���̰� Ŭ ��
			{
				nx = px[p] + 0;
				ny = py[p] + 1;
			}
			else//���� ���� ���̰� ���� ��
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
	else//������ �Ǵ� ������ ���� �÷��̾ �Ʒ���
	{
		if (yy > 0)//������ �Ǵ� ������ ���� �÷��̾ ����
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//�Ʒ����� �� ���̰� Ŭ ��
			{
				nx = px[p] + 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//������ �� ���̰� Ŭ ��
			{
				nx = px[p] + 0;
				ny = py[p] - 1;
			}
			else//�Ʒ��� ���� ���̰� ���� ��
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
		else//������ �Ǵ� ������ ���� �÷��̾ ������
		{
			if (sqrt(xx * xx) > sqrt(yy * yy))//�Ʒ����� �� ���̰� Ŭ ��
			{
				nx = px[p] + 1;
				ny = py[p] + 0;
			}
			else if (sqrt(xx * xx) < sqrt(yy * yy))//�������� �� ���̰� Ŭ ��
			{
				nx = px[p] + 0;
				ny = py[p] + 1;
			}
			else//�Ʒ��� ���� ���̰� ���� ��
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
		//���⿡ ������ ĭ�� �������� ���� �� ��ȣ�ۿ� �ڵ� �����
		for (int p = 0; p < n_player; p++)
		{
			//����ִ� �÷��̾� �� �������� ������ ������ ���� ��
			if ((player[p].is_alive == true))
			{
				if (back_buf[px[p] - 1][py[p]] == 'I' || back_buf[px[p] + 1][py[p]] == 'I' || back_buf[px[p]][py[p] - 1] == 'I' || back_buf[px[p]][py[p] + 1] == 'I')
				{
					if (player[p].hasitem == true)//�÷��̾ �������� ������ ���� ���
					{

					}
					else//�÷��̾ �������� ������ ���� ���� ���
					{
						player[p].hasitem == true;//������ ������ �ִٰ� ����
						for (int i = 0; i < count_item; i++)
						{
							if (back_buf[itemx[i]][itemy[i]] == 'I')
							{
								if ((itemx[i] == px[p] - 1) && (itemy[i] == py[p]))//���� ������
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p] + 1) && (itemy[i] == py[p]))//�Ʒ��� ������
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1))//���ʿ� ������
								{
									player[p].item = night_items[i];
									back_buf[itemx[i]][itemy[i]] = ' ';
									break;
								}
								else if ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1))//�����ʿ� ������
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

		//���⿡ ������ ĭ�� �÷��̾� ���� �� ��ȣ�ۿ� �ڵ� �����
		
		
		
		//Ű �Է� �ޱ�
		key_t key = get_key();
		if (key == K_QUIT)//Q�Է� �� ����
		{
			return;
		}
		else if (key != K_UNDEFINED)//����Ű �Է� �޾��� �� �÷��̾� 0 �ڵ�
		{
			if (player[0].is_alive == true)//�÷��̾� 0�� ������� �� ����
			{
				p0(key);
			}
		}

		//�÷��̾� 0������ �÷��̾� 
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