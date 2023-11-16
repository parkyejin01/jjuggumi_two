#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"

#define DIALOG_DURATION_SEC		4

char bye_p[9] = { ' ',',',' ',',',' ',',',' ',',',' ' };

int px[PLAYER_MAX], py[PLAYER_MAX], period[PLAYER_MAX];
int flag = 0;
int cam_catch = 0;
int hide = 0;
int no_dead = 0;
int next_game = 0;
int ticktick = 0;
int suc_p[PLAYER_MAX] = { 0 };

int speed[10] = { 10, 520, 660, 840, 1200, 1000, 800, 580, 200, 100 };
int spd_num = 0;
char blank[40] = "";

void p0_turn(void);
void mugunghwa_init(void);
void display_mgh(void);
void yh_turn(int, char[40]);
void cnt_alive(void);
void move_p0(key_t key);
void move_rnd(int i, int dir);
void moving(int i, int nx, int ny);



void mugunghwa_init(void)
{
	map_init(11, 35);

	for (int i = 0; i < n_player; i++)
	{
		if (n_player >= 9) { px[i] = 1 + i; }
		else if (n_player >= 7) { px[i] = 2 + i; }
		else { px[i] = 3 + i; }

		py[i] = N_COL - 2;
		period[i] = randint(20, 40);

		back_buf[px[i]][py[i]] = '0' + i;
	}

	for (int i = 0; i < n_player; i++)//플레이어 이동주기 각각 다르게 설정
	{
		for (int j = 0; j < n_player; j++)
		{
			if (period[i] == period[j])
			{
				period[j] = randint(20, 40);
			}
		}
	}


	for (int i = 4; i < 7; i++)
	{
		back_buf[i][1] = '#';//[3][1],[4][1],[5][1]술레 -> [2][1],[6][1],[3][2],[4][2],[5][2]통과
	}

	tick = 0;

}

void display_mgh(void)
{
	display();
	//3초가 끝나면 gotoxy(N_ROW + 1, 0); for(int i=0;i<20 or 31..){yh_say[i]=' ';}for(,,,,){printf("%c",yh_say[i])}

	//술래
	gotoxy(N_ROW + 1, 0);
	char yh_say[40] = "무궁화꽃이피었습니다";


	if (ticktick % speed[spd_num] == 0)
	{
		//-술래가 뒤를 돌았을 때
		if (spd_num == 9)
		{
			yh_turn(spd_num, yh_say);
			if (flag == 1)
			{
				return;
			}
			gotoxy(N_ROW + 1, 0);
			spd_num = 0;
			ticktick = 0;

			if (next_game == 1)
			{
				return;
			}
			if (n_alive == 1)//생존자 1명
			{
				return;
			}
		}

		for (int i = 0; i < (spd_num + 1) * 2; i += 2)
		{
			printf("%c%c ", yh_say[i], yh_say[i + 1]);
		}
		ticktick = 0;
		spd_num++;

	}

	ticktick += 10;
	//

	gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
	print_status();

	next_game = 1;
	for (int i = 1; i < 10; i++)
	{
		if (next_game == 0)
		{
			break;
		}
		else
		{
			for (int j = 1; j < 34; j++)
			{
				if (!((i == 4 || i == 5 || i == 6) && j == 1) && (back_buf[i][j] != ' '))
				{
					next_game = 0;
					break;
				}
			}
		}

	}
	if (next_game == 1)
	{
		return;
	}

}

void yh_turn(int sayend, char yh_say[40])
{
	for (int i = 4; i < 7; i++)
	{
		back_buf[i][1] = '@';
	}
	display();

	gotoxy(N_ROW + 1, 0);

	for (int i = 0; i < (sayend + 1) * 2; i += 2)
	{
		printf("%c%c ", yh_say[i], yh_say[i + 1]);
	}

	int sTime = (unsigned)time(NULL);
	Sleep(10);

	int yh_tick = 0;

	while (1)
	{
		int pTime = (unsigned)time(NULL);
		if ((pTime - sTime) == 3)
		{
			if (bye > 0)
			{
				dialog_mgh(bye_p);
				bye = 0;
			}

			for (int i = 4; i < 7; i++)
			{
				back_buf[i][1] = '#';
			}
			display();
			gotoxy(N_ROW + 1, 0);
			for (int i = 0; i < 40; i++)
			{
				blank[i] = ' ';
			}
			for (int i = 0; i < 40; i++)
			{
				printf("%c", blank[i]);
			}



			return;
		}

		//플레이어 0
		p0_turn();

		if (flag == 1)
		{
			return;
		}




		//0제외 플레이어

		for (int p = 1; p < n_player; p++)
		{
			/*if (back_buf[px[p] + 1][py[p]] == '#' || back_buf[px[p]][py[p] - 1] == '#' || back_buf[px[p] - 1][py[p]] == '#')
			{
				break;
			}*/


			cam_catch = 0;
			int rnd10 = randint(0, 999);

			if ((yh_tick % period[p] == 0) && rnd10 < 100 && suc_p[p] == 0 && player[p].is_alive == true)
			{
				move_rnd(p, -1);

				for (int i = 1; i <= py[p] - 1; i++)
				{
					if (cam_catch == 1)
						break;
					else
					{
						for (int j = 0; j < n_player; j++)
						{

							if ((back_buf[px[p]][py[p] - i] == '0' + j) && hide == 1)
							{
								cam_catch = 1;
								hide = 0;
								//player[p] = true;
								break;
							}
						}
					}

				}
				if ((cam_catch == 0) && (no_dead == 0))
				{
					player[p].is_alive = false;
					bye_p[bye] = '0' + p;
					bye += 2;
					//배열[변수]='0'+p;
					//변수 += 2;

					cnt_alive();
				}

			}
		}


		display();


		gotoxy(N_ROW + 4, 0);  // 추가로 표시할 정보가 있으면 맵과 상태창 사이의 빈 공간에 출력
		print_status();


		Sleep(10);
		yh_tick += 10;


		for (int p = 0; p < n_player; p++)
		{
			if (player[p].is_alive == false)
			{
				back_buf[px[p]][py[p]] = ' ';
				suc_p[p] = 1;
				display();
			}
		}


		next_game = 1;
		for (int i = 1; i < 10; i++)
		{
			if (next_game == 0)
			{
				break;
			}
			else
			{
				for (int j = 1; j < 34; j++)
				{
					if (!((i == 4 || i == 5 || i == 6) && j == 1) && (back_buf[i][j] != ' '))
					{
						next_game = 0;
						break;
					}
				}
			}

		}
		if (next_game == 1)
		{
			if (bye > 0)
			{
				dialog_mgh(bye_p);
				bye = 0;
			}
			return;
		}

		if (n_alive == 1)//생존자 1명
		{
			if (bye > 0)
			{
				dialog_mgh(bye_p);
				bye = 0;
			}
			return;
		}

	}

}

void cnt_alive(void)
{
	int sum = 0;
	for (int p = 0; p < n_player; p++)
	{
		sum += player[p].is_alive;
	}
	n_alive = sum;
}

void move_rnd(int player, int dir)
{
	int p = player;  // 이름이 길어서 p로 줄임
	int nx, ny;  // 움직여서 다음에 놓일 자리


	// 움직일 공간이 없는 경우는 없다고 가정(무한 루프에 빠짐)
	int count = 0;
	do {

		int rnd = randint(0, 999);

		if (rnd >= 0 && rnd < 700)
		{
			nx = px[p] + 0;
			ny = py[p] - 1;
			hide = 1;
			no_dead = 0;
		}
		else if (rnd < 800)
		{
			nx = px[p] - 1;
			ny = py[p] + 0;
			hide = 0;
			no_dead = 0;
		}
		else if (rnd < 900)
		{
			nx = px[p] + 1;
			ny = py[p] + 0;
			hide = 0;
			no_dead = 0;
		}
		else
		{
			nx = px[p] + 0;
			ny = py[p] + 0;
			hide = 0;
			no_dead = 1;
		}

		if (count > 5)//무한루프에 빠질 시, 무한루프에 빠진 플레이어는 제자리 유지하며 무한루프 탈출
		{
			nx = px[p] + 0;
			ny = py[p] + 0;
			no_dead = 1;
			return;
		}

		count++;

	} while (!placable(nx, ny));

	if (back_buf[px[p] + 1][py[p]] == '#' || back_buf[px[p]][py[p] - 1] == '#' || back_buf[px[p] - 1][py[p]] == '#')
	{
		suc_p[p] = 1;
	}

	moving(p, nx, ny);
}

// back_buf[][]에 기록
void moving(int player, int nx, int ny)
{
	int p = player;  // 이름이 길어서...
	back_buf[nx][ny] = back_buf[px[p]][py[p]];
	back_buf[px[p]][py[p]] = ' ';
	px[p] = nx;
	py[p] = ny;
}

void move_p0(key_t key)//방향키를 입력받았을 시 플레이어 0 행동
{
	int nx, ny;
	switch (key)
	{
	case K_UP: nx = px[0] - 1; ny = py[0] + 0; break;
	case K_DOWN: nx = px[0] + 1; ny = py[0] + 0; break;
	case K_LEFT: nx = px[0] + 0; ny = py[0] - 1; break;
	default: return;
	}
	if (!placable(nx, ny))
	{
		return;
	}
	if (back_buf[px[0] + 1][py[0]] == '#' || back_buf[px[0]][py[0] - 1] == '#' || back_buf[px[0] - 1][py[0]] == '#')
	{
		suc_p[0] = 1;
	}

	//술래 turn 때
	if ((spd_num == 9) && ((nx != px[0]) || (ny != py[0])) && suc_p[0] == 0 && player[0].is_alive == true)
	{
		cam_catch = 0;

		for (int i = 1; i <= py[0] - 1; i++)
		{
			if (cam_catch == 1)
				break;

			for (int j = 0; j < n_player; j++)
			{
				if ((back_buf[px[0]][py[0] - i] == '0' + j) && ((nx == px[0]) && (ny == py[0] - 1)))
				{
					cam_catch = 1;
					//player[0] = true;
					break;
				}
			}
		}
		if (cam_catch == 0)
		{
			player[0].is_alive = false;
			bye_p[bye] = '0';
			bye += 2;
			cnt_alive();
		}
	}

	moving(0, nx, ny);
}


void p0_turn(void)
{
	key_t key = get_key();
	if (key == K_QUIT)//Q입력 시 종료
	{
		flag = 1;
		return;
	}
	else if (key != K_UNDEFINED)
	{
		//방향키를 입력 받았을 시
		move_p0(key);
	}
}

void mugunghwa(void)
{
	//기본 맵 만들기
	mugunghwa_init();

	system("cls");
	display();
	//
	bye = 0;

	while (1)
	{
		//플레이어 0 조종
		p0_turn();
		if (flag == 1)
		{
			return;
		}
		//플레이어 0제외 무작위로 실행
		for (int i = 1; i < n_player; i++)
		{
			if (tick % period[i] == 0 && suc_p[i] == 0)
			{
				move_rnd(i, -1);
			}
		}


		display_mgh();
		if (flag == 1)
		{
			return;
		}
		if (next_game == 1)
		{
			return;
		}
		if (n_alive == 1)//생존자 1명
		{
			ending();
		}


		Sleep(10);
		tick += 10;

		for (int p = 0; p < n_player; p++)
		{
			if (back_buf[px[p] + 1][py[p]] == '#' || back_buf[px[p]][py[p] - 1] == '#' || back_buf[px[p] - 1][py[p]] == '#')
			{
				back_buf[px[p]][py[p]] = ' ';
				suc_p[p] = 1;
			}
		}


	}
}