#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include "jjuggumi.h"
#include "canvas.h"

#define DIALOG_DURATION_SEC		4

char rmb_buf[ROW_MAX][COL_MAX];

void end_win(void);
void end_no_win(void);
void print_status_jul(void);

void end_win(void)
{
	printf("===========================================================================\n");
	printf("       ~$:          #-  .=         *,!;           \n");
	printf("      .* ;~        -~!- =.*       ~~ .*           \n");
	printf("      .!  $.    .  ~. =$  :       #  =,           \n");
	printf("       !  ,~    *!!*      ~.      ..*-            \n");
	printf("       -!  *     ! .      -:      *               \n");
	printf("         ;~.     $.-:;!!!!!!!!:,                  \n");
	printf("              *.                 *                \n");
	printf("            .*-                    :~    .,~;;    \n");
	printf("   .!-      :-                      $  $#=!-      ##    ##    ##    ###  ##\n");
	printf("  .$-:!~   ,*               ,       *.            ## ## ##    ##     ### ##\n");
	printf("  .$   ;;  ;-        .      #!      --            ##  ## #    ##    # #####\n");
	printf("   ,=~  :  *         #!             ,~             ## ##      ##    ## ####\n");
	printf("    .;***~ !          . ~##=\'       .:             ## ##      ##    ##  ###\n");
	printf("           *            *##/        .:             ##  ##     ##    ##   ##\n");
	printf("            *                       ,:~:;-        \n");
	printf("         .::*:.                    :;     :~      \n");
	printf("        -                                 ;       \n");
	printf("       !       ;                    =.  ~:-       \n");
	printf("       ;,      *                    $ ,,,,;       \n");
	printf("         ~$==~ ,:                    ~#           \n");
	printf("          *,. .$.     -      -,       !.          \n");
	printf("              $.      #,     ;-       ~,          \n");
	printf("             .!      -*~    .#!:     ,*           \n");
	printf("              *.;!*,,* *-.,.!- ~$*::;=            \n");
}

void end_no_win(void)
{
	printf("###  ##   ## ##            ## ## ##    ##    ###  ##  ###  ##  ## ###   ## ####\n");
	printf(" ### ##  ### ###           ## ## ##    ##     ### ##   ### ##  ##       ##   ##\n");
	printf("# #####  ##   ##           ##  ## #    ##    # #####  # #####  ## ###   ## ####\n");
	printf("## ####  ##   ##            ## ##      ##    ## ####  ## ####  ## ###   ## ### \n");
	printf("##  ###  ### ###            ## ##      ##    ##  ###  ##  ###  ##       ##    #\n");
	printf("##   ##   ## ##             ##  ##     ##    ##   ##  ##   ##  ## ###   ##   ## \n");
}

void ending(void)
{
	system("cls");
	if (n_alive >= 2)
	{
		printf("����ڸ� ������ ���߽��ϴ�. \n");
		printf("===============================================================================\n");
		printf("player ");
		int k = 1;
		for (int p = 0; p < n_player; p++)
		{
			if (player[p].is_alive == true)
			{
				printf("%d", p);
				if (k < n_alive)
				{
					printf(", ");
				}
				k++;
			}
		}
		printf(" alive\n\n");
		end_no_win();
	}
	else
	{
		printf("����ڰ� ���Խ��ϴ�!\n");
		printf("===========================================================================\n");
		printf("����ڴ� player ");
		for (int p = 0; p < n_player; p++)
		{
			if (player[p].is_alive == true)
			{
				printf("%d", p);
			}
		}
		printf(" �Դϴ�! Congratulations~~!\n");
		end_win();
	}

	exit(0);
}

// (zero-base) row��, col���� Ŀ�� �̵�
void gotoxy(int row, int col) {
	COORD pos = { col,row };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

// row��, col���� ch ���
void printxy(char ch, int row, int col) {
	gotoxy(row, col);
	printf("%c", ch);
}

void map_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '*';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = (i == 0 || i == N_ROW - 1) ? '*' : ' ';
		}
	}
}

// �ٴٸ��� �� �Լ�
void julmap_init(int n_row, int n_col) {
	// �� ���۸��� ������ ����
	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = front_buf[i][j] = ' ';
		}
	}

	N_ROW = n_row;
	N_COL = n_col;
	for (int i = 0; i < N_ROW; i++) {
		// ���Թ� �̷��� �� �� �ִµ� �Ϻη� �� ����������
		back_buf[i][0] = back_buf[i][N_COL - 1] = '#';

		for (int j = 1; j < N_COL - 1; j++) {
			back_buf[i][j] = ((i == 0 || i == N_ROW - 1) && (j != (N_COL - 1) / 2)) ? '#' : ' ';
		}
	}
}

// back_buf[row][col]�� �̵��� �� �ִ� �ڸ����� Ȯ���ϴ� �Լ�
bool placable(int row, int col) {
	if (row < 0 || row >= N_ROW ||
		col < 0 || col >= N_COL ||
		back_buf[row][col] != ' ') {
		return false;
	}
	return true;
}

// ��ܿ� ����, �ϴܿ��� ���� ���¸� ���
void display(void) {
	draw();
	gotoxy(N_ROW + 4, 0);  // �߰��� ǥ���� ������ ������ �ʰ� ����â ������ �� ������ ���
	print_status();
}

void display_jul(void) {
	draw();
	gotoxy(N_ROW + 4, 0);
	print_status_jul();
}

void draw(void) {
	for (int row = 0; row < N_ROW; row++) {
		for (int col = 0; col < N_COL; col++) {
			if (front_buf[row][col] != back_buf[row][col]) {
				front_buf[row][col] = back_buf[row][col];
				printxy(front_buf[row][col], row, col);
			}
		}
	}
}

void print_status(void) {
	printf("no. of players left: %d\n", n_alive);
	printf("\t\t\tintl\tstr\tstm\n");
	//���⿡ ������ ����ü �迭 ���� �߰� � �� ������ �÷��̾� �� ������ ȹ�� �迭 �����Ѱ� ����ص� �ɵ�
	for (int p = 0; p < n_player; p++) {
		printf("player %2d: %5s", p, player[p].is_alive ? "alive" : "DEAD");
		printf("\t%d(%s%d)\t%d(%s%d)\t%3d%%", player[p].intel, player[p].intel >= 0 ? "+" : "", player[p].item.intel_buf,
			player[p].str, player[p].str >= 0 ? "+" : "", player[p].item.str_buf, player[p].stamina);
		printf("\n");
	}
}

void print_status_jul(void) {
	printf("no. of players left: %d\n", n_alive);
	printf("\t\t\tintl\tstr\tstm\n");
	//���⿡ ������ ����ü �迭 ���� �߰� � �� ������ �÷��̾� �� ������ ȹ�� �迭 �����Ѱ� ����ص� �ɵ�
	for (int p = 0; p < n_player; p++) {
		if (died_player[p] == 1)//�׾��� �÷��̾�� *�ٿ��� ǥ��
		{
			printf("player %2d: %5s*", p, player[p].is_alive ? "alive" : "DEAD");
		}
		else
		{
			printf("player %2d: %5s", p, player[p].is_alive ? "alive" : "DEAD");
		}
		printf("\t%d(%s%d)\t%d(%s%d)\t%3d%%", player[p].intel, player[p].intel >= 0 ? "+" : "", player[p].item.intel_buf,
			player[p].str, player[p].str >= 0 ? "+" : "", player[p].item.str_buf, player[p].stamina);
		printf("\n");
	}
}

void dialog(char message[]) {

}


void dialog_mgh(char message[])
{
	int k = 0;
	if (bye >= 16)
	{
		k += 24;
	}
	else if (bye >= 12)
	{
		k += 16;
	}
	else if (bye >= 8)
	{
		k += 8;
	}


	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			rmb_buf[i][j] = back_buf[i][j];
		}
	}

	for (int i = 3; i < 8; i++) {
		for (int j = 5; j < 30 + k; j++) {
			back_buf[i][j] = ' ';
		}
	}

	for (int i = 3; i < 8; i++) {
		back_buf[i][5] = back_buf[i][29 + k] = '*';

		for (int j = 6; j < 29 + k; j++) {
			back_buf[i][j] = (i == 3 || i == 7) ? '*' : ' ';
		}
	}

	for (int i = 3; i < 8; i++) {
		for (int j = 5; j < 30 + k; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}


	for (int i = DIALOG_DURATION_SEC; i > 0; i--)
	{
		gotoxy(5, 7);
		printf("%d player ", i);

		for (int j = 0; j <= bye - 2; j++)
		{
			printf("%c", message[j]);
			if (j % 2 == 1)
			{
				printf(" ");
			}
		}
		printf(" dead!");


		Sleep(1000);
	}

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = rmb_buf[i][j];
		}
	}

	for (int i = 3; i < 8; i++) {
		for (int j = 5; j < 30 + k; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}

}


void dialog_jul(char message[])
{
	int k = 0;
	int dp = 0;
	if (bye == 2)
	{
		k = 6;
		dp += 2;
	}

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			rmb_buf[i][j] = back_buf[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 27 + k; j++) {
			back_buf[i][j] = ' ';
		}
	}

	for (int i = 0; i < 3; i++) {
		back_buf[i][5] = back_buf[i][26+k] = '*';

		for (int j = 6; j < 26 + k; j++) {
			back_buf[i][j] = (i == 0 || i == 2) ? '*' : ' ';
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 27 + k; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}


	for (int i = DIALOG_DURATION_SEC; i > 0; i--)
	{
		gotoxy(1, 8);
		printf("%d player ", i);
		for (int j = 0; j <= dp; j++)
		{
			printf("%c", message[j]);
			if (j % 2 == 1)
			{
				printf(" ");
			}
		}
		printf(" dead!");

		Sleep(1000);
	}

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = rmb_buf[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 27 + k; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}
}

void dialog_juldarigi_lie(char message[])
{

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			rmb_buf[i][j] = back_buf[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 32; j++) {
			back_buf[i][j] = ' ';
		}
	}

	for (int i = 0; i < 3; i++) {
		back_buf[i][5] = back_buf[i][31] = '*';

		for (int j = 6; j < 31; j++) {
			back_buf[i][j] = (i == 0 || i == 2) ? '*' : ' ';
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 32; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}


	gotoxy(1, 8);
	
	printf("\'%s\' team is lying!", message);

	Sleep(1000);

	for (int i = 0; i < ROW_MAX; i++) {
		for (int j = 0; j < COL_MAX; j++) {
			back_buf[i][j] = rmb_buf[i][j];
		}
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 5; j < 32; j++) {
			gotoxy(i, j);
			printf("%c", back_buf[i][j]);
		}
	}
}
