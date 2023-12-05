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


//야간운동 맵 생성
void nightgame_init(void)
{
    //맵 틀 만들기
    map_init(10, 30);

    //플레이어 위치 랜덤
    for (int i = 0; i < n_player; i++)
    {
        if (player[i].is_alive == true)
        {
            px[i] = randint(1, 8);
            py[i] = randint(1, 28);

            //중복되지 않은 위치 설정
            while (back_buf[px[i]][py[i]] != ' ')
            {
                px[i] = randint(1, 8);
                py[i] = randint(1, 28);
            }

            back_buf[px[i]][py[i]] = '0' + i;//플레이어 시작 위치로 이동

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
    //야간운동 아이템 배열 옮기기
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

        //중복되지 않은 위치 설정
        while (back_buf[itemx[i]][itemy[i]] != ' ')
        {
            itemx[i] = randint(1, 8);
            itemy[i] = randint(1, 28);
        }

        back_buf[itemx[i]][itemy[i]] = 'I';
    }


    tick = 0;
}


//방향키 입력 받았을 시 플레이어 0 코드
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


//플레이어 0제외한 플레이어 코드
void the_rest_player(int p)
{

    double shortest1 = INT_MAX;
    int index1 = -1;
    //아이템들 중 가장 짧은 거리
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
    //살아있는 플레이어 사이에서 아이템 소유한 플레이어들 중 가장 짧은 거리
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

    //==>비교해서 가장 짧은 거리에 있는 쪽으로 이동
    if (index1 != -1) {//남아있는 아이템이 있는 경우
        if (shortest1 < shortest2)//아이템이 더 가까울 때
        {
            shortmove(p, px[p] - itemx[index1], py[p] - itemy[index1]);
        }
        else if (shortest1 > shortest2)//아이템 소유 플레이어가 더 가까울 때
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
        else
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
       
    }
    else {//남아있는 아이템이 없는 경우
        if (index2 == -2)//남아있는 아이템도 없고, 아이템을 소유한 플레이어도 없을 경우
        {
            nx = px[0];
            ny = py[0];
        }
        else//아이템을 소유한 플레이어가 있을 경우
        {
            shortmove(p, px[p] - px[index2], py[p] - py[index2]);
        }
    }
    if (!placable(nx, ny))//위치할 수 없는 곳이면 움직이지 않도록 설정
    {
        return;
    }
    player_move_night(p, nx, ny);

}


//플레이어 이동 후 맵에 백업
void player_move_night(int p, int nx, int ny)
{
    back_buf[nx][ny] = back_buf[px[p]][py[p]];
    back_buf[px[p]][py[p]] = ' ';
    px[p] = nx;
    py[p] = ny;
}


//아이템 또는 아이템을 소유한 플레이어와 현재 반복문의 플레이어 사이 거리
//((아이템 또는 아이템 소유 플레이어)--거리--(반복문에서 현재 차례 플레이어))
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

    if (xx > 0)//아이템 또는 아이템 소유 플레이어가 위쪽
    {
        if (yy > 0)//아이템 또는 아이템 소유 플레이어가 왼쪽
        {
            if (sqrt(xx * xx) > sqrt(yy * yy))//위쪽이 더 차이가 클 때
            {
                nx = px[p] - 1;
                ny = py[p] + 0;
            }
            else if (sqrt(xx * xx) < sqrt(yy * yy))//왼쪽이 더 차이가 클 때
            {
                nx = px[p] + 0;
                ny = py[p] - 1;
            }
            else//위쪽 왼쪽 차이가 같을 때
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
        else//아이템 또는 아이템 소유 플레이어가 오른쪽
        {
            if (sqrt(xx * xx) > sqrt(yy * yy))//위쪽이 더 차이가 클 때
            {
                nx = px[p] - 1;
                ny = py[p] + 0;
            }
            else if (sqrt(xx * xx) < sqrt(yy * yy))//오른쪽이 더 차이가 클 때
            {
                nx = px[p] + 0;
                ny = py[p] + 1;
            }
            else//위쪽 왼쪽 차이가 같을 때
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
    else//아이템 또는 아이템 소유 플레이어가 아래쪽
    {
        if (yy > 0)//아이템 또는 아이템 소유 플레이어가 왼쪽
        {
            if (sqrt(xx * xx) > sqrt(yy * yy))//아래쪽이 더 차이가 클 때
            {
                nx = px[p] + 1;
                ny = py[p] + 0;
            }
            else if (sqrt(xx * xx) < sqrt(yy * yy))//왼쪽이 더 차이가 클 때
            {
                nx = px[p] + 0;
                ny = py[p] - 1;
            }
            else//아래쪽 왼쪽 차이가 같을 때
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
        else//아이템 또는 아이템 소유 플레이어가 오른쪽
        {
            if (sqrt(xx * xx) > sqrt(yy * yy))//아래쪽이 더 차이가 클 때
            {
                nx = px[p] + 1;
                ny = py[p] + 0;
            }
            else if (sqrt(xx * xx) < sqrt(yy * yy))//오른쪽이 더 차이가 클 때
            {
                nx = px[p] + 0;
                ny = py[p] + 1;
            }
            else//아래쪽 왼쪽 차이가 같을 때
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

//아이템이 인접한 공간에 있을 때 상호작용 코드
void pick_item(int p)
{
    //살아있는 플레이어 중 아이템이 인접한 공간에 있을 때
    if ((player[p].is_alive == true))
    {
        if (back_buf[px[p] - 1][py[p]] == 'I' || back_buf[px[p] + 1][py[p]] == 'I' || back_buf[px[p]][py[p] - 1] == 'I' || back_buf[px[p]][py[p] + 1] == 'I')
        {
            if (player[p].hasitem == true)//플레이어가 아이템을 가지고 있을 경우
            {
                int pick;
                if (p == 0)
                {
                    gotoxy(4, 32);
                    printf("아이템을 교환하시겠습니까?(네: 1/아니오: 0) : ");
                    scanf_s("%d", &pick);

                    gotoxy(4, 32);
                    printf("                                                ");

                }
                else
                {
                    pick = randint(0, 1);
                }

                if (pick == 1)//교환
                {
                    for (int i = 0; i < count_item; i++)
                    {
                        if (back_buf[itemx[i]][itemy[i]] == 'I')
                        {
                            if (((itemx[i] == px[p] - 1) && (itemy[i] == py[p])) || ((itemx[i] == px[p] + 1) && (itemy[i] == py[p])) || ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1)) || ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1)))//위에 아이템
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
                    printf("player %d이(가) %s(으)로 item을 교환했습니다!! ='ㅅ'= ", p, player[p].item.name);
                }
                else
                {
                    printf("player %d이(가) item을 지나쳤습니다...", p);
                }
                Sleep(1200);
                gotoxy(N_ROW + 2, 0);
                printf("                                                                               ");

                if (player[p].stamina > 100)
                {
                    player[p].stamina = 100;
                }

            }
            else//플레이어가 아이템을 가지고 있지 않은 경우
            {
                player[p].hasitem = true;//아이템 가지고 있다고 설정
                for (int i = 0; i < count_item; i++)
                {
                    if (back_buf[itemx[i]][itemy[i]] == 'I')
                    {
                        if ((itemx[i] == px[p] - 1) && (itemy[i] == py[p]))//위에 아이템
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p] + 1) && (itemy[i] == py[p]))//아래에 아이템
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p]) && (itemy[i] == py[p] - 1))//왼쪽에 아이템
                        {
                            player[p].item = night_items[i];
                            back_buf[itemx[i]][itemy[i]] = ' ';
                            player[p].stamina += player[p].item.stamina_buf;
                            break;
                        }
                        else if ((itemx[i] == px[p]) && (itemy[i] == py[p] + 1))//오른쪽에 아이템
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
                printf("player %d가 item %s을(를) 주웠습니다!", p, player[p].item.name);
                Sleep(1200);
                gotoxy(N_ROW + 2, 0);
                printf("                                                                  ");



            }
        }
    }
}


//플레이어끼리 만났을 때 상호작용 코드
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
                    printf("플레이어 %d를 만났습니다! (강탈 시도: 1/회유 시도: 2/무시: 3) : ", i);
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
                        printf("플레이어 %d를 만났습니다! (강탈 시도: 1/회유 시도: 2/무시: 3) : ", player_wait);
                        scanf_s("%d", &pickpick);

                        gotoxy(4, 32);
                        printf("                                                                             ");
                    }
                    else
                    {
                        pickpick = randint(1, 3);
                    }
                }

                if (player[player_do].stamina > 0)//행동하는 플레이어의 스테미나가 0 보다 클 때
                {
                    if (pickpick == 1)//강탈
                    {
                        double do_str = (player[player_do].str + player[player_do].item.str_buf) * (player[player_do].stamina) / 100;
                        double wait_str = (player[player_wait].str + player[player_wait].item.str_buf) * (player[player_wait].stamina) / 100;

                        if (do_str > wait_str)//유효힘이 더 쎄서 강탈 성공
                        {
                            if (player[player_wait].hasitem == true && player[player_do].hasitem == true)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 item %s을(를) 강탈했습니다!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == true && player[player_do].hasitem == false)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_do].hasitem = true;
                                player[player_wait].hasitem = false;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 item %s을(를) 강탈했습니다!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == false && player[player_do].hasitem == true)
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 item 강탈에 성공했지만, %d이(가) item이 없습니다!", player_do, player_wait, player_wait);
                            }
                            else
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 item 강탈에 성공했지만, %d이(가) item이 없습니다!", player_do, player_wait, player_wait);
                            }
                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                                                ");
                            player[player_do].stamina -= 40;

                        }
                        else//강탈 실패
                        {
                            gotoxy(N_ROW + 2, 0);
                            printf("player %d이 %d로부터 아이템 강탈에 실패했습니다...", player_do, player_wait);
                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                    ");

                            player[player_do].stamina -= 60;
                        }
                    }
                    else if (pickpick == 2)//회유
                    {
                        double do_intel = (player[player_do].intel + player[player_do].item.intel_buf) * (player[player_do].stamina) / 100;
                        double wait_intel = (player[player_wait].intel + player[player_wait].item.intel_buf) * (player[player_wait].stamina) / 100;


                        if (do_intel > wait_intel)//회유 성공
                        {
                            if (player[player_wait].hasitem == true && player[player_do].hasitem == true)
                            {
                                ITEM tmp_item3 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_wait].item = tmp_item3;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d을(를) 회유하여 item %s을(를) 얻었습니다!", player_do, player_wait, player[player_do].item.name);

                            }
                            else if (player[player_wait].hasitem == true && player[player_do].hasitem == false)
                            {
                                ITEM tmp_item2 = player[player_do].item;
                                player[player_do].item = player[player_wait].item;
                                player[player_do].hasitem = true;
                                player[player_wait].hasitem = false;
                                player[player_wait].item = tmp_item2;
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d을(를) 회유하여 item %s을(를) 얻었습니다!", player_do, player_wait, player[player_do].item.name);
                            }
                            else if (player[player_wait].hasitem == false && player[player_do].hasitem == true)
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 회유에 성공했지만, %d이(가) item이 없습니다!", player_do, player_wait, player_wait);
                            }
                            else
                            {
                                gotoxy(N_ROW + 2, 0);
                                printf("player %d이(가) %d(으)로부터 회유에 성공했지만, %d이(가) item이 없습니다!", player_do, player_wait, player_wait);
                            }

                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                                                                      ");

                            player[player_do].stamina -= 40;
                        }
                        else//회유 실패
                        {
                            gotoxy(N_ROW + 2, 0);
                            printf("player %d이 %d을(를) 회유하는 것에 실패했습니다...", player_do, player_wait);

                            Sleep(1200);
                            gotoxy(N_ROW + 2, 0);
                            printf("                                                                                    ");

                            player[player_do].stamina -= 60;
                        }
                    }
                    else//무시
                    {
                        gotoxy(N_ROW + 2, 0);
                        printf("플레이어 %d가 %d를 무시했습니다...-,-",player_do, player_wait);
                        Sleep(1200);
                        gotoxy(N_ROW + 2, 0);
                        printf("                                                                                    ");
                        
                    }
                }
                else//무시
                {
                    pickpick = 3;

                    gotoxy(N_ROW + 2, 0);
                    printf("스테미나가 0이라 플레이어 %d가 %d를 무시합니다.", player_do, player_wait);
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

void stamin_night(void)//게임 마칠 때마다 일정량 회복
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

        //키 입력 받기
        key_t key = get_key();
        if (key == K_QUIT)//Q입력 시 종료
        {
            stamin_night();
            return;
        }
        else if (key != K_UNDEFINED)//방향키 입력 받았을 시 플레이어 0 코드
        {
            if (player[0].is_alive == true)//플레이어 0이 살아있을 때 실행
            {
                p0(key);
            }
        }

        //플레이어 0제외한 플레이어 
        for (int p = 1; p < n_player; p++)
        {
            if (player[p].is_alive == true)
            {
                //일정 주기마다 플레이어 행동
                if (tick % period[p] == 0)
                {
                    the_rest_player(p);
                }
            }
        }


        display();

        tick += 10;
        Sleep(10);


        //여기에 인접한 칸에 아이템이 있을 때 상호작용 코드 만들기
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

        //여기에 인접한 칸에 플레이어 있을 때 상호작용 코드 만들기
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