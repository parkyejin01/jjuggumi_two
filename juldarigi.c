#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


int jul;//�� ��� ���� ���� ��ġ ����
double fight_str;
int die;
int lie_left = 0;
int lie_right = 0;
double str_list[PLAYER_MAX];//�÷��̾� �� ��ȿ �� �迭
int juldarigi_fail[PLAYER_MAX];//�ٴٸ��� Ż�� �÷��̾� Ȯ�� �迭
char jul_fail_p[3] = { ' ', ', ', ' ' };//���̾�α׷� �� �� �ٴٸ��� Ż�� �÷��̾ ������ �迭

void juldarigi(void);
void juldarigi_init(void);
void stamin(void);

void jul_move(int);
void player_move(int);

void jul_move(int m)
{
	for (int i = 0; i < N_ROW; i++)//�ٴٸ��� �� �����
	{
		for (int j = 0; j < N_COL; j++)
		{
			if ((i == (N_ROW - 1) / 2) && ((j >= (N_COL - 1) / 2 - 1 + m) && (j <= (N_COL - 1) / 2 + 1 + m)))
			{
				back_buf[i][j] = '-';
			}
		}
	}
}

void player_move(int m)
{
	int x, y;
	int k1 = 2;
	int k2 = 2;
	for (int i = 0; i < n_player; i++)
	{
		x = (N_ROW - 1) / 2;

		if (i % 2 == 0)
		{
			y = (N_COL - 1) / 2 - k1 + m;
		}
		else
		{
			y = (N_COL - 1) / 2 + k2 + m;
		}


		if (juldarigi_fail[i] == 0)
		{
			back_buf[x][y] = '0' + i;
			if (i % 2 == 0)
			{
				k1++;
			}
			else
			{
				k2++;
			}
		}
	}
}


void juldarigi_init(void)//���� �� �����
{
	for (int p = 0; p < n_player; p++)//���� �÷��̾� flag���� �迭 �����ϰ� �츮��
	{
		if (player[p].is_alive == false)
		{
			died_player[p] = 1;
			player[p].hasitem = false;
			for (int i = 0; i < 100; i++)
			{
				player[p].item.name[i] = ' ';
			}
			player[p].item.intel_buf = player[p].item.str_buf = 0;//������ ���ֱ�
			player[p].item.stamina_buf = 0;
			player[p].is_alive = true;
		}
		else
		{
			died_player[p] = 0;
		}
		//�ٴٸ��� Ż�� �÷��̾� Ȯ�� �迭
		juldarigi_fail[p] = 0;
	}
	julmap_init(3, 32);//�� �׵θ�
	jul_move(0);//�ٴٸ��� �� �����

	//�÷��̾� �����
	player_move(0);

	//�÷��̾� �� ��ȿ �� �迭 ����
	for (int p = 0; p < n_player; p++)
	{
		str_list[p] = (player[p].str + player[p].item.str_buf) * (player[p].stamina + player[p].item.stamina_buf) / 100;
	}

}


void stamin(void)//���� ��ĥ ������ ������ ȸ��
{
	for (int p = 0; p < n_player; p++)
	{
		if (player[p].is_alive == true)
		{
			player[p].stamina += 40;
			if (player[p].stamina > 100)
			{
				player[p].stamina = 100;
			}
		}
	}
}



void juldarigi(void)
{
	juldarigi_init();
	system("cls");
	display_jul();
	jul = 0;//�� ������� ���� ��ġ
	int tick = 0;
	int plus_str = 0;

	while (1)
	{
		key_t key = juldarigi_get_key();
		if (key == K_QUIT)//Q�Է� �� ����
		{
			stamin();
			return;
		}

		
		
		if (tick <= 1000)
		{
			if (key != K_UNDEFINED)
			{
				//Ű ����
				switch (key)
				{
				case 'z': plus_str--; break;
				case '/': plus_str++; break;
				case 'x': break;
				case '.': break;
				default: break;
				}
			}
			double left_str = 0;
			double right_str = 0;
			for (int p = 0; p < n_player; p++)
			{
				if (juldarigi_fail[p] == 0)//����ִ� �÷��̾
				{
					if (p % 2 == 0)//���� �� ��ȿ�� ��
					{
						left_str += str_list[p];
					}
					else//������ �� ��ȿ�� ��
					{
						right_str += str_list[p];
					}
				}
			}
			fight_str = right_str - left_str + plus_str;//��ȿ ���� ��
			gotoxy(N_ROW + 2, 0);
			printf("str:\t%.1lf", fight_str);//��ȿ �� �� ǥ��
		}

		//1��
		if (tick == 1000)
		{
			die = 0;//Ż�� Ȯ�� ����
			//�� ��� ��ġ �� ����
			if (fight_str < 0)
			{
				jul--;
			}
			else if (fight_str > 0)
			{
				jul++;
			}

			
			if (jul < -1 || jul > 1)//�÷��̾ ���ۿ� ���� ��
			{
				//�� ����
				if (fight_str < 0)//���� �¸� ��
				{
					jul++;
				}
				else if (fight_str > 0)//������ �¸� ��
				{
					jul--;
				}

				//�÷��̾� Ż��
				for (int p = 0; p < n_player; p++)
				{
					if (juldarigi_fail[p] == 0)//Ż������ ���� �÷��̾� �߿���
					{
						if (fight_str < 0)//���� �¸� ��
						{
							if (p % 2 == 1) //������ �÷��̾� Ż��
							{
								die = 1;
							}
						}
						else if (fight_str > 0)//������ �¸� ��
						{
							if (p % 2 == 0)//���� �÷��̾� Ż��
							{
								die = 1;
							}
						}

						if (die == 1)//Ż���ڹ߻��� �� �Ͽ� 1��
						{
							juldarigi_fail[p] = 1;
							jul_fail_p[die - 1] = '0' + p;
							break;
						}
					}
				}
			}

			//�� �� ����ó��
			for (int i = 0; i < N_ROW; i++)
			{
				for (int j = 0; j < N_COL; j++)
				{
					if ((i == (N_ROW - 1) / 2) && ((j != 0) && (j != N_COL - 1)))
					{
						back_buf[i][j] = ' ';
					}
				}
			}



			//�ٰ� �÷��̾� ȭ�� �̵�
			jul_move(jul);
			player_move(jul);
			draw();
		}


		//2��
		if (tick == 2000)
		{
			//���̾�α� ���
			if (die > 0)
			{
				dialog_jul(jul_fail_p);

				//Ż���� ����
				int left = 0;
				int right = 0;
				for (int p = 0; p < n_player; p++)
				{
					if (juldarigi_fail[p] == 1)
					{
						if (p % 2 == 0)
						{
							left = 1;
						}
						else
						{
							right = 1;
						}
					}
					else
					{
						if (p % 2 == 0)
						{
							left = 0;
						}
						else
						{
							right = 0;
						}
					}

				}

				if (left == 1 || right == 1)
				{
					for (int p = 0; p < n_player; p++)
					{
						if ((left == 1) && (p % 2 == 0))
						{
							if (died_player[p] == 1)
							{
								player[p].is_alive = false;
							}
							else
							{
								player[p].hasitem = false;
								for (int i = 0; i < 100; i++)
								{
									player[p].item.name[i] = ' ';
								}
								player[p].item.intel_buf = player[p].item.str_buf = player[p].item.stamina_buf = 0;
								player[p].intel /= 2;
								player[p].str /= 2;
							}
						}
						else if ((right == 1) && (p % 2 == 1))
						{
							if (died_player[p] == 1)
							{
								player[p].is_alive = false;
							}
							else
							{
								player[p].hasitem = false;
								for (int i = 0; i < 100; i++)
								{
									player[p].item.name[i] = ' ';
								}
								player[p].item.intel_buf = player[p].item.str_buf = player[p].item.stamina_buf = 0;
								player[p].intel /= 2;
								player[p].str /= 2;
							}
						}
						else
						{
							if (died_player[p] == 1)
							{
								n_alive++;
							}
						}
					}
					stamin();
					break;
				}
			}
			
			
			tick = 0;
			plus_str = 0;
		}

		display_jul();
		//���� ����
		tick += 10;
		Sleep(10);
	}
}