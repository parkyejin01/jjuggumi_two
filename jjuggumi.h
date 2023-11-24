#ifndef _JJUGGUMI_H_
#define _JJUGGUMI_H_

#include <Windows.h>
#include <stdbool.h>

#define PLAYER_MAX		10
#define ITEM_MAX		10


typedef struct {
	char name[100];
	int intel_buf, str_buf, stamina_buf;
} ITEM;

ITEM item[ITEM_MAX];

typedef struct {
	int id;
	char name[100];

	int intel, str, stamina;

	bool is_alive;
	bool hasitem;
	ITEM item;
} PLAYER;


PLAYER player[PLAYER_MAX];  // �⺻�� true, Ż���ϸ� false
int n_player, n_alive;
int n_item;
int tick;  // �ð�
int died_player[PLAYER_MAX];//�ٴٸ��� �� ���� �÷��̾� flag���� �迭

// �̴ϰ���
//void sample(void);
void mugunghwa(void);
//void mugunghwa(void);
void nightgame(void);
void juldarigi(void);
void jebi(void);

int randint(int low, int high);

#endif