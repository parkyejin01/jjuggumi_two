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
		//����Ű�� ���� �����ϰ� �����̽��ٸ� ������ �� �÷��̾� ��� ���� �����ϰ� ���� �÷��̾�� �� �ѱ��
		//������ ������� �����ϸ� fail���� ������ Ż���ϰ� �ٷ� ���� ���� �����ϰ� �Ѵ�. 
		//fail����� ���帶�� �Ѱ�����! �������� pass����


		display();

		tick += 10;
		Sleep(10);
	}
}