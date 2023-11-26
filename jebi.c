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
int jebi_list[PLAYER_MAX] = { 0 };//���� ����Ʈ //���� space�ٰ� ������ (jebi_point == point)�϶� failó���ϰ� �ٽ� ���� ����=>���� ����round++;
												//(jebi_point != point)�϶�, point<jebi_point�̸� jebi_point�� -2�ϰ� jebi_list[point/2-1] ���� jebi_list[(n_alive-1-k)*2]����(point/2-1<=  <=(n_alive-1-k)*2����) jebi_list[i] = jebi_list[i+1]
												//point>jebi_point�̸� �״�� �ξ �������(���� ��ġ���� ������ ���� ������!) //�׸��� k��ġ �ٲ��ֱ� ���� back_buf[(n_alive-1-k)*2]=' '�� �ٲ��ְ� k++���ش�. 
int jebi_player[PLAYER_MAX];//���� ���� n_alive���� ���� �÷��̾� �����ؼ� ���� ���� �̴� �÷��̾� �ٲ� ��(player_point++;) jebi_player[player_point]�� �÷��̾ �ٲ��ش�. 
int player_point = 0;//�� ���帶�� player_point=0���� //space�ٰ� ���� ������ player_point++;���ش�. (�갡 k)
int count_pass = 0;
int jebi_point = 0;//���� ����Ʈ���� fail������ y�� ��ġ
char space_player[1] = { ' ' };


void jebi_init(void);
void reset_jebi(void);
void space_key(void);


void jebi_init(void)//���� �� ����
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
		if (key == K_QUIT)//Q�Է� �� ����
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
		//����Ű�� ���� �����ϰ� �����̽��ٸ� ������ �� �÷��̾� ��� ���� �����ϰ� ���� �÷��̾�� �� �ѱ��
		//������ ������� �����ϸ� fail���� ������ Ż���ϰ� �ٷ� ���� ���� �����ϰ� �Ѵ�. 
		//fail����� ���帶�� �Ѱ�����! �������� pass����
		//�ݺ��� ���� ����ؼ� ���鼭 ��, ��, �����̽�, q Ű �Է¹޵��� ����
		//fail���� ������ ���� ����� �ٲ��ְ�, �� ���帶�� ���� �����ϰ�
		//�����̽� �ٰ� ������, pss fail���� Ȯ���ϰ� ���̾�α� ����� �Ŀ� ���� �÷��̾�� �ѱ��!
		//�� �Ʒ� �ʿ� N_ROW +1,0���� gotoxy ����� �̵��ϰ� ���� ����, ���� �÷��̾ ����ش�. 
		//��, �� Ű�� �Է� ������ ���� �ִ� '@'�� '?'�� �ٲ��� �Ŀ� +-2ĭ �̵��ؼ� '?'�� '@'�� �ٲ��ش�. 
		//�̶� ��, �� �� ��ġ �����ؼ� ���� '@'�� �� �Ǵ� ���� ���� ���� ��, ������ ������� �ϸ� �̵����� ���ϰ� ó���Ѵ�. 
		//�̰��� py�� ��ġ�� �����س��� ó�� ���� ���� ��(��: 2)���� ������ ���� ��(n_alive-1)���� �� ���帶�� �ް�, 
		//�̶� �÷��̾� �ϵ��� ��� ���ư��鼭 ������ ���� ���� �ٱ� ������ �÷��̾� ���� �� ������(�����̽� �ٰ� ���� ������) ���� k�� �����Ͽ� k=0���� �����Ѱ��� k++; ���Ѽ�
		//(n_alive-1-k)*2 ���� �޴´�. ���尡 ���ư������� k=0���� �ʱ�ȭ �����ش�!
		//����� �� (3,2)����� �ϸ� �ɵ�
		//jebi_point(fail���� ����Ʈ) == point == (���� ����Ʈ..?)
		
		

		gotoxy(N_ROW + 1, 0);
		printf("round %d, turn: player %d", jebi_round, jebi_player[player_point]);
		display();

		tick += 10;
		Sleep(10);
	}
}