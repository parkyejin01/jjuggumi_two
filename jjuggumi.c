// 2023-2 ������α׷��� ����: �޲ٹ� ����
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

//��Ʈ��
void intro_1(void);
void intro_2(void);
void intro_3(void);
void intro(void);
//
int jjuggumi_init(void);


void intro_1(void)
{
	system("cls");
	printf("             .;!!;!;,=#@#=*#@$                  \n");
	printf("           #!    ,#=.         -@                \n");
	printf("          $     #,              #.              \n");
	printf("           #    @                 !,            \n");
	printf("           ~@@@@                   #            \n");
	printf("              ,-             #      =           \n");
	printf("              @                     =           \n");
	printf("    ====      ~         #    =,     ;           \n");
	printf("             .,            ,,=      ~           \n");
	printf(" ======      ,,                     *           \n");
	printf("          .:=**,                    #$          \n");
	printf("        ~@,     !@*                .-!          \n");
	printf("       ~,          =:              @ ,.         \n");
	printf("       @             @            #@$=          \n");
	printf("       @      .       =.             -#         \n");
	printf("        #@#=~,.-$@;    $               #        \n");
	printf("                   ~@,  ;              ~        \n");
	printf("   =========         ,,                 :       \n");
	printf("                     #                  ;       \n");
	printf("                     !       !    ;    -        \n");
	printf("=========            !      =     =    #        \n");
	printf("                     *.    ;@     #, .@         \n");
	printf("                      :@*;@, @*;$= ,$~          \n");
	Sleep(500);
	system("cls");
}

void intro_2(void)
{
	printf("         .~:~-.                                   \n");
	printf("       =$~....-*#,                                \n");
	printf("     ;=          !=                               \n");
	printf("    !:            .$              ;#=!*#~         \n");
	printf("   !~              ,!          .$$.     ;:!$######\n");
	printf("   =                $        ,#;         $        \n");
	printf("  =                 .;      #:           *        \n");
	printf("  #                  #    **      $      *,       \n");
	printf(" .;                  =  ,#.      *@      #,       \n");
	printf(" :              ~.   : :*       @.!~    ;;        \n");
	printf(" =       ==    / !   !;;      =@   :@$##~         \n");
	printf(" !      /  *         *:    ,$*:-                  \n");
	printf(" ~          . ~      #   .#:  #                   \n");
	printf(" .:          $/     .!  ;*   !.                   \n");
	printf("  #                 #  $-    @   @.               \n");
	printf("  #                 #  $-    @   @.               \n");
	printf("   @              ,# @      *   @,@,              \n");
	printf("   .#               #       #   -#~               \n");
	printf("    .#*            ,@       =           ,=:,      \n");
	printf("    :*.              =     .:   ,           =@@##-\n");
	printf("   ,!                #     --   ,,          ,=,.@,\n");
	printf("   #           #     $     ~,  *.*           !!#,,\n");
	printf("   #    =,     @#*. $.     :,  $.#            -,,,\n");
	printf("   @   ,*#     *  ,-       ~,                     \n");
	printf("   .@@#= ,@. -@            ,~                     \n");
	printf("           ~:,              *                     \n");
	Sleep(500);
	system("cls");
}

void intro_3(void)
{
	printf("=====================================================================================================================\n");
	printf("      ##       ##    ##  #    #####    #####    ##  #    ##  #      ##              #####     ###     ##  #    ##### \n");
	printf("     ##       ##    ##  ##   ##       ##       ##  ##   ### ##     ##              ##       ### ##   ### ##   ##     \n");
	printf("     ##       ##    ##  ##   ##       ##       ##  ##   ######     ##              ##       ##  ##   ######   ##     \n");
	printf(" #  ##    #  ##    ##  ##   ### ##   ### ##   ##  ##   ##  ##     ##              ### ##   ######   ##  ##   #####   \n");
	printf("##  ##   ##  ##    #   ##   ##  ##   ##  ##   #   ##   ##  ##     ##              ##  ##   ##  ##   ##  ##   ##      \n");
	printf("## ##    ## ##    ##  ##   ##  ##   ##  ##   ##  ##   ##  ##     ##              ##  ##   ##  ##   ##  ##   ##       \n");
	printf("#####    #####    ######   ######   ######   ######   ##  ##     ##              ######   ##  ##   ##  ##   ######   \n");
	printf("=====================================================================================================================\n");
	printf("                                       �޲ٹ� ���ӿ� ���� ������ ȯ���մϴ�\n");
	printf("=====================================================================================================================\n");
	Sleep(2100);
	system("cls");

	printf("�̴ϰ��� ������");
	for (int i = 0; i < 5; i++)
	{
		printf(" . ");
		Sleep(100);
	}
	system("cls");
	printf("���ݺ��� ������ �����մϴ�\n");
	Sleep(500);
	system("cls");
}

void intro(void)
{
	intro_1();
	intro_2();
	intro_3();
}

// low �̻� high ���� ������ �߻���Ű�� �Լ�
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;  // -1 �����ϸ� �����Լ����� ó���ϰ� ����
	}

	// �÷��̾� ������ load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// ���� �� ��� ����(����ü ������, ��������������)
		PLAYER* p = &player[i];

		// ���Ͽ��� �� ���� �ִ� �б�
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100; // 100%

		// ���� ����
		p->is_alive = true;
		p->hasitem = false;
	}

	// ������ ������ load
	fscanf_s(fp, "%d", &n_item);
	for (int i = 0; i < n_item; i++) {
		fscanf_s(fp, "%s%d%d%d",
			item[i].name, (unsigned int)sizeof(item[i].name),
			&(item[i].intel_buf),
			&(item[i].str_buf),
			&(item[i].stamina_buf));
	}

	fclose(fp);
	n_alive = n_player;
	/*printf("�÷��̾� ��: ");
	scanf_s("%d", &n_player);

	n_alive = n_player;
	for (int i = 0; i < n_player; i++) {
		player[i].is_alive = true;
	}*/
	return 0;
}

int main(void) {
	jjuggumi_init();
	intro();
	//sample();
	mugunghwa();
	nightgame();
	juldarigi();
	jebi();
	ending();
	return 0;
}