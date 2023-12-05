// 2023-2 고급프로그래밍 과제: 쭈꾸미 게임
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "jjuggumi.h"

#define	DATA_FILE	"jjuggumi.dat"

//인트로
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
	printf("                                       쭈꾸미 게임에 오신 여러분 환영합니다\n");
	printf("=====================================================================================================================\n");
	Sleep(2100);
	system("cls");

	printf("미니게임 생성중");
	for (int i = 0; i < 5; i++)
	{
		printf(" . ");
		Sleep(100);
	}
	system("cls");
	printf("지금부터 게임을 시작합니다\n");
	Sleep(500);
	system("cls");
}

void intro(void)
{
	intro_1();
	intro_2();
	intro_3();
}

// low 이상 high 이하 난수를 발생시키는 함수
int randint(int low, int high) {
	int rnum = rand() % (high - low + 1) + low;
	return rnum;
}

int jjuggumi_init(void) {
	srand((unsigned int)time(NULL));

	FILE* fp;
	fopen_s(&fp, DATA_FILE, "r");
	if (fp == NULL) {
		return -1;  // -1 리턴하면 메인함수에서 처리하고 종료
	}

	// 플레이어 데이터 load
	fscanf_s(fp, "%d", &n_player);
	for (int i = 0; i < n_player; i++) {
		// 아직 안 배운 문법(구조체 포인터, 간접참조연산자)
		PLAYER* p = &player[i];

		// 파일에서 각 스탯 최댓값 읽기
		fscanf_s(fp, "%s%d%d",
			p->name, (unsigned int)sizeof(p->name),
			&(p->intel), &(p->str));
		p->stamina = 100; // 100%

		// 현재 상태
		p->is_alive = true;
		p->hasitem = false;
	}

	// 아이템 데이터 load
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
	/*printf("플레이어 수: ");
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