#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <string.h>
#include "canvas.h"
#include "jjuggumi.h"
#include "keyin.h"


int jul;//줄 가운데 정렬 기준 위치 변수
double fight_str;
int left_lie = 0;
int left_flag = 0;
int right_lie = 0;
int right_flag = 0;
int use_lie_l = 0;
int use_lie_r = 0;


int die;
double str_list[PLAYER_MAX];//플레이어 별 유효 힘 배열
int juldarigi_fail[PLAYER_MAX];//줄다리기 탈락 플레이어 확인 배열
char jul_fail_p[3] = { ' ', ',', ' ' };//다이얼로그로 매 판 줄다리기 탈락 플레이어를 전달할 배열

void juldarigi(void);
void juldarigi_init(void);
void stamin(void);
void lie_stamin(void);

void jul_move(int);
void player_move(int);

void jul_move(int m)
{
	for (int i = 0; i < N_ROW; i++)//줄다리기 줄 만들기
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


void juldarigi_init(void)//시작 맵 만들기
{
	for (int p = 0; p < n_player; p++)//죽은 플레이어 flag변수 배열 설정하고 살리기
	{
		if (player[p].is_alive == false)
		{
			died_player[p] = 1;
			player[p].hasitem = false;
			for (int i = 0; i < 100; i++)
			{
				player[p].item.name[i] = ' ';
			}
			player[p].item.intel_buf = player[p].item.str_buf = 0;//아이템 없애기
			player[p].item.stamina_buf = 0;
			player[p].is_alive = true;
		}
		else
		{
			died_player[p] = 0;
		}
		//줄다리기 탈락 플레이어 확인 배열
		juldarigi_fail[p] = 0;
	}
	julmap_init(3, 32);//맵 테두리
	jul_move(0);//줄다리기 줄 만들기

	//플레이어 세우기
	player_move(0);

	//플레이어 별 유효 힘 배열 설정
	for (int p = 0; p < n_player; p++)
	{
		str_list[p] = (player[p].str + player[p].item.str_buf) * (player[p].stamina + player[p].item.stamina_buf) / 100;
	}

}


void stamin(void)//게임 마칠 때마다 일정량 회복
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

//눕기를 쓴 플레이어는 스테미나 -30 감소
void lie_stamin(void)
{
	if (left_lie == 1 || right_lie == 1)
	{
		for (int p = 0; p < n_player; p++)
		{
			if (juldarigi_fail[p] == 0)
			{
				if (left_lie == 1 && p % 2 == 0 && use_lie_l == 0)
				{
					player[p].stamina -= 30;
				}
				else if (right_lie == 1 && p % 2 == 1 && use_lie_r == 0)
				{
					player[p].stamina -= 30;
				}
				if (player[p].stamina < 0)
				{
					player[p].stamina = 0;
				}
			}
		}
		if (left_lie == 1 && use_lie_l == 0)
		{
			//다이얼로그 출력
			char arrleft[] = "left";
			dialog_juldarigi_lie(arrleft);
			use_lie_l = 1;
		}
		if (right_lie == 1 && use_lie_r == 0)
		{
			//다이얼로그 출력
			char arrright[] = "right";
			dialog_juldarigi_lie(arrright);
			use_lie_r = 1;
		}
	}
}


void juldarigi(void)
{
	juldarigi_init();
	system("cls");
	display_jul();
	jul = 0;//줄 가운데정렬 기준 위치
	int tick = 0;
	int plus_str = 0;

	while (1)
	{
		key_t key = juldarigi_get_key();
		if (key == K_QUIT)//Q입력 시 종료
		{
			stamin();
			return;
		}

		
		
		if (tick <= 1000)
		{
			if (key != K_UNDEFINED)
			{
				//키 지정
				switch (key)
				{
				case 'z': plus_str--; break;
				case '/': plus_str++; break;
				case 'x': left_lie = 1; break;
				case '.': right_lie = 1; break;
				default: break;
				}
			}

			//눕기 사용시 다이얼로그, 스테미나 감소
			lie_stamin();

			double left_str = 0;
			double right_str = 0;
			for (int p = 0; p < n_player; p++)
			{
				if (juldarigi_fail[p] == 0)//살아있는 플레이어만
				{
					if (p % 2 == 0)//왼쪽 팀 유효힘 합
					{
						left_str += str_list[p];
					}
					else//오른쪽 팀 유효힘 합
					{
						right_str += str_list[p];
					}
				}
			}

			if (left_lie == 1)
			{
				left_str *= 2;
			}
			
			if (right_lie == 1)
			{
				right_str *= 2;
			}

			fight_str = right_str - left_str + plus_str;//유효 힘의 합
			gotoxy(N_ROW + 2, 0);
			printf("str:\t%.1lf", fight_str);//유효 힘 합 표시
		}

		//1초
		if (tick == 1000)
		{
			bye = 0;//탈락 확인 변수
			//줄 가운데 위치 값 조정
			if (fight_str < 0)
			{
				jul--;
				if (left_lie == 1)
				{
					jul--;
				}
			}
			else if (fight_str > 0)
			{
				jul++;
				if (right_lie == 1)
				{
					jul++;
				}
			}

			
			if (jul < -1 || jul > 1)//플레이어가 구멍에 빠질 때
			{
				//줄 고정
				if (fight_str < 0)//왼쪽 승리 시
				{
					jul++;
					if (left_lie == 1 && jul < -1)//눕기 시 오른쪽팀 플레이어 탈락자가 두명일 때
					{
						jul++;
						left_flag = 1;
					}
				}
				else if (fight_str > 0)//오른쪽 승리 시
				{
					jul--;
					if (right_lie == 1 && jul > 1)//눕기 시 왼쪽팀 플레이어 탈락자가 두명일 때
					{
						jul--;
						right_flag = 1;
					}
				}

				die = 0;
				//플레이어 탈락
				for (int p = 0; p < n_player; p++)
				{
					if (juldarigi_fail[p] == 0)//탈락하지 않은 플레이어 중에서
					{
						if (fight_str < 0)//왼쪽 승리 시
						{
							if (p % 2 == 1) //오른쪽 플레이어 탈락
							{
								bye ++;
							}
						}
						else if (fight_str > 0)//오른쪽 승리 시
						{
							if (p % 2 == 0)//왼쪽 플레이어 탈락
							{
								bye ++;
							}
						}

						if (bye == 1 && (left_flag == 0 && right_flag == 0))//탈락자발생은 한 턴에 1명만
						{
							juldarigi_fail[p] = 1;
							jul_fail_p[die] = '0' + p;
							break;
						}
						else if (bye <= 2)
						{
							juldarigi_fail[p] = 1;
							jul_fail_p[die] = '0' + p;
							die += 2;
							if (die == 4)
							{
								break;
							}
						}
					}
				}
			}

			//맵 안 리셋처리
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



			//줄과 플레이어 화면 이동
			jul_move(jul);
			player_move(jul);
			draw();
		}


		//2초
		if (tick == 2000)
		{
			//다이얼로그 출력
			if (bye > 0)
			{
				dialog_jul(jul_fail_p);
				left_lie = 0;
				right_lie = 0;
				left_flag = 0;
				right_flag = 0;
				use_lie_l = 0;
				use_lie_r = 0;

				//탈락자 정산
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
		//루프 설정
		tick += 10;
		Sleep(10);
	}
}