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
int move_0flag = 0;


void nightgame_init(void);
void p0(key_t);
void player_move_night(int, int, int);
void the_rest_player(int);
double get_distance(int, int, int, int);
void shortmove(int, int, int);
void pick_item(int);
void player_meet(int);
void stamin_night(void);


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

            period[i] = randint(400, 500);
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
    
    move_0flag = 1;

    player_move_night(0, nx, ny);
}


//�÷��̾� 0������ �÷��̾� �ڵ�
void the_rest_player(int p)
{

    double shortest1 = INT_MAX;
    int index1 = -1;
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
    int index2 = -2;
    //����ִ� �÷��̾� ���̿��� ������ ������ �÷��̾�� �� ���� ª�� �Ÿ�
    for (int i = 0; i < n_player; i++)
    {
        if (player[i].hasitem == true && player[i].is_alive == true && p != i)
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
    if (index1 != -1) {//�����ִ� �������� �ִ� ���
        if (shortest1 < shortest2)//�������� �� ����� ��
        {
            shortmove(p, px[p] - itemx[index1], py[p] - itemy[index1]);
        }
        else if (shortest1 > shortest2)//������ ���� �÷��̾ �� ����� ��
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
        else
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
       
    }
    else {//�����ִ� �������� ���� ���
        if (index2 == -2)//�����ִ� �����۵� ����, �������� ������ �÷��̾ ���� ���
        {
            nx = px[0];
            ny = py[0];
        }
        else//�������� ������ �÷��̾ ���� ���
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
    }
    if (!placable(nx, ny))//��ġ�� �� ���� ���̸� �������� �ʵ��� ����
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

//�������� ������ ������ ���� �� ��ȣ�ۿ� �ڵ�
void pick_item(int p)
{
    //����ִ� �÷��̾� �� �������� ������ ������ ���� ��
    if ((player[p].is_alive == true))
    {
        if (back_buf[px[p] - 1][py[p]] == 'I' || back_buf[px[p] + 1][py[p]] == 'I' || back_buf[px[p]][py[p] - 1] == 'I' || back_buf[px[p]][py[p] + 1] == 'I')
        {
            if (player[p].hasitem == true)//�÷��̾ �������� ������ ���� ���
            {
                int pick;
                if (p == 0)
                {
                    gotoxy(4, 32);
                    printf("�������� ��ȯ�Ͻðڽ��ϱ�?(��: 1/�ƴϿ�: 0) : ");
                    scanf_s("%d", &pick);

                    gotoxy(4, 32);
                    printf("                                                ");

                }
                else
                {
                    pick = randint(0, 1);
                }

                if (pick == 1)//��ȯ
                {
                    for (int i = 0; i < count_item; i++)
                    {
                        if (back_buf[itemx[i]][itemy[i]] == 'I')
                        {
                            if (((itemx[i] == px[p] - 1) && (itemy[i] == py[p])) || ((itemx[i] == px[p] + 1) && (itemy[i] == py[p])) || ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1)) || ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1)))//���� ������
                            {
                                ITEM tmp_item = player[p].item;
                                player[p].item = night_items[i];
                                player[p].stamina += player[p].item.stamina_buf;
                                night_items[i] = tmp_item;

                                break;
                            }
                        }
                    }
                }

                gotoxy(N_ROW + 2, 0);
                if (pick == 1)
                {
                    printf("player %d��(��) %s(��)�� item�� ��ȯ�߽��ϴ�!! ='��'= ", p, player[p].item.name);
                }
                else
                {
                    printf("player %d��(��) item�� �����ƽ��ϴ�...", p);
                }
                Sleep(1200);
                gotoxy(N_ROW + 2, 0);
                printf("                                                                               ");

                if (player[p].stamina > 100)
                {
                    player[p].stamina = 100;
                }

            }
            else//�÷��̾ �������� ������ ���� ���� ���
            {
                player[p].hasitem = true;//������ ������ �ִٰ� ����
                for (int i = 0; i < count_item; i++)
                {
                    if (back_buf[itemx[i]][itemy[i]] == 'I')
                    {
                        if ((itemx[i] == px[p] - 1) && (itemy[i] == py[p]))//���� ������
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p] + 1) && (itemy[i] == py[p]))//�Ʒ��� ������
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1))//���ʿ� ������
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1))//�����ʿ� ������
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                    }
                }

                if (player[p].stamina > 100)
                {
                    player[p].stamina = 100;
                }


                gotoxy(N_ROW + 2, 0);
                printf("player %d�� item %s��(��) �ֿ����ϴ�!", p, player[p].item.name);
                Sleep(1200);
                gotoxy(N_ROW + 2, 0);
                printf("                                                                  ");



            }
        }
    }
}


//�÷��̾�� ������ �� ��ȣ�ۿ� �ڵ�
void player_meet(int p)
{
    for (int i = 0; i < n_player; i++)
    {
        if (player[i].is_alive == true && p != i)
        {
            if (((px[p] - 1 == px[i]) && (py[p] == py[i])) || ((px[p] + 1 == px[i]) && (py[p] == py[i])) || ((px[p] == px[i]) && (py[p] - 1 == py[i])) || ((px[p] == px[i]) && (py[p] + 1 == py[i])))
            {
                int pickpick;
                int player_do;
                int player_wait;
                if (p == 0)
                {
                    gotoxy(4, 32);
                    printf("�÷��̾� %d�� �������ϴ�! (��Ż �õ�: 1/ȸ�� �õ�: 2/����: 3) : ", i);
                    scanf_s("%d", &pickpick);

                    gotoxy(4, 32);
                    printf("                                                                             ");
                    player_do = 0;
                    player_wait = i;
                }
                else
                {
                    if (player[p].hasitem == true && player[i].hasitem == false)
                    {
                        player_do = i;
                        player_wait = p;
                        if (player_wait == 0)
                        {
                            player_do = p;
                            player_wait = i;
                        }
                    }
                    else if (player[p].hasitem == false && player[i].hasitem == true)
                    {
                        player_do = p;
                        player_wait = i;
                        if (player_wait == 0)
                        {
                            player_do = i;
                            player_wait = p;
                        }
                    }
                    else
                    {
                        int rnd_do = randint(0, 1);

                        if (rnd_do == 0)
                        {
                            player_do = p;
                            player_wait = i;
                        }
                        else
                        {
                            player_do = i;
                            player_wait = p;
                        }

                        if (player_wait == 0)
                        {
                            int tmp = player_do;
                            player_do = player_wait;
                            player_wait = tmp;
                        }
                    }

                    if (player_do == 0)
                    {
                        gotoxy(4, 32);
                        printf("�÷��̾� %d�� �������ϴ�! (��Ż �õ�: 1/ȸ�� �õ�: 2/����: 3) : ", player_wait);
                        scanf_s("%d", &pickpick);

                        gotoxy(4, 32);
                        printf("                                                                             ");
                    }
                    else
                    {
                        pickpick = randint(1, 3);
                    }
                }

                if (player[player_do].stamina > 0)//�ൿ�ϴ� �÷��̾��� ���׹̳��� 0 ���� Ŭ ��
                {
                    if (pickpick == 1)//��Ż
                    {
                        double do_str = (player[player_do].str + player[player_do].item.str_buf) * (player[player_do].stamina) / 100;
                        double wait_str = (player[player_wait].str + player[player_wait].item.str_buf) * (player[player_wait].stamina) / 100;

                        if (do_str > wait_str)//��ȿ���� �� �꼭 ��Ż ����
                        {
                            if (player[player_wait].hasitem == true && player[player_do].hasitem == true)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� item %s��(��) ��Ż�߽��ϴ�!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == true && player[player_do].hasitem == false)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_do].hasitem = true;
                                player[player_wait].hasitem = false;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� item %s��(��) ��Ż�߽��ϴ�!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == false && player[player_do].hasitem == true)
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� item ��Ż�� ����������, %d��(��) item�� �����ϴ�!", player_do, player_wait, player_wait);
                            }
                            else
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� item ��Ż�� ����������, %d��(��) item�� �����ϴ�!", player_do, player_wait, player_wait);
                            }
                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                                                ");
                            player[player_do].stamina -= 40;

                        }
                        else//��Ż ����
                        {
                            gotoxy(N_ROW + 2, 0);
                            printf("player %d�� %d�κ��� ������ ��Ż�� �����߽��ϴ�...", player_do, player_wait);
                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                    ");

                            player[player_do].stamina -= 60;
                        }
                    }
                    else if (pickpick == 2)//ȸ��
                    {
                        double do_intel = (player[player_do].intel + player[player_do].item.intel_buf) * (player[player_do].stamina) / 100;
                        double wait_intel = (player[player_wait].intel + player[player_wait].item.intel_buf) * (player[player_wait].stamina) / 100;


                        if (do_intel > wait_intel)//ȸ�� ����
                        {
                            if (player[player_wait].hasitem == true && player[player_do].hasitem == true)
                            {
                                ITEM tmp_item3 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_wait].item = tmp_item3;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d��(��) ȸ���Ͽ� item %s��(��) ������ϴ�!", player_do, player_wait, player[player_do].item.name);

                            }
                            else if (player[player_wait].hasitem == true && player[player_do].hasitem == false)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_do].hasitem = true;
                                player[player_wait].hasitem = false;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d��(��) ȸ���Ͽ� item %s��(��) ������ϴ�!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == false && player[player_do].hasitem == true)
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� ȸ���� ����������, %d��(��) item�� �����ϴ�!", player_do, player_wait, player_wait);
                            }
                            else
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d��(��) %d(��)�κ��� ȸ���� ����������, %d��(��) item�� �����ϴ�!", player_do, player_wait, player_wait);
                            }

                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                                                                      ");

                            player[player_do].stamina -= 40;
                        }
                        else//ȸ�� ����
                        {
                            gotoxy(N_ROW + 2, 0);
                            printf("player %d�� %d��(��) ȸ���ϴ� �Ϳ� �����߽��ϴ�...", player_do, player_wait);

                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                    ");

                            player[player_do].stamina -= 60;
                        }
                    }
                    else//����
                    {
                        gotoxy(N_ROW + 2, 0);
                        printf("�÷��̾� %d�� %d�� �����߽��ϴ�...-,-",player_do, player_wait);
                        Sleep(1200);
                        gotoxy(N_ROW + 2, 0);
                        printf("                                                                                    ");
                        
                    }
                }
                else//����
                {
                    pickpick = 3;

                    gotoxy(N_ROW + 2, 0);
                    printf("���׹̳��� 0�̶� �÷��̾� %d�� %d�� �����մϴ�.", player_do, player_wait);
                    Sleep(700);
                    gotoxy(N_ROW + 2, 0);
                    printf("                                                                                    ");
                }


                if (player[player_do].stamina < 0)
                {
                    player[player_do].stamina = 0;
                }


                return;
            }
        }
    }
}

void stamin_night(void)//���� ��ĥ ������ ������ ȸ��
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


void nightgame(void)
{
    nightgame_init();
    system("cls");
    display();

    while (1)
    {

        //Ű �Է� �ޱ�
        key_t key = get_key();
        if (key == K_QUIT)//Q�Է� �� ����
        {
            stamin_night();
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
                //���� �ֱ⸶�� �÷��̾� �ൿ
                if (tick % period[p] == 0)
                {
                    the_rest_player(p);
                }
            }
        }


        display();

        tick += 10;
        Sleep(10);


        //���⿡ ������ ĭ�� �������� ���� �� ��ȣ�ۿ� �ڵ� �����
        for (int p = 0; p < n_player; p++)
        {
            if (p == 0)
            {
                pick_item(p);

                display();
            }
            else
            {
                if (tick % period[p] == 10)
                {
                    pick_item(p);

                    display();
                }
            }
        }

        //���⿡ ������ ĭ�� �÷��̾� ���� �� ��ȣ�ۿ� �ڵ� �����
        for (int p = 0; p < n_player; p++)
        {
            if (player[p].is_alive == true)
            {
                if (p == 0 && move_0flag == 1)
                {
                    player_meet(p);
                    move_0flag = 0;
                }
                else
                {
                    if (tick % period[p] == 10)
                    {
                        player_meet(p);
                    }
                }
            }
        }
    }

}